#pragma once

#if defined(CMDMANAGER_EXPORTS) // inside DLL
#define DLL_EXPORT __declspec(dllexport)
#else // outside DLL
#define DLL_EXPORT __declspec(dllimport)
#endif  // CMDMANAGER_EXPORTS
