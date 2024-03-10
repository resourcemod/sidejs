//
// Created by twelvee on 3/10/24.
//

#include "binding.h"

bool binding::init() {
    if (this->js_class) {
        return true;
    }
    JSClassDefinition classDefinition = kJSClassDefinitionEmpty;

    static JSStaticFunction staticFunctions[] = {
            {"internalBinding", internalBinding, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete},
            { nullptr, nullptr, 0 }
    };

    JSObjectRef internalBindingObject = JSObjectMakeFunctionWithCallback(this->rt->globalContext, JSStringCreateWithUTF8CString("internalBinding"), internalBinding);
    JSObjectSetProperty(this->rt->globalContext, this->global, JSStringCreateWithUTF8CString("internalBinding"), internalBindingObject, kJSPropertyAttributeNone, nullptr);

    return false;
}

JSValueRef binding::internalBinding(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount,
                                    const JSValueRef *arguments, JSValueRef *exception) {
    return nullptr;
}
