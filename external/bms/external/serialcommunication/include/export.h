#pragma once

#if defined(SERIALCOMMUNICATION_EXPORTS) // inside DLL
#define DLLAPI __declspec(dllexport)
#else // outside DLL
#define DLLAPI __declspec(dllimport)
#endif  // SERIALCOMMUNICATION_EXPORTS
