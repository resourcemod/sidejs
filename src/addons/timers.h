//
// Created by twelvee on 3/9/24.
//

#ifndef SIDEJS_TIMERS_H
#define SIDEJS_TIMERS_H

#include "../runtime/runtime.h"
#include <uv.h>
#include <map>

class timers {
    JSObjectRef global;
    JSClassRef js_class;
    runtime* rt;
public:
    timers(runtime* rt, JSObjectRef global) {
        this->rt = rt;
        this->global = global;
        this->init();
    }

    bool init();

    static JSValueRef setTimeout(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    static JSValueRef setImmediate(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    static JSValueRef setInterval(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    static JSValueRef clearTimeout(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    static JSValueRef clearImmediate(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    static JSValueRef clearInterval(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

    static void onTimeoutCallback(uv_timer_t *handle);
    static void onIntervalCallback(uv_timer_t *handle);

    static void process_mem_usage(double& vm_usage, double& resident_set);
};

#endif //SIDEJS_TIMERS_H
