//
// Created by twelvee on 3/9/24.
//

#include <fstream>
#include <iostream>
#include "timers.h"
#include "../utils.h"

extern runtime* sidejsRuntime;

struct TIMEOUT_STRUCT {
    JSContextRef ctx;
    JSValueRef fn;
    JSObjectRef obj;
    JSValueRef *exception;
};

void timers::onTimeoutCallback(uv_timer_t *handle) {
    auto* h = static_cast<TIMEOUT_STRUCT*>(handle->data);
    JSObjectCallAsFunction(h->ctx, JSValueToObject(h->ctx, h->fn, h->exception), h->obj, 0, nullptr, h->exception);
    uv_timer_stop(handle);
    sidejsRuntime->timerRefs.erase(h->fn);
}

void timers::onIntervalCallback(uv_timer_t *handle) {
    auto* h = static_cast<TIMEOUT_STRUCT*>(handle->data);
    JSObjectCallAsFunction(h->ctx, JSValueToObject(h->ctx, h->fn, h->exception), h->obj, 0, nullptr, h->exception);
}

bool timers::init() {
    if (this->js_class) {
        return true;
    }
    JSClassDefinition classDefinition = kJSClassDefinitionEmpty;

    static JSStaticFunction staticFunctions[] = {
            {"setImmediate", setImmediate, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete},
            {"setTimeout", setTimeout, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete},
            {"setInterval", setInterval, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete},
            {"clearTimeout", clearTimeout, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete},
            {"clearInterval", clearTimeout, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete},
            {"clearImmediate", clearImmediate, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete},
            { nullptr, nullptr, 0 }
    };

    JSObjectRef setImmediateObject = JSObjectMakeFunctionWithCallback(this->rt->globalContext, JSStringCreateWithUTF8CString("setImmediate"), setImmediate);
    JSObjectSetProperty(this->rt->globalContext, this->global, JSStringCreateWithUTF8CString("setImmediate"), setImmediateObject, kJSPropertyAttributeNone, nullptr);

    JSObjectRef setTimeoutObj = JSObjectMakeFunctionWithCallback(this->rt->globalContext, JSStringCreateWithUTF8CString("setTimeout"), setTimeout);
    JSObjectSetProperty(this->rt->globalContext, this->global, JSStringCreateWithUTF8CString("setTimeout"), setTimeoutObj, kJSPropertyAttributeNone, nullptr);

    JSObjectRef setIntervalObj = JSObjectMakeFunctionWithCallback(this->rt->globalContext, JSStringCreateWithUTF8CString("setInterval"), setInterval);
    JSObjectSetProperty(this->rt->globalContext, this->global, JSStringCreateWithUTF8CString("setInterval"), setIntervalObj, kJSPropertyAttributeNone, nullptr);

    JSObjectRef clearTimeoutObj = JSObjectMakeFunctionWithCallback(this->rt->globalContext, JSStringCreateWithUTF8CString("clearTimeout"), clearTimeout);
    JSObjectSetProperty(this->rt->globalContext, this->global, JSStringCreateWithUTF8CString("clearTimeout"), clearTimeoutObj, kJSPropertyAttributeNone, nullptr);

    JSObjectRef clearIntervalObj = JSObjectMakeFunctionWithCallback(this->rt->globalContext, JSStringCreateWithUTF8CString("clearInterval"), clearInterval);
    JSObjectSetProperty(this->rt->globalContext, this->global, JSStringCreateWithUTF8CString("clearInterval"), clearIntervalObj, kJSPropertyAttributeNone, nullptr);

    JSObjectRef clearImmediateObj = JSObjectMakeFunctionWithCallback(this->rt->globalContext, JSStringCreateWithUTF8CString("clearImmediate"), clearImmediate);
    JSObjectSetProperty(this->rt->globalContext, this->global, JSStringCreateWithUTF8CString("clearImmediate"), clearImmediateObj, kJSPropertyAttributeNone, nullptr);

    return false;
}

JSValueRef timers::setTimeout(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount,
                              const JSValueRef *arguments, JSValueRef *exception) {
    if (argumentCount != 2) {
        JS_CREATE_EXCEPTION(exception, ctx, "Error: undefined signature. 2 parameters expected.");
        return JSValueMakeUndefined(ctx);
    }
    if (!JSObjectIsFunction(ctx, JSValueToObject(ctx, arguments[0], exception))) {
        JS_CREATE_EXCEPTION(exception, ctx, "Error: first argument must be function.");
        return JSValueMakeUndefined(ctx);
    }
    if (!JSValueIsNumber(ctx, arguments[1])) {
        JS_CREATE_EXCEPTION(exception, ctx, "Error: second argument must be number.");
        return JSValueMakeUndefined(ctx);
    }

    int timeout = (int) JSValueToNumber(ctx, arguments[1], exception);
    auto *timeoutStruct = new TIMEOUT_STRUCT(ctx, arguments[0], object, exception);
    uv_timer_t timer;
    sidejsRuntime->timerRefs[arguments[0]] = timer;
    sidejsRuntime->timerRefs[arguments[0]].data = timeoutStruct;
    uv_timer_init(sidejsRuntime->loop, &sidejsRuntime->timerRefs[arguments[0]]);
    uv_timer_start(&sidejsRuntime->timerRefs[arguments[0]], timers::onTimeoutCallback, timeout, 0);
    return JSValueMakeBoolean(ctx, true);
}

JSValueRef timers::setInterval(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount,
                               const JSValueRef *arguments, JSValueRef *exception) {
    if (argumentCount != 2) {
        JS_CREATE_EXCEPTION(exception, ctx, "Error: undefined signature. 2 parameters expected.");
        return JSValueMakeUndefined(ctx);
    }
    if (!JSObjectIsFunction(ctx, JSValueToObject(ctx, arguments[0], exception))) {
        JS_CREATE_EXCEPTION(exception, ctx, "Error: first argument must be function.");
        return JSValueMakeUndefined(ctx);
    }
    if (!JSValueIsNumber(ctx, arguments[1])) {
        JS_CREATE_EXCEPTION(exception, ctx, "Error: second argument must be number.");
        return JSValueMakeUndefined(ctx);
    }

    int timeout = (int) JSValueToNumber(ctx, arguments[1], exception);
    auto *timeoutStruct = new TIMEOUT_STRUCT(ctx, arguments[0], object, exception);
    uv_timer_t timer;
    sidejsRuntime->timerRefs[arguments[0]] = timer;
    sidejsRuntime->timerRefs[arguments[0]].data = timeoutStruct;
    uv_timer_init(sidejsRuntime->loop, &sidejsRuntime->timerRefs[arguments[0]]);

    uv_timer_start(&sidejsRuntime->timerRefs[arguments[0]], timers::onIntervalCallback, 0, timeout);
    return JSValueMakeBoolean(ctx, true);
}

JSValueRef timers::clearTimeout(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount,
                                const JSValueRef *arguments, JSValueRef *exception) {
    return nullptr;
}

JSValueRef timers::clearInterval(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount,
                                 const JSValueRef *arguments, JSValueRef *exception) {
    return nullptr;
}

JSValueRef timers::setImmediate(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount,
                                const JSValueRef *arguments, JSValueRef *exception) {
    return nullptr;
}

JSValueRef timers::clearImmediate(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount,
                                  const JSValueRef *arguments, JSValueRef *exception) {
    return nullptr;
}
