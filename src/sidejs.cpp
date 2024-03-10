#include <iostream>
#include "runtime/runtime.h"
#include <uv.h>

using namespace std;

runtime* sidejsRuntime;

int main(int argc, const char * argv[]) {
    sidejsRuntime = new runtime();

    sidejsRuntime->useConsole();
    sidejsRuntime->useTimers();
    sidejsRuntime->useBinding();

    if (argc > 0) {
        sidejsRuntime->loadFromFile(argv[1]);
        uv_run(sidejsRuntime->loop, UV_RUN_DEFAULT);
        return 0;
    }
    return 0;
}