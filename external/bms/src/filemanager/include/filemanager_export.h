#pragma once

#if defined(FILEMANAGER_EXPORTS) // inside DLL
#define EXPORT __declspec(dllexport)
#else // outside DLL
#define EXPORT __declspec(dllimport)
#endif  // FILEMANAGER_EXPORTS
