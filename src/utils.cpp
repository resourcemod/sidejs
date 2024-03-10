//
// Created by twelvee on 3/9/24.
//

#include "utils.h"

JSStringRef utils::JSRefToStringRef(JSContextRef ctx, JSValueRef argument, JSValueRef *exception) {
    return JSValueToStringCopy(ctx, argument, exception);
}

std::string utils::JSStringToStdString(JSStringRef jsString) {
    size_t maxBufferSize = JSStringGetMaximumUTF8CStringSize(jsString);
    char *utf8Buffer = new char[maxBufferSize];
    size_t bytesWritten = JSStringGetUTF8CString(jsString, utf8Buffer, maxBufferSize);
    std::string utf_string = std::string(utf8Buffer, bytesWritten - 1);
    delete[] utf8Buffer;
    return utf_string;
}

JSValueRef utils::JSCreateException(JSContextRef ctx, const std::string &) {
    auto exceptionString = JSStringCreateWithUTF8CString("Error: undefined signature. 2 parameters expected.");
    auto ex = JSValueMakeString(ctx, exceptionString);
    return ex;
}

int utils::ReadFileSync(std::string *result, const char *path) {
    uv_fs_t req;
    auto defer_req_cleanup = OnScopeLeave([&req]() {
        uv_fs_req_cleanup(&req);
    });

    uv_file file = uv_fs_open(nullptr, &req, path, O_RDONLY, 0, nullptr);
    if (req.result < 0) {
        // req will be cleaned up by scope leave.
        return req.result;
    }
    uv_fs_req_cleanup(&req);

    auto defer_close = OnScopeLeave([file]() {
        uv_fs_t close_req;
        CHECK_EQ(0, uv_fs_close(nullptr, &close_req, file, nullptr));
        uv_fs_req_cleanup(&close_req);
    });

    *result = std::string("");
    char buffer[4096];
    uv_buf_t buf = uv_buf_init(buffer, sizeof(buffer));

    while (true) {
        const int r =
                uv_fs_read(nullptr, &req, file, &buf, 1, result->length(), nullptr);
        if (req.result < 0) {
            // req will be cleaned up by scope leave.
            return req.result;
        }
        uv_fs_req_cleanup(&req);
        if (r <= 0) {
            break;
        }
        result->append(buf.base, r);
    }
    return 0;
}

std::vector<char> utils::ReadFileSync(FILE *fp) {
    CHECK_EQ(ftell(fp), 0);
    int err = fseek(fp, 0, SEEK_END);
    CHECK_EQ(err, 0);
    size_t size = ftell(fp);
    CHECK_NE(size, static_cast<size_t>(-1L));
    err = fseek(fp, 0, SEEK_SET);
    CHECK_EQ(err, 0);

    std::vector<char> contents(size);
    size_t num_read = fread(contents.data(), size, 1, fp);
    CHECK_EQ(num_read, 1);
    return contents;
}

Utf8Value::Utf8Value(JSContextRef ctx, JSValueRef value) {
    if (!JSValueIsString(ctx, value)) {
        return;
    }

    std::string string;
    size_t storage;
    if (!StringBytes::StorageSize(isolate, string, UTF8).To(&storage)) return;
    storage += 1;
    target->AllocateSufficientStorage(storage);
    const int flags =
            String::NO_NULL_TERMINATION | String::REPLACE_INVALID_UTF8;
    const int length =
            string->WriteUtf8(isolate, target->out(), storage, nullptr, flags);
    target->SetLengthAndZeroTerminate(length);
}
