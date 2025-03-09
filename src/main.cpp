// HelloWorldCPP.cpp
// A simple Hello World macOS app using C++

#include <CoreGraphics/CoreGraphics.h>
#include <iostream>
#include <memory>
#include <objc/NSObjCRuntime.h>
#include <objc/message.h>
#include <objc/objc.h>
#include <objc/runtime.h>
#include <string>

typedef enum NSApplicationActivationPolicy {
    NSApplicationActivationPolicyRegular = 0,
    NSApplicationActivationPolicyAccessory = 1,
    NSApplicationActivationPolicyERROR = 2,
} NSApplicationActivationPolicy;

typedef enum NSWindowStyleMask {
    NSWindowStyleMaskBorderless = 0,
    NSWindowStyleMaskTitled = 1 << 0,
    NSWindowStyleMaskClosable = 1 << 1,
    NSWindowStyleMaskMiniaturizable = 1 << 2,
    NSWindowStyleMaskResizable = 1 << 3,
} NSWindowStyleMask;

typedef enum NSBackingStoreType {
    NSBackingStoreBuffered = 2,
} NSBackingStoreType;

// Template for objc_msgSend to make it more type-safe in C++
template <typename RetT, typename... ArgT>
RetT objc_msg_send(id self, SEL selector, ArgT... args)
{
    using MSGSendT = RetT (*)(id, SEL, ArgT...);
    return ((MSGSendT)objc_msgSend)(self, selector, args...);
}

// Helper to create SEL from string
SEL sel(const char* name)
{
    return sel_registerName(name);
}

// Helper to get class by name
// Class cls(const char* name) {
//     return objc_getClass(name);
// }

template <typename RetT = Class>
RetT cls(const char* name)
{
    auto clazz = objc_getClass(name);
    return reinterpret_cast<RetT>(clazz);
}

// Window delegate protocol implementation
void windowWillClose(id self, SEL _cmd, id notification)
{
    id NSApp = objc_msg_send<id>(
        cls<id>("NSApplication"),
        sel("sharedApplication")
    );
    objc_msg_send<void>(NSApp, sel("terminate:"), nullptr);
}

// Wrapper for NSString creation
id createNSString(const std::string& str)
{
    return objc_msg_send<id>(
        cls<id>("NSString"),
        sel("stringWithUTF8String:"),
        str.c_str()
    );
}

/**
 * https://developer.apple.com/documentation/foundation/nsautoreleasepool
 */
class AutoreleasePool {
private:
    id pool;

public:
    AutoreleasePool()
    {
        pool = objc_msg_send<id>(
            objc_msg_send<id>(cls<id>("NSAutoreleasePool"), sel("alloc")),
            sel("init")
        );
    }
    ~AutoreleasePool() { objc_msg_send<void>(pool, sel("drain")); }
};

int main(int argc, const char* argv[])
{
    try {
        // Use RAII for autorelease pool
        AutoreleasePool pool;

        // Get NSApplication instance
        id NSApp = objc_msg_send<id>(
            cls<id>("NSApplication"),
            sel("sharedApplication")
        );

        // Create window delegate class
        Class WindowDelegate = objc_allocateClassPair(
            cls("NSObject"),
            "WindowDelegate",
            0
        );
        class_addMethod(
            WindowDelegate,
            sel("windowWillClose:"),
            (IMP)windowWillClose,
            "v@:@"
        );
        objc_registerClassPair(WindowDelegate);

        // Create window delegate instance
        id delegate = objc_msg_send<id>(
            objc_msg_send<id>(
                reinterpret_cast<id>(WindowDelegate),
                sel("alloc")
            ),
            sel("init")
        );

        // Setup window
        CGRect frame = CGRectMake(100, 100, 400, 200);
        unsigned long
            styleMask = NSWindowStyleMask::NSWindowStyleMaskTitled |
                        NSWindowStyleMask::NSWindowStyleMaskClosable |
                        NSWindowStyleMask::NSWindowStyleMaskMiniaturizable |
                        NSWindowStyleMask::NSWindowStyleMaskResizable;

        id window = objc_msg_send<id>(cls<id>("NSWindow"), sel("alloc"));
        window = objc_msg_send<id>(
            window,
            sel("initWithContentRect:styleMask:backing:defer:"),
            frame,
            styleMask,
            NSBackingStoreBuffered,
            false
        );

        // Set window properties
        objc_msg_send<void>(
            window,
            sel("setTitle:"),
            createNSString("Hello World - C++ App")
        );
        objc_msg_send<void>(window, sel("setDelegate:"), delegate);

        // Create Hello World label
        id label = objc_msg_send<id>(cls<id>("NSTextField"), sel("alloc"));
        label = objc_msg_send<id>(
            label,
            sel("initWithFrame:"),
            CGRectMake(100, 80, 200, 40)
        );
        objc_msg_send<void>(
            label,
            sel("setStringValue:"),
            createNSString("Hello, World!")
        );
        objc_msg_send<void>(label, sel("setBezeled:"), false);
        objc_msg_send<void>(label, sel("setDrawsBackground:"), false);
        objc_msg_send<void>(label, sel("setEditable:"), false);
        objc_msg_send<void>(label, sel("setSelectable:"), false);

        // Set font
        id font = objc_msg_send<id>(
            cls<id>("NSFont"),
            sel("systemFontOfSize:"),
            24.0
        );
        objc_msg_send<void>(label, sel("setFont:"), font);

        // Add label to window content view
        id contentView = objc_msg_send<id>(window, sel("contentView"));
        objc_msg_send<void>(contentView, sel("addSubview:"), label);

        // Show window and activate app
        objc_msg_send<void>(window, sel("makeKeyAndOrderFront:"), nullptr);
        objc_msg_send<void>(NSApp, sel("activateIgnoringOtherApps:"), true);

        // Run the application
        objc_msg_send<void>(NSApp, sel("run"));

        // Pool will be automatically drained at the end of scope

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
}
