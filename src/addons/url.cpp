//
// Created by twelvee on 3/10/24.
//

#include "url.h"
#include "../utils.h"

JSValueRef url::DomainToASCII(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount,
                              const JSValueRef *arguments, JSValueRef *exception) {
    CHECK_GE(argumentCount, 1);
    CHECK(JSValueIsString(ctx, arguments[0]));

    std::string input = utils::JSStringToStdString(JSValueToStringCopy(ctx, arguments[0], exception));
    if (input.empty()) {
        return JSValueMakeNull(ctx);
    }

    // It is important to have an initial value that contains a special scheme.
    // Since it will change the implementation of `set_hostname` according to URL
    // spec.
    auto out = ada::parse<ada::url>("ws://x");
    if (!out->set_hostname(input)) {
        return JSValueMakeNull(ctx);
    }
    std::string host = out->get_hostname();
    return JSValueMakeString(ctx, JSStringCreateWithUTF8CString(host.c_str()));
}

JSValueRef url::DomainToUnicode(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount,
                                const JSValueRef *arguments, JSValueRef *exception) {
    CHECK_GE(argumentCount, 1);
    CHECK(JSValueIsString(ctx, arguments[0]));

    std::string input = utils::JSStringToStdString(JSValueToStringCopy(ctx, arguments[0], exception));
    if (input.empty()) {
        return JSValueMakeNull(ctx);
    }

    // It is important to have an initial value that contains a special scheme.
    // Since it will change the implementation of `set_hostname` according to URL
    // spec.
    auto out = ada::parse<ada::url>("ws://x");

    if (!out->set_hostname(input)) {
        return JSValueMakeNull(ctx);
    }
    std::string result = ada::unicode::to_unicode(out->get_hostname());

    return JSValueMakeString(ctx, JSStringCreateWithUTF8CString(result.c_str()));
}

JSValueRef url::Format(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount,
                       const JSValueRef *arguments, JSValueRef *exception) {
    CHECK_GT(argumentCount, 4);
    CHECK(JSValueIsString(ctx, arguments[0]));  // url href
    CHECK(JSValueIsBoolean(ctx, arguments[1]));
    CHECK(JSValueIsBoolean(ctx, arguments[2]));
    CHECK(JSValueIsBoolean(ctx, arguments[3]));
    CHECK(JSValueIsBoolean(ctx, arguments[4]));

    Utf8Value href(ctx, arguments[0]);
    const bool hash = JSValueToBoolean(ctx, arguments[1]);
    const bool unicode = JSValueToBoolean(ctx, arguments[2]);
    const bool search = JSValueToBoolean(ctx, arguments[3]);
    const bool auth = JSValueToBoolean(ctx, arguments[4]);

    // ada::url provides a faster alternative to ada::url_aggregator if we
    // directly want to manipulate the url components without using the respective
    // setters. therefore we are using ada::url here.
    auto out = ada::parse<ada::url>(href.ToStringView());
    CHECK(out);

    if (!hash) {
        out->hash = std::nullopt;
    }

    if (unicode && out->has_hostname()) {
        out->host = ada::idna::to_unicode(out->get_hostname());
    }

    if (!search) {
        out->query = std::nullopt;
    }

    if (!auth) {
        out->username = "";
        out->password = "";
    }

    std::string result = out->get_href();
    return JSValueMakeString(ctx, JSStringCreateWithUTF8CString(result.c_str()));
}

JSValueRef url::CanParse(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount,
                         const JSValueRef *arguments, JSValueRef *exception) {
    CHECK_GE(argumentCount, 1);
    CHECK(JSValueIsString(ctx, arguments[0]));  // input
    // args[1] // base url

    Utf8Value input(ctx, arguments[0]);
    std::string_view input_view = input.ToStringView();

    bool can_parse{};
    if (JSValueIsString(ctx, arguments[1])) {
        Utf8Value base(ctx, arguments[1]);
        std::string_view base_view = base.ToStringView();
        can_parse = ada::can_parse(input_view, &base_view);
    } else {
        can_parse = ada::can_parse(input_view);
    }

    return JSValueMakeBoolean(ctx, can_parse);
}

JSValueRef url::Update(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount,
                       const JSValueRef *arguments, JSValueRef *exception) {
    return nullptr;
}

JSValueRef url::Parse(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount,
                      const JSValueRef *arguments, JSValueRef *exception) {
    CHECK_GE(argumentCount, 1);
    CHECK(JSValueIsString(ctx, arguments[0]));  // input
    // args[1] // base url

    url *binding_data = new url();
    std::optional<std::string> base_{};

    Utf8Value input(ctx, arguments[0]);
    ada::result<ada::url_aggregator> base;
    ada::url_aggregator *base_pointer = nullptr;
    if (JSValueIsString(ctx, arguments[1])) {
        base_ = Utf8Value(ctx, arguments[1]).ToString();
        base = ada::parse<ada::url_aggregator>(*base_);
        if (!base) {
            return JSValueMakeNull(ctx); // todo: throw exception
            //return ThrowInvalidURL(realm->env(), input.ToStringView(), base_);
        }
        base_pointer = &base.value();
    }
    auto out =
            ada::parse<ada::url_aggregator>(input.ToStringView(), base_pointer);

    if (!out) {
        return JSValueMakeNull(ctx); // todo: throw exception
        //return ThrowInvalidURL(realm->env(), input.ToStringView(), base_);
    }

    //binding_data->UpdateComponents(out->get_components(), out->type); // todo: buffers workflow, might be helpful to optimize this code
    std::string s = {out->get_href().begin(), out->get_href().end()};
    return JSValueMakeString(ctx, JSStringCreateWithUTF8CString(
            s.c_str())); // todo: check if it's a static property or a function callback
}

JSValueRef url::GetOrigin(JSContextRef ctx, JSObjectRef function, JSObjectRef object, size_t argumentCount,
                          const JSValueRef *arguments, JSValueRef *exception) {
    CHECK_GE(argumentCount, 1);
    CHECK(JSValueIsString(ctx, arguments[0]));  // input

    Utf8Value input(ctx, arguments[0]);
    std::string_view input_view = input.ToStringView();
    auto out = ada::parse<ada::url_aggregator>(input_view);

    if (!out) {
        fprintf(stderr, "Invalid URL");
        return JSValueMakeNull(ctx);
    }

    std::string origin = out->get_origin();
    return JSValueMakeString(ctx, JSStringCreateWithUTF8CString(origin.c_str()));
}

std::optional<std::string> FileURLToPath(const ada::url_aggregator &file_url) {
    if (file_url.type != ada::scheme::FILE) {
        //todo: THROW_ERR_INVALID_URL_SCHEME
        fprintf(stderr, "invalid scheme.");
        return std::nullopt;
    }

    std::string_view pathname = file_url.get_pathname();
#ifdef _WIN32
    size_t first_percent = std::string::npos;
  size_t pathname_size = pathname.size();
  std::string pathname_escaped_slash;

  for (size_t i = 0; i < pathname_size; i++) {
    if (pathname[i] == '/') {
      pathname_escaped_slash += '\\';
    } else {
      pathname_escaped_slash += pathname[i];
    }

    if (pathname[i] != '%') continue;

    if (first_percent == std::string::npos) {
      first_percent = i;
    }

    // just safe-guard against access the pathname
    // outside the bounds
    if ((i + 2) >= pathname_size) continue;

    char third = pathname[i + 2] | 0x20;

    bool is_slash = pathname[i + 1] == '2' && third == 102;
    bool is_forward_slash = pathname[i + 1] == '5' && third == 99;

    if (!is_slash && !is_forward_slash) continue;

    THROW_ERR_INVALID_FILE_URL_PATH(
        env->isolate(),
        "File URL path must not include encoded \\ or / characters");
    return std::nullopt;
  }

  std::string_view hostname = file_url.get_hostname();
  std::string decoded_pathname = ada::unicode::percent_decode(
      std::string_view(pathname_escaped_slash), first_percent);

  if (hostname.size() > 0) {
    // If hostname is set, then we have a UNC path
    // Pass the hostname through domainToUnicode just in case
    // it is an IDN using punycode encoding. We do not need to worry
    // about percent encoding because the URL parser will have
    // already taken care of that for us. Note that this only
    // causes IDNs with an appropriate `xn--` prefix to be decoded.
    return "\\\\" + ada::unicode::to_unicode(hostname) + decoded_pathname;
  }

  char letter = decoded_pathname[1] | 0x20;
  char sep = decoded_pathname[2];

  // a..z A..Z
  if (letter < 'a' || letter > 'z' || sep != ':') {
    THROW_ERR_INVALID_FILE_URL_PATH(env->isolate(),
                                    "File URL path must be absolute");
    return std::nullopt;
  }

  return decoded_pathname.substr(1);
#else   // _WIN32
    std::string_view hostname = file_url.get_hostname();

    if (hostname.size() > 0) {
        // todo: THROW_ERR_INVALID_FILE_URL_HOST  (node code: ~/src/node_url.cc#L394)
        fprintf(stderr, "File URL host must be \"localhost\" or empty on platform.");
        return std::nullopt;
    }

    size_t first_percent = std::string::npos;
    for (size_t i = 0; (i + 2) < pathname.size(); i++) {
        if (pathname[i] != '%') continue;

        if (first_percent == std::string::npos) {
            first_percent = i;
        }

        if (pathname[i + 1] == '2' && (pathname[i + 2] | 0x20) == 102) {
            fprintf(stderr, "File URL path must not include encoded / characters");
            return std::nullopt;
        }
    }

    return ada::unicode::percent_decode(pathname, first_percent);
#endif  // _WIN32
}