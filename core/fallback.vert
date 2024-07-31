layout(location = 0) in vec3 position;

layout(location = 0) uniform mat4 projection;
layout(location = 1) uniform mat4 head2Eye;
layout(location = 2) uniform mat4 world2Head;

void main()
{
	gl_Position = projection * head2Eye * world2Head * vec4(position, 1);
}