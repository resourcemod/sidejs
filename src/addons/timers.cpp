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

void timers::process_mem_usage(double& vm_usage, double& resident_set)
{
    using std::ios_base;
    using std::ifstream;
    using std::string;

    vm_usage     = 0.0;
    resident_set = 0.0;

    // 'file' stat seems to give the most reliable results
    //
    ifstream stat_stream("/proc/self/stat",ios_base::in);

    // dummy vars for leading entries in stat that we don't care about
    //
    string pid, comm, state, ppid, pgrp, session, tty_nr;
    string tpgid, flags, minflt, cminflt, majflt, cmajflt;
    string utime, stime, cutime, cstime, priority, nice;
    string O, itrealvalue, starttime;

    // the two fields we want
    //
    unsigned long vsize;
    long rss;

    stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
                >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
                >> utime >> stime >> cutime >> cstime >> priority >> nice
                >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

    stat_stream.close();

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage     = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}

void timers::onIntervalCallback(uv_timer_t *handle) {
    auto* h = static_cast<TIMEOUT_STRUCT*>(handle->data);
    JSObjectCallAsFunction(h->ctx, JSValueToObject(h->ctx, h->fn, h->exception), h->obj, 0, nullptr, h->exception);
    double vm, rss;
    process_mem_usage(vm, rss);
    std::cout << "VM: " << vm << "; RSS: " << rss << std::endl;
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
