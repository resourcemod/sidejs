//
// Created by twelvee on 07.03.24.
//

#ifndef SIDEJS_RUNTIME_H
#define SIDEJS_RUNTIME_H

#include <JavaScriptCore/JavaScript.h>
#include <memory>
#include <map>
#include <uv.h>

#define ASSERT_NULL(var) if (var == nullptr) return true;

class runtime {
public:
    runtime() {
        this->environment = JSContextGroupCreate();
        this->globalContext = JSGlobalContextCreateInGroup(this->environment, nullptr);
        this->loop = uv_loop_new();
    }

    JSContextGroupRef environment;
    JSGlobalContextRef globalContext;
    uv_loop_t* loop;

    std::map<JSValueRef, uv_timer_t> timerRefs;

    bool fileExists(const std::string& path);
    int loadFromFile(const std::string& path);

    bool useConsole();
    bool useTimers();
    bool useBinding();

    JSValueRef evaluate(const std::string &content) const;
};


#endif //SIDEJS_RUNTIME_H
