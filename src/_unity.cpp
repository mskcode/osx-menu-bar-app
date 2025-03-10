// File combining all the source files into a single translation unit which
// then can be compiled into a single binary file (executable). This technique
// is called "unity build", "unified build", "jumbo build" or "blob build".

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include <CoreGraphics/CoreGraphics.h>
#include <objc/objc.h>
#include <objc/message.h>
#include <objc/NSObjCRuntime.h>
#include <objc/runtime.h>

// Includes for all CPP files.
// Notice that the order of the includes matter when doing things like this
// since there's no header files.

#include "osx.cpp"
#include "main.cpp"
