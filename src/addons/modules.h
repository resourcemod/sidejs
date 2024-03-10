//
// Created by twelvee on 3/10/24.
//

#ifndef SIDEJS_MODULES_H
#define SIDEJS_MODULES_H


#include <string>
#include <optional>
#include <JavaScriptCore/JavaScript.h>
#include "../../third_party/simdjson/simdjson.h"
#include <unordered_map>

class modules {
public:
    struct PackageConfig {
        std::string file_path;
        std::optional<std::string> name;
        std::optional<std::string> main;
        std::string type = "none";
        std::optional<std::string> exports;
        std::optional<std::string> imports;
        std::string raw_json;
    };

    struct ErrorContext {
        std::optional<std::string> base;
        std::string specifier;
        bool is_esm;
    };

    static JSValueRef ReadPackageJSON(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    static JSValueRef GetNearestParentPackageJSON(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    static JSValueRef GetNearestParentPackageJSONType(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    static JSValueRef GetPackageScopeConfig(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

private:
    std::unordered_map<std::string, PackageConfig> package_configs_;
    simdjson::ondemand::parser json_parser;
    // returns null on error
    static const PackageConfig* GetPackageJSON(
            std::string_view path,
            ErrorContext* error_context = nullptr);
    static const PackageConfig* TraverseParent(std::string_view check_path);
};


#endif //SIDEJS_MODULES_H
