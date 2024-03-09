
/*
 * Copyright (C) 2014 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/// This file is generated by create-http-header-name-table, do not edit.

#ifndef HTTPHeaderNames_h
#define HTTPHeaderNames_h

#include <wtf/Forward.h>

namespace WebCore {

enum class HTTPHeaderName : uint8_t {
    Accept,
    AcceptCharset,
    AcceptEncoding,
    AcceptLanguage,
    AcceptRanges,
    AccessControlAllowCredentials,
    AccessControlAllowHeaders,
    AccessControlAllowMethods,
    AccessControlAllowOrigin,
    AccessControlExposeHeaders,
    AccessControlMaxAge,
    AccessControlRequestHeaders,
    AccessControlRequestMethod,
    Age,
    Authorization,
    CacheControl,
    Connection,
    ContentDisposition,
    ContentEncoding,
    ContentLanguage,
    ContentLength,
    ContentLocation,
    ContentRange,
    ContentSecurityPolicy,
    ContentSecurityPolicyReportOnly,
    ContentType,
    Cookie,
    Cookie2,
    CrossOriginEmbedderPolicy,
    CrossOriginEmbedderPolicyReportOnly,
    CrossOriginOpenerPolicy,
    CrossOriginOpenerPolicyReportOnly,
    CrossOriginResourcePolicy,
    DNT,
    Date,
    DefaultStyle,
    ETag,
    Expect,
    Expires,
    Host,
    IcyMetaInt,
    IcyMetadata,
    IfMatch,
    IfModifiedSince,
    IfNoneMatch,
    IfRange,
    IfUnmodifiedSince,
    KeepAlive,
    LastEventID,
    LastModified,
    Link,
    Location,
    Origin,
    PingFrom,
    PingTo,
    Pragma,
    ProxyAuthorization,
    Purpose,
    Range,
    Referer,
    ReferrerPolicy,
    Refresh,
    ReportTo,
    SecFetchDest,
    SecFetchMode,
    SecWebSocketAccept,
    SecWebSocketExtensions,
    SecWebSocketKey,
    SecWebSocketProtocol,
    SecWebSocketVersion,
    ServerTiming,
    ServiceWorker,
    ServiceWorkerAllowed,
    ServiceWorkerNavigationPreload,
    SetCookie,
    SetCookie2,
    SourceMap,
    StrictTransportSecurity,
    TE,
    TimingAllowOrigin,
    Trailer,
    TransferEncoding,
    Upgrade,
    UpgradeInsecureRequests,
    UserAgent,
    Vary,
    Via,
    XContentTypeOptions,
    XDNSPrefetchControl,
    XFrameOptions,
    XSourceMap,
    XTempTablet,
    XXSSProtection,
};

const unsigned numHTTPHeaderNames = 93;
const size_t minHTTPHeaderNameLength = 2;
const size_t maxHTTPHeaderNameLength = 40;

bool findHTTPHeaderName(StringView, HTTPHeaderName&);
WEBCORE_EXPORT StringView httpHeaderNameString(HTTPHeaderName);

} // namespace WebCore

namespace WTF {

template<> struct EnumTraits<WebCore::HTTPHeaderName> {
    using values = EnumValues<
        WebCore::HTTPHeaderName,
        WebCore::HTTPHeaderName::Accept,
        WebCore::HTTPHeaderName::AcceptCharset,
        WebCore::HTTPHeaderName::AcceptEncoding,
        WebCore::HTTPHeaderName::AcceptLanguage,
        WebCore::HTTPHeaderName::AcceptRanges,
        WebCore::HTTPHeaderName::AccessControlAllowCredentials,
        WebCore::HTTPHeaderName::AccessControlAllowHeaders,
        WebCore::HTTPHeaderName::AccessControlAllowMethods,
        WebCore::HTTPHeaderName::AccessControlAllowOrigin,
        WebCore::HTTPHeaderName::AccessControlExposeHeaders,
        WebCore::HTTPHeaderName::AccessControlMaxAge,
        WebCore::HTTPHeaderName::AccessControlRequestHeaders,
        WebCore::HTTPHeaderName::AccessControlRequestMethod,
        WebCore::HTTPHeaderName::Age,
        WebCore::HTTPHeaderName::Authorization,
        WebCore::HTTPHeaderName::CacheControl,
        WebCore::HTTPHeaderName::Connection,
        WebCore::HTTPHeaderName::ContentDisposition,
        WebCore::HTTPHeaderName::ContentEncoding,
        WebCore::HTTPHeaderName::ContentLanguage,
        WebCore::HTTPHeaderName::ContentLength,
        WebCore::HTTPHeaderName::ContentLocation,
        WebCore::HTTPHeaderName::ContentRange,
        WebCore::HTTPHeaderName::ContentSecurityPolicy,
        WebCore::HTTPHeaderName::ContentSecurityPolicyReportOnly,
        WebCore::HTTPHeaderName::ContentType,
        WebCore::HTTPHeaderName::Cookie,
        WebCore::HTTPHeaderName::Cookie2,
        WebCore::HTTPHeaderName::CrossOriginEmbedderPolicy,
        WebCore::HTTPHeaderName::CrossOriginEmbedderPolicyReportOnly,
        WebCore::HTTPHeaderName::CrossOriginOpenerPolicy,
        WebCore::HTTPHeaderName::CrossOriginOpenerPolicyReportOnly,
        WebCore::HTTPHeaderName::CrossOriginResourcePolicy,
        WebCore::HTTPHeaderName::DNT,
        WebCore::HTTPHeaderName::Date,
        WebCore::HTTPHeaderName::DefaultStyle,
        WebCore::HTTPHeaderName::ETag,
        WebCore::HTTPHeaderName::Expect,
        WebCore::HTTPHeaderName::Expires,
        WebCore::HTTPHeaderName::Host,
        WebCore::HTTPHeaderName::IcyMetaInt,
        WebCore::HTTPHeaderName::IcyMetadata,
        WebCore::HTTPHeaderName::IfMatch,
        WebCore::HTTPHeaderName::IfModifiedSince,
        WebCore::HTTPHeaderName::IfNoneMatch,
        WebCore::HTTPHeaderName::IfRange,
        WebCore::HTTPHeaderName::IfUnmodifiedSince,
        WebCore::HTTPHeaderName::KeepAlive,
        WebCore::HTTPHeaderName::LastEventID,
        WebCore::HTTPHeaderName::LastModified,
        WebCore::HTTPHeaderName::Link,
        WebCore::HTTPHeaderName::Location,
        WebCore::HTTPHeaderName::Origin,
        WebCore::HTTPHeaderName::PingFrom,
        WebCore::HTTPHeaderName::PingTo,
        WebCore::HTTPHeaderName::Pragma,
        WebCore::HTTPHeaderName::ProxyAuthorization,
        WebCore::HTTPHeaderName::Purpose,
        WebCore::HTTPHeaderName::Range,
        WebCore::HTTPHeaderName::Referer,
        WebCore::HTTPHeaderName::ReferrerPolicy,
        WebCore::HTTPHeaderName::Refresh,
        WebCore::HTTPHeaderName::ReportTo,
        WebCore::HTTPHeaderName::SecFetchDest,
        WebCore::HTTPHeaderName::SecFetchMode,
        WebCore::HTTPHeaderName::SecWebSocketAccept,
        WebCore::HTTPHeaderName::SecWebSocketExtensions,
        WebCore::HTTPHeaderName::SecWebSocketKey,
        WebCore::HTTPHeaderName::SecWebSocketProtocol,
        WebCore::HTTPHeaderName::SecWebSocketVersion,
        WebCore::HTTPHeaderName::ServerTiming,
        WebCore::HTTPHeaderName::ServiceWorker,
        WebCore::HTTPHeaderName::ServiceWorkerAllowed,
        WebCore::HTTPHeaderName::ServiceWorkerNavigationPreload,
        WebCore::HTTPHeaderName::SetCookie,
        WebCore::HTTPHeaderName::SetCookie2,
        WebCore::HTTPHeaderName::SourceMap,
        WebCore::HTTPHeaderName::StrictTransportSecurity,
        WebCore::HTTPHeaderName::TE,
        WebCore::HTTPHeaderName::TimingAllowOrigin,
        WebCore::HTTPHeaderName::Trailer,
        WebCore::HTTPHeaderName::TransferEncoding,
        WebCore::HTTPHeaderName::Upgrade,
        WebCore::HTTPHeaderName::UpgradeInsecureRequests,
        WebCore::HTTPHeaderName::UserAgent,
        WebCore::HTTPHeaderName::Vary,
        WebCore::HTTPHeaderName::Via,
        WebCore::HTTPHeaderName::XContentTypeOptions,
        WebCore::HTTPHeaderName::XDNSPrefetchControl,
        WebCore::HTTPHeaderName::XFrameOptions,
        WebCore::HTTPHeaderName::XSourceMap,
        WebCore::HTTPHeaderName::XTempTablet,
        WebCore::HTTPHeaderName::XXSSProtection>;
};

static const NeverDestroyed<String> staticHeaderNames[] = {
    MAKE_STATIC_STRING_IMPL("accept"),
    MAKE_STATIC_STRING_IMPL("accept-charset"),
    MAKE_STATIC_STRING_IMPL("accept-encoding"),
    MAKE_STATIC_STRING_IMPL("accept-language"),
    MAKE_STATIC_STRING_IMPL("accept-ranges"),
    MAKE_STATIC_STRING_IMPL("access-control-allow-credentials"),
    MAKE_STATIC_STRING_IMPL("access-control-allow-headers"),
    MAKE_STATIC_STRING_IMPL("access-control-allow-methods"),
    MAKE_STATIC_STRING_IMPL("access-control-allow-origin"),
    MAKE_STATIC_STRING_IMPL("access-control-expose-headers"),
    MAKE_STATIC_STRING_IMPL("access-control-max-age"),
    MAKE_STATIC_STRING_IMPL("access-control-request-headers"),
    MAKE_STATIC_STRING_IMPL("access-control-request-method"),
    MAKE_STATIC_STRING_IMPL("age"),
    MAKE_STATIC_STRING_IMPL("authorization"),
    MAKE_STATIC_STRING_IMPL("cache-control"),
    MAKE_STATIC_STRING_IMPL("connection"),
    MAKE_STATIC_STRING_IMPL("content-disposition"),
    MAKE_STATIC_STRING_IMPL("content-encoding"),
    MAKE_STATIC_STRING_IMPL("content-language"),
    MAKE_STATIC_STRING_IMPL("content-length"),
    MAKE_STATIC_STRING_IMPL("content-location"),
    MAKE_STATIC_STRING_IMPL("content-range"),
    MAKE_STATIC_STRING_IMPL("content-security-policy"),
    MAKE_STATIC_STRING_IMPL("content-security-policy-report-only"),
    MAKE_STATIC_STRING_IMPL("content-type"),
    MAKE_STATIC_STRING_IMPL("cookie"),
    MAKE_STATIC_STRING_IMPL("cookie2"),
    MAKE_STATIC_STRING_IMPL("cross-origin-embedder-policy"),
    MAKE_STATIC_STRING_IMPL("cross-origin-embedder-policy-report-only"),
    MAKE_STATIC_STRING_IMPL("cross-origin-opener-policy"),
    MAKE_STATIC_STRING_IMPL("cross-origin-opener-policy-report-only"),
    MAKE_STATIC_STRING_IMPL("cross-origin-resource-policy"),
    MAKE_STATIC_STRING_IMPL("dnt"),
    MAKE_STATIC_STRING_IMPL("date"),
    MAKE_STATIC_STRING_IMPL("default-style"),
    MAKE_STATIC_STRING_IMPL("etag"),
    MAKE_STATIC_STRING_IMPL("expect"),
    MAKE_STATIC_STRING_IMPL("expires"),
    MAKE_STATIC_STRING_IMPL("host"),
    MAKE_STATIC_STRING_IMPL("icy-metaint"),
    MAKE_STATIC_STRING_IMPL("icy-metadata"),
    MAKE_STATIC_STRING_IMPL("if-match"),
    MAKE_STATIC_STRING_IMPL("if-modified-since"),
    MAKE_STATIC_STRING_IMPL("if-none-match"),
    MAKE_STATIC_STRING_IMPL("if-range"),
    MAKE_STATIC_STRING_IMPL("if-unmodified-since"),
    MAKE_STATIC_STRING_IMPL("keep-alive"),
    MAKE_STATIC_STRING_IMPL("last-event-id"),
    MAKE_STATIC_STRING_IMPL("last-modified"),
    MAKE_STATIC_STRING_IMPL("link"),
    MAKE_STATIC_STRING_IMPL("location"),
    MAKE_STATIC_STRING_IMPL("origin"),
    MAKE_STATIC_STRING_IMPL("ping-from"),
    MAKE_STATIC_STRING_IMPL("ping-to"),
    MAKE_STATIC_STRING_IMPL("pragma"),
    MAKE_STATIC_STRING_IMPL("proxy-authorization"),
    MAKE_STATIC_STRING_IMPL("purpose"),
    MAKE_STATIC_STRING_IMPL("range"),
    MAKE_STATIC_STRING_IMPL("referer"),
    MAKE_STATIC_STRING_IMPL("referrer-policy"),
    MAKE_STATIC_STRING_IMPL("refresh"),
    MAKE_STATIC_STRING_IMPL("report-to"),
    MAKE_STATIC_STRING_IMPL("sec-fetch-dest"),
    MAKE_STATIC_STRING_IMPL("sec-fetch-mode"),
    MAKE_STATIC_STRING_IMPL("sec-websocket-accept"),
    MAKE_STATIC_STRING_IMPL("sec-websocket-extensions"),
    MAKE_STATIC_STRING_IMPL("sec-websocket-key"),
    MAKE_STATIC_STRING_IMPL("sec-websocket-protocol"),
    MAKE_STATIC_STRING_IMPL("sec-websocket-version"),
    MAKE_STATIC_STRING_IMPL("server-timing"),
    MAKE_STATIC_STRING_IMPL("service-worker"),
    MAKE_STATIC_STRING_IMPL("service-worker-allowed"),
    MAKE_STATIC_STRING_IMPL("service-worker-navigation-preload"),
    MAKE_STATIC_STRING_IMPL("set-cookie"),
    MAKE_STATIC_STRING_IMPL("set-cookie2"),
    MAKE_STATIC_STRING_IMPL("sourcemap"),
    MAKE_STATIC_STRING_IMPL("strict-transport-security"),
    MAKE_STATIC_STRING_IMPL("te"),
    MAKE_STATIC_STRING_IMPL("timing-allow-origin"),
    MAKE_STATIC_STRING_IMPL("trailer"),
    MAKE_STATIC_STRING_IMPL("transfer-encoding"),
    MAKE_STATIC_STRING_IMPL("upgrade"),
    MAKE_STATIC_STRING_IMPL("upgrade-insecure-requests"),
    MAKE_STATIC_STRING_IMPL("user-agent"),
    MAKE_STATIC_STRING_IMPL("vary"),
    MAKE_STATIC_STRING_IMPL("via"),
    MAKE_STATIC_STRING_IMPL("x-content-type-options"),
    MAKE_STATIC_STRING_IMPL("x-dns-prefetch-control"),
    MAKE_STATIC_STRING_IMPL("x-frame-options"),
    MAKE_STATIC_STRING_IMPL("x-sourcemap"),
    MAKE_STATIC_STRING_IMPL("x-temp-tablet"),
    MAKE_STATIC_STRING_IMPL("x-xss-protection"),
};

static const WTF::String& httpHeaderNameStringImpl(WebCore::HTTPHeaderName headerName)
{
    return staticHeaderNames[static_cast<size_t>(headerName)];
}

} // namespace WTF

#endif // HTTPHeaderNames_h
