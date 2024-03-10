//
// Created by twelvee on 3/10/24.
//

#include "modules.h"
#include "../../third_party/ada/ada.h"
#include "url.h"
#include "../utils.h"

JSValueRef modules::ReadPackageJSON(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount,
                                  const JSValueRef *arguments, JSValueRef *exception) {
    CHECK_GE(argumentCount, 1);  // path, [is_esm, base, specifier]
    CHECK(JSValueIsString(ctx, arguments[0]));  // path
    CHECK(JSValueIsBoolean(ctx, arguments[1]));  // path

    Utf8Value path(ctx, arguments[0]);
    bool is_esm = JSValueToBoolean(ctx, arguments[1]);
    auto error_context = ErrorContext();
    if (is_esm) {
        CHECK(JSValueIsUndefined(ctx, arguments[2]) || JSValueIsString(ctx, arguments[2]));  // base
        CHECK(JSValueIsString(ctx, arguments[3]));                            // specifier

        if (JSValueIsString(ctx, arguments[2])) {
            Utf8Value base_value(ctx, arguments[2]);
            error_context.base = base_value.ToString();
        }
        Utf8Value specifier(ctx, arguments[3]);
        error_context.specifier = specifier.ToString();
    }

    auto package_json =
            GetPackageJSON(path.ToString(), is_esm ? &error_context : nullptr);
    if (package_json == nullptr) {
        return JSValueMakeNull(ctx);
    }

    //args.GetReturnValue().Set(package_json->Serialize(realm));
    return JSValueMakeFromJSONString(ctx, JSStringCreateWithUTF8CString(package_json->raw_json.c_str()));
}

JSValueRef modules::GetNearestParentPackageJSON(JSContextRef ctx, JSObjectRef function, JSObjectRef object,
                                              size_t argumentCount, const JSValueRef *arguments,
                                              JSValueRef *exception) {
    CHECK_GE(argumentCount, 1);
    CHECK(JSValueIsString(ctx, arguments[0]));

    Utf8Value path_value(ctx, arguments[0]);
    auto package_json = TraverseParent(path_value.ToStringView());

    if (package_json != nullptr) {
        //args.GetReturnValue().Set(package_json->Serialize(realm));
        return JSValueMakeFromJSONString(ctx, JSStringCreateWithUTF8CString(package_json->raw_json.c_str()));
    }
    return JSValueMakeNull(ctx);
}

JSValueRef modules::GetNearestParentPackageJSONType(JSContextRef ctx, JSObjectRef function, JSObjectRef object,
                                                  size_t argumentCount, const JSValueRef *arguments,
                                                  JSValueRef *exception) {
    CHECK_GE(argumentCount, 1);
    CHECK(JSValueIsString(ctx, arguments[0]));

    Utf8Value path(ctx, arguments[0]);
    auto package_json = TraverseParent(path.ToStringView());

    if (package_json == nullptr) {
        return JSValueMakeNull(ctx);
    }

/*    Local<Value> values[3] = {
            ToV8Value(realm->context(), package_json->type).ToLocalChecked(),
            ToV8Value(realm->context(), package_json->file_path).ToLocalChecked(),
            ToV8Value(realm->context(), package_json->raw_json).ToLocalChecked()};
    args.GetReturnValue().Set(Array::New(realm->isolate(), values, 3));*/
    JSValueRef array[3] = {
            JSValueMakeString(ctx, JSStringCreateWithUTF8CString(package_json->type.c_str())),
            JSValueMakeString(ctx, JSStringCreateWithUTF8CString(package_json->file_path.c_str())),
            JSValueMakeString(ctx, JSStringCreateWithUTF8CString(package_json->raw_json.c_str())),
    };
    return JSObjectMakeArray(ctx, 3, array, exception); // holy shit I'm going to kill myself. it won't work and I know it.
}

JSValueRef
modules::GetPackageScopeConfig(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount,
                                   const JSValueRef *arguments, JSValueRef *exception) {
    CHECK_GE(argumentCount, 1);
    CHECK(JSValueIsString(ctx, arguments[0]));

    Utf8Value resolved(ctx, arguments[0]);
    auto package_json_url_base = ada::parse(resolved.ToStringView());
    if (!package_json_url_base) {
        ThrowInvalidURL(resolved.ToStringView(), std::nullopt);
        return JSValueMakeNull(ctx);
    }
    auto package_json_url =
            ada::parse("./package.json", &package_json_url_base.value());
    if (!package_json_url) {
        ThrowInvalidURL( "./package.json", resolved.ToString());
        return JSValueMakeNull(ctx);
    }

    std::string_view node_modules_package_path = "/node_modules/package.json";
    auto error_context = ErrorContext();
    error_context.is_esm = true;

    // TODO(@anonrig): Rewrite this function and avoid calling URL parser.
    while (true) {
        auto pathname = package_json_url->get_pathname();
        if (pathname.size() >= node_modules_package_path.size() &&
            pathname.compare(pathname.size() - node_modules_package_path.size(),
                             node_modules_package_path.size(),
                             node_modules_package_path) == 0) {
            break;
        }

        auto file_url = FileURLToPath(*package_json_url);
        CHECK(file_url);
        error_context.specifier = resolved.ToString();
        auto package_json = GetPackageJSON(*file_url, &error_context);
        if (package_json != nullptr) {
            return JSValueMakeString(ctx, JSStringCreateWithUTF8CString(package_json->raw_json.c_str())); // todo: check
            //return args.GetReturnValue().Set(package_json->Serialize(realm));
        }

        auto last_href = std::string(package_json_url->get_href());
        auto last_pathname = std::string(package_json_url->get_pathname());
        package_json_url = ada::parse("../package.json", &package_json_url.value());
        if (!package_json_url) {
            ThrowInvalidURL( "../package.json", last_href);
            return JSValueMakeNull(ctx);
        }

        // Terminates at root where ../package.json equals ../../package.json
        // (can't just check "/package.json" for Windows support).
        if (package_json_url->get_pathname() == last_pathname) {
            break;
        }
    }

    auto package_json_url_as_path =
            FileURLToPath(*package_json_url);
    CHECK(package_json_url_as_path);
    return JSValueMakeString(ctx, JSStringCreateWithUTF8CString(package_json_url_as_path->c_str()));
}

const modules::PackageConfig *
modules::GetPackageJSON(std::string_view path, modules::ErrorContext *error_context) {
    auto binding_data = new modules();

    auto cache_entry = binding_data->package_configs_.find(path.data());
    if (cache_entry != binding_data->package_configs_.end()) {
        return &cache_entry->second;
    }

    PackageConfig package_config{};
    package_config.file_path = path;
    // No need to exclude BOM since simdjson will skip it.
    if (utils::ReadFileSync(&package_config.raw_json, path.data()) < 0) {
        return nullptr;
    }

    simdjson::ondemand::document document;
    simdjson::ondemand::object main_object;
    simdjson::error_code error =
            binding_data->json_parser.iterate(package_config.raw_json).get(document);

    const auto throw_invalid_package_config = [error_context, path]() {
        if (error_context == nullptr) {
            fprintf(stderr, "Invalid package config %s.", path.data());
            return 1;
        } else if (error_context->base.has_value()) {
            auto file_url = ada::parse(error_context->base.value());
            CHECK(file_url);
            auto file_path = FileURLToPath(*file_url);
            CHECK(file_path.has_value());
            fprintf(stderr,
                    "Invalid package config %s while importing \"%s\" from %s.",
                    path.data(),
                    error_context->specifier.c_str(),
                    file_path->c_str());
        } else {
            fprintf(stderr, "Invalid package config %s.", path.data());
        }

        return 0;
    };

    if (error || document.get_object().get(main_object)) {
        return reinterpret_cast<const PackageConfig *>(throw_invalid_package_config()); // todo: is this ok?
    }

    simdjson::ondemand::raw_json_string key;
    simdjson::ondemand::value value;
    std::string_view field_value;
    simdjson::ondemand::json_type field_type;

    for (auto field: main_object) {
        // Throw error if getting key or value fails.
        if (field.key().get(key) || field.value().get(value)) {
            return reinterpret_cast<const PackageConfig *>(throw_invalid_package_config());
        }

        // based on coverity using key with == derefs the raw value
        // avoid derefing if its null
        if (key.raw() == nullptr) continue;

        if (key == "name") {
            // Though there is a key "name" with a corresponding value,
            // the value may not be a string or could be an invalid JSON string
            if (value.get_string(package_config.name)) {
                return reinterpret_cast<const PackageConfig *>(throw_invalid_package_config());
            }
        } else if (key == "main") {
            // Omit all non-string values
            USE(value.get_string(package_config.main));
        } else if (key == "exports") {
            if (value.type().get(field_type)) {
                return reinterpret_cast<const PackageConfig *>(throw_invalid_package_config());
            }
            switch (field_type) {
                case simdjson::ondemand::json_type::object:
                case simdjson::ondemand::json_type::array: {
                    if (value.raw_json().get(field_value)) {
                        return reinterpret_cast<const PackageConfig *>(throw_invalid_package_config());
                    }
                    package_config.exports = field_value;
                    break;
                }
                case simdjson::ondemand::json_type::string: {
                    if (value.get_string(package_config.exports)) {
                        return reinterpret_cast<const PackageConfig *>(throw_invalid_package_config());
                    }
                    break;
                }
                default:
                    break;
            }
        } else if (key == "imports") {
            if (value.type().get(field_type)) {
                return reinterpret_cast<const PackageConfig *>(throw_invalid_package_config());
            }
            switch (field_type) {
                case simdjson::ondemand::json_type::array:
                case simdjson::ondemand::json_type::object: {
                    if (value.raw_json().get(field_value)) {
                        return reinterpret_cast<const PackageConfig *>(throw_invalid_package_config());
                    }
                    package_config.imports = field_value;
                    break;
                }
                case simdjson::ondemand::json_type::string: {
                    if (value.get_string(package_config.imports)) {
                        return reinterpret_cast<const PackageConfig *>(throw_invalid_package_config());
                    }
                    break;
                }
                default:
                    break;
            }
        } else if (key == "type") {
            if (value.get_string().get(field_value)) {
                return reinterpret_cast<const PackageConfig *>(throw_invalid_package_config());
            }
            // Only update type if it is "commonjs" or "module"
            // The default value is "none" for backward compatibility.
            if (field_value == "commonjs" || field_value == "module") {
                package_config.type = field_value;
            }
        }
    }
    // package_config could be quite large, so we should move it instead of
    // copying it.
    auto cached = binding_data->package_configs_.insert(
            {std::string(path), std::move(package_config)});

    return &cached.first->second;
}

const modules::PackageConfig *modules::TraverseParent(std::string_view check_path) {
    auto root_separator_index = check_path.find_first_of(kPathSeparator);
    size_t separator_index = 0;

    do {
        separator_index = check_path.find_last_of(kPathSeparator);
        check_path = check_path.substr(0, separator_index);

        // We don't need to try "/"
        if (check_path.empty()) {
            break;
        }

        // Check if the path ends with `/node_modules`
        if (check_path.size() >= kNodeModules.size() &&
            std::equal(check_path.end() - kNodeModules.size(),
                       check_path.end(),
                       kNodeModules.begin())) {
            return nullptr;
        }

        auto package_json = GetPackageJSON(
                std::string(check_path) + kPathSeparator + "package.json",
                nullptr);
        if (package_json != nullptr) {
            return package_json;
        }
    } while (separator_index > root_separator_index);

    return nullptr;
}
