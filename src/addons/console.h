//
// Created by twelvee on 07.03.24.
//

#ifndef SIDEJS_CONSOLE_H
#define SIDEJS_CONSOLE_H
#include <JavaScriptCore/JavaScript.h>
#include "../runtime/runtime.h"

class console {
    JSObjectRef global;
    JSClassRef js_class;
    runtime* rt;
public:
    console(runtime* rt, JSObjectRef global) {
        this->rt = rt;
        this->global = global;
        this->init();
    }

    bool init();

    static JSValueRef console_log(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    static JSValueRef console_error(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
};


#endif //SIDEJS_CONSOLE_H
