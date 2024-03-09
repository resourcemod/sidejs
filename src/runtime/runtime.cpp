//
// Created by twelvee on 07.03.24.
//

#include "runtime.h"
#include "../addons/console.h"
#include <sys/stat.h>
#include <sstream>
#include <fstream>
#include "../addons/timers.h"

int runtime::loadFromFile(const std::string& path) {
    if (!this->fileExists(path)) {
        printf("Error: File not found. %s", path.c_str());
        return 1;
    }

    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    this->evaluate(buffer.str());
    return 0;
}

bool runtime::fileExists(const std::string& path) {
    struct stat buffer{};
    return (stat(path.c_str(), &buffer) == 0);
}

JSValueRef runtime::evaluate(const std::string& content) const {
    JSValueRef result = JSEvaluateScript(this->globalContext, JSStringCreateWithUTF8CString(content.c_str()), nullptr, nullptr, 1, nullptr);
    return result;
}

bool runtime::useConsole() {
    JSObjectRef globalObject = JSContextGetGlobalObject(this->globalContext);
    ASSERT_NULL(globalObject);
    new console(this, globalObject);
    return false;
}

bool runtime::useTimers() {
    JSObjectRef globalObject = JSContextGetGlobalObject(this->globalContext);
    ASSERT_NULL(globalObject);
    new timers(this, globalObject);
    return false;
}
