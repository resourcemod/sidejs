//
// Created by twelvee on 3/10/24.
//

#ifndef SIDEJS_URL_H
#define SIDEJS_URL_H

#include "../../third_party/ada/ada.h"
#include <JavaScriptCore/JavaScript.h>
#include <cstdio>

enum url_update_action {
    kProtocol = 0,
    kHost = 1,
    kHostname = 2,
    kPort = 3,
    kUsername = 4,
    kPassword = 5,
    kPathname = 6,
    kSearch = 7,
    kHash = 8,
    kHref = 9,
};

class url {
public:
    static JSValueRef DomainToASCII(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

    static JSValueRef DomainToUnicode(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

    static JSValueRef CanParse(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

    static JSValueRef Format(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    static JSValueRef GetOrigin(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    static JSValueRef Parse(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    static JSValueRef Update(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

private:

    static void* fast_can_parse_methods_[];
};

void ThrowInvalidURL(std::string_view input,
                     std::optional<std::string> base);
std::string FromFilePath(std::string_view file_path);
std::optional<std::string> FileURLToPath(const ada::url_aggregator& file_url);
void FromNamespacedPath(std::string* path);


#endif //SIDEJS_URL_H
