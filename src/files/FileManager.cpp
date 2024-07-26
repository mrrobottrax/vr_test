#include "pch.h"
#include "FileManager.h"

#ifdef _WINDOWS
const char *FileManager::LoadResourceBytes(int id, const wchar_t *type)
{
	HRSRC handle = FindResource(NULL, MAKEINTRESOURCE(id), type);

	if (handle == 0) throw std::runtime_error(std::format("Failed to find resource id = {}", id));

	HGLOBAL global = ::LoadResource(NULL, handle);

	if (global == 0) throw std::runtime_error(std::format("Failed to find resource id = {}", id));

	return reinterpret_cast<char *>(LockResource(global));
}
#endif // _WINDOWS