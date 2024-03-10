//
// Created by twelvee on 3/10/24.
//

#ifndef SIDEJS_BINDING_H
#define SIDEJS_BINDING_H


#include "JavaScriptCore/JavaScript.h"
#include "../runtime/runtime.h"

class binding {
    JSObjectRef global;
    JSClassRef js_class;
    runtime* rt;
public:
    binding(runtime* rt, JSObjectRef global) {
        this->rt = rt;
        this->global = global;
        this->init();
    }

    bool init();

    static JSValueRef internalBinding(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
};


#endif //SIDEJS_BINDING_H
