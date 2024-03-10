//
// Created by twelvee on 3/9/24.
//

#ifndef SIDEJS_UTILS_H
#define SIDEJS_UTILS_H

#include <JavaScriptCore/JavaScript.h>
#include <string>
#include <vector>
#include <uv.h>
#include <cstring>

#define JS_CREATE_EXCEPTION(exception, ctx, str)                                                                    \
        auto exceptionString = JSStringCreateWithUTF8CString("Error: undefined signature. 2 parameters expected."); \
        auto ex = JSValueMakeString(ctx, exceptionString);                                                          \
        exception = &ex;

#ifdef __GNUC__
#define MUST_USE_RESULT __attribute__((warn_unused_result))
#else
#define MUST_USE_RESULT
#endif

// Maybe remove kPathSeparator when cpp17 is ready
#ifdef _WIN32
constexpr char kPathSeparator = '\\';
/* MAX_PATH is in characters, not bytes. Make sure we have enough headroom. */
constexpr std::string_view kNodeModules = "/node_modules";
#define PATH_MAX_BYTES (MAX_PATH * 4)
#else
constexpr char kPathSeparator = '/';
constexpr std::string_view kNodeModules = "\\node_modules";
#define PATH_MAX_BYTES (PATH_MAX)
#endif

// These should be used in our code as opposed to the native
// versions as they abstract out some platform and or
// compiler version specific functionality
// malloc(0) and realloc(ptr, 0) have implementation-defined behavior in
// that the standard allows them to either return a unique pointer or a
// nullptr for zero-sized allocation requests.  Normalize by always using
// a nullptr.
template <typename T>
inline T* UncheckedRealloc(T* pointer, size_t n);
template <typename T>
inline T* UncheckedMalloc(size_t n);
template <typename T>
inline T* UncheckedCalloc(size_t n);

// Same things, but aborts immediately instead of returning nullptr when
// no memory is available.
template <typename T>
inline T* Realloc(T* pointer, size_t n);
template <typename T>
inline T* Malloc(size_t n);
template <typename T>
inline T* Calloc(size_t n);

inline char* Malloc(size_t n);
inline char* Calloc(size_t n);
inline char* UncheckedMalloc(size_t n);
inline char* UncheckedCalloc(size_t n);

template <typename T>
inline T MultiplyWithOverflowCheck(T a, T b);

namespace per_process {
// Tells whether the per-process V8::Initialize() is called and
// if it is safe to call v8::Isolate::TryGetCurrent().
    extern bool v8_initialized;
}  // namespace per_process

// Used by the allocation functions when allocation fails.
// Thin wrapper around v8::Isolate::LowMemoryNotification() that checks
// whether V8 is initialized.
void LowMemoryNotification();

// The reason that Assert() takes a struct argument instead of individual
// const char*s is to ease instruction cache pressure in calls from CHECK.
struct AssertionInfo {
    const char* file_line;  // filename:line
    const char* message;
    const char* function;
};

void Assert(const AssertionInfo& info);
void DumpNativeBacktrace(FILE* fp);
void DumpJavaScriptBacktrace(FILE* fp);

// Windows 8+ does not like abort() in Release mode
#ifdef _WIN32
#define ABORT_NO_BACKTRACE() _exit(static_cast<int>(node::ExitCode::kAbort))
#else
#define ABORT_NO_BACKTRACE() abort()
#endif

// Caller of this macro must not be marked as [[noreturn]]. Printing of
// backtraces may not work correctly in [[noreturn]] functions because
// when generating code for them the compiler can choose not to
// maintain the frame pointers or link registers that are necessary for
// correct backtracing.
// `ABORT` must be a macro and not a [[noreturn]] function to make sure the
// backtrace is correct.
#define ABORT()                                                                \
  do {                                                                         \
    node::DumpNativeBacktrace(stderr);                                         \
    node::DumpJavaScriptBacktrace(stderr);                                     \
    fflush(stderr);                                                            \
    ABORT_NO_BACKTRACE();                                                      \
  } while (0)

#define ERROR_AND_ABORT(expr)                                                  \
  do {                                                                         \
    /* Make sure that this struct does not end up in inline code, but      */  \
    /* rather in a read-only data section when modifying this code.        */  \
    static const AssertionInfo args = {                                  \
        __FILE__ ":" STRINGIFY(__LINE__), #expr, PRETTY_FUNCTION_NAME};        \
    /* `node::Assert` doesn't return. Add an [[noreturn]] abort() here to  */  \
    /* make the compiler happy about no return value in the caller         */  \
    /* function when calling ERROR_AND_ABORT.                              */  \
    ABORT_NO_BACKTRACE();                                                      \
  } while (0)

#ifdef __GNUC__
#define LIKELY(expr) __builtin_expect(!!(expr), 1)
#define UNLIKELY(expr) __builtin_expect(!!(expr), 0)
#define PRETTY_FUNCTION_NAME __PRETTY_FUNCTION__
#else
#define LIKELY(expr) expr
#define UNLIKELY(expr) expr
#define PRETTY_FUNCTION_NAME ""
#endif

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)

#define CHECK(expr)                                                           \
  do {                                                                        \
    if (UNLIKELY(!(expr))) {                                                  \
      ERROR_AND_ABORT(expr);                                                  \
    }                                                                         \
  } while (0)

#define CHECK_EQ(a, b) CHECK((a) == (b))
#define CHECK_GE(a, b) CHECK((a) >= (b))
#define CHECK_GT(a, b) CHECK((a) > (b))
#define CHECK_LE(a, b) CHECK((a) <= (b))
#define CHECK_LT(a, b) CHECK((a) < (b))
#define CHECK_NE(a, b) CHECK((a) != (b))
#define CHECK_NULL(val) CHECK((val) == nullptr)
#define CHECK_NOT_NULL(val) CHECK((val) != nullptr)
#define CHECK_IMPLIES(a, b) CHECK(!(a) || (b))

// Swaps bytes in place. nbytes is the number of bytes to swap and must be a
// multiple of the word size (checked by function).
inline void SwapBytes16(char* data, size_t nbytes);
inline void SwapBytes32(char* data, size_t nbytes);
inline void SwapBytes64(char* data, size_t nbytes);

// tolower() is locale-sensitive.  Use ToLower() instead.
inline char ToLower(char c);
inline std::string ToLower(const std::string& in);

// toupper() is locale-sensitive.  Use ToUpper() instead.
inline char ToUpper(char c);
inline std::string ToUpper(const std::string& in);

// strcasecmp() is locale-sensitive.  Use StringEqualNoCase() instead.
inline bool StringEqualNoCase(const char* a, const char* b);

// strncasecmp() is locale-sensitive.  Use StringEqualNoCaseN() instead.
inline bool StringEqualNoCaseN(const char* a, const char* b, size_t length);

template <typename T, size_t N>
constexpr size_t arraysize(const T (&)[N]) {
    return N;
}

template <typename T, size_t N>
constexpr size_t strsize(const T (&)[N]) {
    return N - 1;
}

// Allocates an array of member type T. For up to kStackStorageSize items,
// the stack is used, otherwise malloc().
template <typename T, size_t kStackStorageSize = 1024>
class MaybeStackBuffer {
public:
    const T* out() const {
        return buf_;
    }

    T* out() {
        return buf_;
    }

    // operator* for compatibility with `v8::String::(Utf8)Value`
    T* operator*() {
        return buf_;
    }

    const T* operator*() const {
        return buf_;
    }

    T& operator[](size_t index) {
        CHECK_LT(index, length());
        return buf_[index];
    }

    const T& operator[](size_t index) const {
        CHECK_LT(index, length());
        return buf_[index];
    }

    size_t length() const {
        return length_;
    }

    // Current maximum capacity of the buffer with which SetLength() can be used
    // without first calling AllocateSufficientStorage().
    size_t capacity() const {
        return capacity_;
    }

    // Make sure enough space for `storage` entries is available.
    // This method can be called multiple times throughout the lifetime of the
    // buffer, but once this has been called Invalidate() cannot be used.
    // Content of the buffer in the range [0, length()) is preserved.
    void AllocateSufficientStorage(size_t storage);

    void SetLength(size_t length) {
        // capacity() returns how much memory is actually available.
        CHECK_LE(length, capacity());
        length_ = length;
    }

    void SetLengthAndZeroTerminate(size_t length) {
        // capacity() returns how much memory is actually available.
        CHECK_LE(length + 1, capacity());
        SetLength(length);

        // T() is 0 for integer types, nullptr for pointers, etc.
        buf_[length] = T();
    }

    // Make dereferencing this object return nullptr.
    // This method can be called multiple times throughout the lifetime of the
    // buffer, but once this has been called AllocateSufficientStorage() cannot
    // be used.
    void Invalidate() {
        CHECK(!IsAllocated());
        capacity_ = 0;
        length_ = 0;
        buf_ = nullptr;
    }

    // If the buffer is stored in the heap rather than on the stack.
    bool IsAllocated() const {
        return !IsInvalidated() && buf_ != buf_st_;
    }

    // If Invalidate() has been called.
    bool IsInvalidated() const {
        return buf_ == nullptr;
    }

    // Release ownership of the malloc'd buffer.
    // Note: This does not free the buffer.
    void Release() {
        CHECK(IsAllocated());
        buf_ = buf_st_;
        length_ = 0;
        capacity_ = arraysize(buf_st_);
    }

    MaybeStackBuffer()
            : length_(0), capacity_(arraysize(buf_st_)), buf_(buf_st_) {
        // Default to a zero-length, null-terminated buffer.
        buf_[0] = T();
    }

    explicit MaybeStackBuffer(size_t storage) : MaybeStackBuffer() {
        AllocateSufficientStorage(storage);
    }

    ~MaybeStackBuffer() {
        if (IsAllocated())
            free(buf_);
    }

    inline std::basic_string<T> ToString() const { return {out(), length()}; }
    inline std::basic_string_view<T> ToStringView() const {
        return {out(), length()};
    }

private:
    size_t length_;
    // capacity of the malloc'ed buf_
    size_t capacity_;
    T* buf_;
    T buf_st_[kStackStorageSize];
};

// Provides access to an ArrayBufferView's storage, either the original,
// or for small data, a copy of it. This object's lifetime is bound to the
// original ArrayBufferView's lifetime.
template <typename T, size_t kStackStorageSize = 64>
class ArrayBufferViewContents {
public:
    ArrayBufferViewContents() = default;

    ArrayBufferViewContents(const ArrayBufferViewContents&) = delete;
    void operator=(const ArrayBufferViewContents&) = delete;

    explicit inline ArrayBufferViewContents(JSValueRef value);
    explicit inline ArrayBufferViewContents(JSObjectRef value);
    inline void Read(JSValueRef abv);
    inline void ReadValue(JSValueRef buf);

    inline bool WasDetached() const { return was_detached_; }
    inline const T* data() const { return data_; }
    inline size_t length() const { return length_; }

private:
    // Declaring operator new and delete as deleted is not spec compliant.
    // Therefore, declare them private instead to disable dynamic alloc.
    void* operator new(size_t size);
    void* operator new[](size_t size);
    void operator delete(void*, size_t);
    void operator delete[](void*, size_t);

    T stack_storage_[kStackStorageSize];
    T* data_ = nullptr;
    size_t length_ = 0;
    bool was_detached_ = false;
};

#define SPREAD_BUFFER_ARG(val, name)                                           \
  CHECK((val)->IsArrayBufferView());                                           \
  v8::Local<v8::ArrayBufferView> name = (val).As<v8::ArrayBufferView>();       \
  const size_t name##_offset = name->ByteOffset();                             \
  const size_t name##_length = name->ByteLength();                             \
  char* const name##_data =                                                    \
      static_cast<char*>(name->Buffer()->Data()) + name##_offset;              \
  if (name##_length > 0) CHECK_NE(name##_data, nullptr);

// Use this when a variable or parameter is unused in order to explicitly
// silence a compiler warning about that.
template <typename T> inline void USE(T&&) {}

template <typename Fn>
struct OnScopeLeaveImpl {
    Fn fn_;
    bool active_;

    explicit OnScopeLeaveImpl(Fn&& fn) : fn_(std::move(fn)), active_(true) {}
    ~OnScopeLeaveImpl() { if (active_) fn_(); }

    OnScopeLeaveImpl(const OnScopeLeaveImpl& other) = delete;
    OnScopeLeaveImpl& operator=(const OnScopeLeaveImpl& other) = delete;
    OnScopeLeaveImpl(OnScopeLeaveImpl&& other)
            : fn_(std::move(other.fn_)), active_(other.active_) {
        other.active_ = false;
    }
};

// Run a function when exiting the current scope. Used like this:
// auto on_scope_leave = OnScopeLeave([&] {
//   // ... run some code ...
// });
template <typename Fn>
inline MUST_USE_RESULT OnScopeLeaveImpl<Fn> OnScopeLeave(Fn&& fn) {
return OnScopeLeaveImpl<Fn>{std::move(fn)};
}

// Simple RAII wrapper for contiguous data that uses malloc()/free().
template <typename T>
struct MallocedBuffer {
    T* data;
    size_t size;

    T* release() {
        T* ret = data;
        data = nullptr;
        return ret;
    }

    void Truncate(size_t new_size) {
        CHECK_LE(new_size, size);
        size = new_size;
    }

    void Realloc(size_t new_size) {
        Truncate(new_size);
        data = UncheckedRealloc(data, new_size);
    }

    bool is_empty() const { return data == nullptr; }

    MallocedBuffer() : data(nullptr), size(0) {}
    explicit MallocedBuffer(size_t size) : data(Malloc<T>(size)), size(size) {}
    MallocedBuffer(T* data, size_t size) : data(data), size(size) {}
    MallocedBuffer(MallocedBuffer&& other) : data(other.data), size(other.size) {
        other.data = nullptr;
    }
    MallocedBuffer& operator=(MallocedBuffer&& other) {
        this->~MallocedBuffer();
        return *new(this) MallocedBuffer(std::move(other));
    }
    ~MallocedBuffer() {
        free(data);
    }
    MallocedBuffer(const MallocedBuffer&) = delete;
    MallocedBuffer& operator=(const MallocedBuffer&) = delete;
};

template <typename T>
class NonCopyableMaybe {
public:
    NonCopyableMaybe() : empty_(true) {}
    explicit NonCopyableMaybe(T&& value)
            : empty_(false),
              value_(std::move(value)) {}

    bool IsEmpty() const {
        return empty_;
    }

    const T* get() const {
        return empty_ ? nullptr : &value_;
    }

    const T* operator->() const {
        CHECK(!empty_);
        return &value_;
    }

    T&& Release() {
        CHECK_EQ(empty_, false);
        empty_ = true;
        return std::move(value_);
    }

private:
    bool empty_;
    T value_;
};

class Utf8Value : public MaybeStackBuffer<char> {
public:
    explicit Utf8Value(JSContextRef ctx, JSValueRef value);

    inline std::string ToString() const { return std::string(out(), length()); }
    inline std::string_view ToStringView() const {
        return std::string_view(out(), length());
    }

    inline bool operator==(const char* a) const { return strcmp(out(), a) == 0; }
    inline bool operator!=(const char* a) const { return !(*this == a); }
};

class utils {
public:
    static std::string JSStringToStdString(JSStringRef jsString);

    static JSStringRef JSRefToStringRef(JSContextRef ctx, JSValueRef argument, JSValueRef *exception);

    static JSValueRef JSCreateException(JSContextRef ctx, const std::string &);

    static int ReadFileSync(std::string* result, const char* path);

    static std::vector<char> ReadFileSync(FILE* fp);
};


#endif //SIDEJS_UTILS_H
