//
// Created by twelvee on 3/9/24.
//

#ifndef SIDEJS_UTILS_H
#define SIDEJS_UTILS_H

#include <JavaScriptCore/JavaScript.h>
#include <string>

#define JS_CREATE_EXCEPTION(exception, ctx, str)                                                                    \
        auto exceptionString = JSStringCreateWithUTF8CString("Error: undefined signature. 2 parameters expected."); \
        auto ex = JSValueMakeString(ctx, exceptionString);                                                          \
        exception = &ex;

class utils {
public:
    static std::string JSStringToStdString(JSStringRef jsString);

    static JSStringRef JSRefToStringRef(JSContextRef ctx, JSValueRef argument, JSValueRef *exception);

    static JSValueRef JSCreateException(JSContextRef ctx, const std::string &);
};


#endif //SIDEJS_UTILS_H
