#pragma once
#include "Version.inc"

#define _STR(x) #x
#define STR(x) _STR(x)
#define VERSION_NUMBER VERSION_MAIN,VERSION_MAJOR,VERSION_MINOR,VERSION_BUILDCNT
#define VERSION_STRING STR(VERSION_MAIN) "." STR(VERSION_MAJOR) "." \
                               STR(VERSION_MINOR) "." STR(VERSION_BUILDCNT)
