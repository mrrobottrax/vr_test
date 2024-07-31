#include <pch.h>
#include <app/App.h>
#include "GlDebug.h"
#include "GlInstance.h"
#include "ShaderProgram.h"

float vertices[] = {
	0, 0, -1,
	0, 1, -1,
	1, 1, -1
};

void GlInstance::Init(SDL_Window *pWindow)
{
	const char *glsl_version = "#version 150";

	// sdl settings

	int contextFlags = 0;
	contextFlags |= SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;

#ifdef _DEBUG
	contextFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif // _DEBUG

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, contextFlags); // always required on Mac
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);

	// sdl init

	m_sdlGlContext = SDL_GL_CreateContext(pWindow);
	if (m_sdlGlContext == nullptr)
	{
		throw std::runtime_error(std::format("SDL_GL_CreateContext failed {}", SDL_GetError()));
	}

	SDL_GL_MakeCurrent(pWindow, m_sdlGlContext);
	SDL_GL_SetSwapInterval(1); // vsync

	// glad
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	// IMGUI
	IMGUI_CHECKVERSION();
	m_pImguiContext = ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(pWindow, m_sdlGlContext);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// init gl stuff

	// init render targets
	m_leftEyeFramebuffer.Init();
	m_rightEyeFramebuffer.Init();

	// create vertex array
	glGenVertexArrays(1, &m_vertexArray);
	glBindVertexArray(m_vertexArray);

	// create vertex buffer
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// set vertex format
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	// create shader
	const void *vertexShaderSource = SDL_LoadFile("core/fallback.vert", nullptr);
	const void *fragmentShaderSource = SDL_LoadFile("core/fallback.frag", nullptr);
	m_fallbackShaderProgram.Compile(vertexShaderSource, fragmentShaderSource, "Default");
	delete[] vertexShaderSource;
	delete[] fragmentShaderSource;
}

void GlInstance::Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_GL_DeleteContext(m_sdlGlContext);
}

static vr::HmdMatrix44_t InvertMatrix(const vr::HmdMatrix34_t &matrix)
{
	vr::HmdMatrix44_t inverseMatrix{};

	// right column is a freebie because bottom row is assumed 0, 0, 0, 1
	inverseMatrix.m[0][3] = -matrix.m[0][3];
	inverseMatrix.m[1][3] = -matrix.m[1][3];
	inverseMatrix.m[2][3] = -matrix.m[2][3];

	// solve the rest (3x3) using elimination / augmented matrix

	float l[4][4]{}; 	// left side of augmented matrix
	float(&r)[4][4] = inverseMatrix.m; // right side

	// copy starting matrix to left side
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			l[i][j] = matrix.m[i][j];
		}

		l[i][3] = 0;
	}
	l[3][3] = 1;

	// set right side diagonals to 1
	for (int i = 0; i < 4; ++i)
	{
		r[i][i] = 1;
	}

	// use elimination to set column, rowB to 0 in left side
	auto eliminate = [&l, &r] (int rowA, int rowB, int column) -> void
		{
			const float multiplier = l[rowB][column] / l[rowA][column];
			for (int i = 0; i < 4; ++i)
			{
				l[rowB][i] -= l[rowA][i] * multiplier;
				r[rowB][i] -= r[rowA][i] * multiplier;
			}
		};

	// 1. use row 0 on row 1 to eliminate 1, 0 (row 1 minus a multiple of row 0)
	eliminate(0, 1, 0);
	// 2. use row 0 on row 2 to eliminate 2, 0
	eliminate(0, 2, 0);
	// 3. use row 1 on row 2 to eliminate 2, 1
	eliminate(1, 2, 1);

	// 3x3 submatrix should now be an upper triangle matrix

	// 4. use row 2 on row 1 to eliminate 1, 2
	eliminate(2, 1, 2);
	// 5. use row 2 on row 0 to eliminate 0, 2
	eliminate(2, 0, 2);
	// 6. use row 1 on row 0 to eliminate 0, 1
	eliminate(1, 0, 1);

	// matrix should now be diagonal only

	// normalize each row
	for (int i = 0; i < 3; ++i) // bottom row is already normal
	{
		const float multiplier = 1.0f / l[i][i];
		for (int j = 0; j < 4; ++j)
		{
			//l[i][j] *= multiplier;
			r[i][j] *= multiplier;
		}
	}

	return inverseMatrix;
}

void GlInstance::RenderScene(vr::EVREye eye, vr::TrackedDevicePose_t renderPose)
{
	// setup
	RenderTarget &rt = eye == vr::Eye_Right ? m_rightEyeFramebuffer : m_leftEyeFramebuffer;

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rt.FramebufferName());
	glViewport(0, 0, rt.Width(), rt.Height());

	// clear
	glClearColor(0, 1, 1, 1);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_fallbackShaderProgram.Id());

	// set hmd matrix
	const vr::HmdMatrix34_t &headToWorldMat = renderPose.mDeviceToAbsoluteTracking;
	vr::HmdMatrix44_t worldToHeadMat = InvertMatrix(headToWorldMat);
	glUniformMatrix4fv(2, 1, GL_TRUE, reinterpret_cast<float(&)[16]>(worldToHeadMat.m));

	// set eye pos matrix
	const vr::HmdMatrix34_t &eyeToHeadMat = vr::VRSystem()->GetEyeToHeadTransform(eye);
	vr::HmdMatrix44_t headToEyeMat = InvertMatrix(eyeToHeadMat);
	glUniformMatrix4fv(1, 1, GL_TRUE, reinterpret_cast<float(&)[16]>(headToEyeMat.m));

	// set projection matrix
	vr::HmdMatrix44_t projMat = vr::VRSystem()->GetProjectionMatrix(eye, 0.01f, 1000.0f);
	glUniformMatrix4fv(0, 1, GL_TRUE, reinterpret_cast<float(&)[16]>(projMat.m));

	// draw test triangle
	glBindVertexArray(m_vertexArray);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	glUseProgram(0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void GlInstance::RenderFrame()
{
	vr::TrackedDevicePose_t renderPose;
	vr::VRCompositor()->WaitGetPoses(&renderPose, 1, nullptr, 0);

	RenderScene(vr::Eye_Left, renderPose);
	RenderScene(vr::Eye_Right, renderPose);

	// submit textures
	vr::Texture_t leftEye{};
	leftEye.handle = (void *)(uintptr_t)m_leftEyeFramebuffer.RenderbufferName();
	leftEye.eType = vr::TextureType_OpenGL;
	leftEye.eColorSpace = vr::ColorSpace_Gamma;

	vr::Texture_t rightEye{};
	rightEye.handle = (void *)(uintptr_t)m_rightEyeFramebuffer.RenderbufferName();
	rightEye.eType = vr::TextureType_OpenGL;
	rightEye.eColorSpace = vr::ColorSpace_Gamma;

	// todo: profile rendering eyes on seperate rts and submitting at once, or sequencially on same rt
	vr::VRCompositor()->Submit(vr::Eye_Left, &leftEye, nullptr, vr::Submit_GlRenderBuffer);
	vr::VRCompositor()->Submit(vr::Eye_Right, &rightEye, nullptr, vr::Submit_GlRenderBuffer);

	// copy to main window
	int width, height;
	SDL_GL_GetDrawableSize(App().MainWindow(), &width, &height);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_leftEyeFramebuffer.FramebufferName());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBlitFramebuffer(
		0, 0,
		m_leftEyeFramebuffer.Width(), m_leftEyeFramebuffer.Height(),
		0, 0,
		width, height,
		GL_COLOR_BUFFER_BIT, GL_NEAREST
	);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	// imgui
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::ShowDemoWindow();

	ImGui::Render();

	// draw imgui
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// end frame
	SDL_GL_SwapWindow(App().MainWindow());
	glFinish();
}