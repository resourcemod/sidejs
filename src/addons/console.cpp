//
// Created by twelvee on 07.03.24.
//

#include <cstdio>
#include <iostream>
#include "console.h"
#include "../utils.h"

bool console::init() {
    if (this->js_class) {
        return true;
    }
    JSClassDefinition classDefinition = kJSClassDefinitionEmpty;

    static JSStaticFunction staticFunctions[] = {
            {"log", console_log, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete},
            {"debug", console_log, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete}, // alias for console.log
            {"info", console_log, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete}, // alias for console.log
            {"error", console_error, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete},
            { nullptr, nullptr, 0 }
    };

    classDefinition.className = "Console";
    classDefinition.attributes = kJSClassAttributeNone;
    classDefinition.staticFunctions = staticFunctions;

    this->js_class = JSClassCreate(&classDefinition);

    JSObjectRef consoleObj = JSObjectMake(this->rt->globalContext, this->js_class, nullptr);
    JSObjectSetProperty(this->rt->globalContext, this->global, JSStringCreateWithUTF8CString("console"), consoleObj, kJSPropertyAttributeNone, nullptr);
    return false;
}

JSValueRef console::console_log(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount,
                                const JSValueRef *arguments, JSValueRef *exception) {
    if (argumentCount == 0) {
        return JSValueMakeNull(ctx);
    }
    for (int i = 0; i < argumentCount; i++) {
        std::string str = utils::JSStringToStdString(utils::JSRefToStringRef(ctx, arguments[i], exception));
        printf("%s \n", str.c_str());
    }
    return JSValueMakeNull(ctx);
}

JSValueRef console::console_error(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount,
                                const JSValueRef *arguments, JSValueRef *exception) {
    if (argumentCount == 0) {
        return JSValueMakeNull(ctx);
    }
    for (int i = 0; i < argumentCount; i++) {
        std::string str = utils::JSStringToStdString(utils::JSRefToStringRef(ctx, arguments[i], exception));
        fprintf(stderr, "%s \n", str.c_str());
    }
    return JSValueMakeNull(ctx);
}

