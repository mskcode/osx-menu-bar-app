int main(int argc, const char* argv[])
{
    try {
        // Use RAII for autorelease pool
        osx::AutoreleasePool pool;

        // Get NSApplication instance
        id NSApp = osx::objc_msg_send<id>(
            osx::cls<id>("NSApplication"),
            osx::sel("sharedApplication")
        );

        // Create window delegate class
        Class WindowDelegate = objc_allocateClassPair(
            osx::cls("NSObject"),
            "WindowDelegate",
            0
        );
        class_addMethod(
            WindowDelegate,
            osx::sel("windowWillClose:"),
            (IMP)osx::windowWillClose,
            "v@:@"
        );
        objc_registerClassPair(WindowDelegate);

        // Create window delegate instance
        id delegate = osx::objc_msg_send<id>(
            osx::objc_msg_send<id>(
                reinterpret_cast<id>(WindowDelegate),
                osx::sel("alloc")
            ),
            osx::sel("init")
        );

        // Setup window
        CGRect frame = CGRectMake(100, 100, 400, 200);
        unsigned long styleMask =
            osx::NSWindowStyleMask::NSWindowStyleMaskTitled |
            osx::NSWindowStyleMask::NSWindowStyleMaskClosable |
            osx::NSWindowStyleMask::NSWindowStyleMaskMiniaturizable |
            osx::NSWindowStyleMask::NSWindowStyleMaskResizable;

        id window = osx::objc_msg_send<id>(
            osx::cls<id>("NSWindow"),
            osx::sel("alloc")
        );
        window = osx::objc_msg_send<id>(
            window,
            osx::sel("initWithContentRect:styleMask:backing:defer:"),
            frame,
            styleMask,
            osx::NSBackingStoreBuffered,
            false
        );

        // Set window properties
        osx::objc_msg_send<void>(
            window,
            osx::sel("setTitle:"),
            osx::createNSString("Hello World - C++ App")
        );
        osx::objc_msg_send<void>(window, osx::sel("setDelegate:"), delegate);

        // Create Hello World label
        id label = osx::objc_msg_send<id>(
            osx::cls<id>("NSTextField"),
            osx::sel("alloc")
        );
        label = osx::objc_msg_send<id>(
            label,
            osx::sel("initWithFrame:"),
            CGRectMake(100, 80, 200, 40)
        );
        osx::objc_msg_send<void>(
            label,
            osx::sel("setStringValue:"),
            osx::createNSString("Hello, World!")
        );
        osx::objc_msg_send<void>(label, osx::sel("setBezeled:"), false);
        osx::objc_msg_send<void>(label, osx::sel("setDrawsBackground:"), false);
        osx::objc_msg_send<void>(label, osx::sel("setEditable:"), false);
        osx::objc_msg_send<void>(label, osx::sel("setSelectable:"), false);

        // Set font
        id font = osx::objc_msg_send<id>(
            osx::cls<id>("NSFont"),
            osx::sel("systemFontOfSize:"),
            24.0
        );
        osx::objc_msg_send<void>(label, osx::sel("setFont:"), font);

        // Add label to window content view
        id contentView = osx::objc_msg_send<id>(
            window,
            osx::sel("contentView")
        );
        osx::objc_msg_send<void>(contentView, osx::sel("addSubview:"), label);

        // Show window and activate app
        osx::objc_msg_send<void>(
            window,
            osx::sel("makeKeyAndOrderFront:"),
            nullptr
        );
        osx::objc_msg_send<void>(
            NSApp,
            osx::sel("activateIgnoringOtherApps:"),
            true
        );

        // Run the application
        osx::objc_msg_send<void>(NSApp, osx::sel("run"));

        // Pool will be automatically drained at the end of scope

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
}
