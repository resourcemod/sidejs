//
// Created by twelvee on 3/9/24.
//

#include "utils.h"

JSStringRef utils::JSRefToStringRef(JSContextRef ctx, JSValueRef argument, JSValueRef *exception) {
    return JSValueToStringCopy(ctx, argument, exception);
}

std::string utils::JSStringToStdString(JSStringRef jsString) {
    size_t maxBufferSize = JSStringGetMaximumUTF8CStringSize(jsString);
    char* utf8Buffer = new char[maxBufferSize];
    size_t bytesWritten = JSStringGetUTF8CString(jsString, utf8Buffer, maxBufferSize);
    std::string utf_string = std::string(utf8Buffer, bytesWritten -1);
    delete [] utf8Buffer;
    return utf_string;
}

JSValueRef utils::JSCreateException(JSContextRef ctx, const std::string&) {
    auto exceptionString = JSStringCreateWithUTF8CString("Error: undefined signature. 2 parameters expected.");
    auto ex = JSValueMakeString(ctx, exceptionString);
    return ex;
}
