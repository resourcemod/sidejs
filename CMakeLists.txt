cmake_minimum_required(VERSION 3.21)
project(sidejs)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        third_party/JavaScriptCore/includes
        third_party/libuv
        third_party/simdjson
        third_party/ada
)

add_executable(
        sidejs
        src/sidejs.cpp
        src/addons/console.cpp
        src/addons/console.h
        src/runtime/runtime.cpp
        src/runtime/runtime.h
        src/utils.cpp
        src/utils.h
        src/addons/timers.cpp
        src/addons/timers.h
        src/addons/binding.cpp
        src/addons/binding.h
        src/addons/modules.cpp
        src/addons/modules.h
        src/addons/url.cpp
        src/addons/url.h

        third_party/simdjson/simdjson.cpp
        third_party/simdjson/simdjson.h
        third_party/ada/ada.cpp
        third_party/ada/ada.h
)

target_link_libraries(sidejs PRIVATE ${CMAKE_SOURCE_DIR}/lib/libJavaScriptCore.a)
target_link_libraries(sidejs PRIVATE ${CMAKE_SOURCE_DIR}/lib/libWTF.a)
target_link_libraries(sidejs PRIVATE ${CMAKE_SOURCE_DIR}/lib/libbmalloc.a)
target_link_libraries(sidejs PUBLIC icui18n)
target_link_libraries(sidejs PUBLIC icuuc)
target_link_libraries(sidejs PUBLIC atomic)
target_link_libraries(sidejs PRIVATE ${CMAKE_SOURCE_DIR}/lib/libuv.a)