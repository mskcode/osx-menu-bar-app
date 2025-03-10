namespace osx {

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

// Helper to get class by name
template <typename RetT = Class>
RetT cls(const char* name)
{
    auto clazz = objc_getClass(name);
    return reinterpret_cast<RetT>(clazz);
}

// Helper to create SEL from string
SEL sel(const char* name)
{
    return sel_registerName(name);
}

// Window delegate protocol implementation
void windowWillClose(id self, SEL _cmd, id notification)
{
    id NSApp = osx::objc_msg_send<id>(
        osx::cls<id>("NSApplication"),
        osx::sel("sharedApplication")
    );
    osx::objc_msg_send<void>(NSApp, sel("terminate:"), nullptr);
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

} // namespace osx
