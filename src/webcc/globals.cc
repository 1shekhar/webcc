#include "webcc/globals.h"

#include <utility>  // for move()

namespace webcc {

// -----------------------------------------------------------------------------

// NOTE:
// Field names are case-insensitive.
// See: https://stackoverflow.com/a/5259004
const std::string kContentType = "Content-Type";
const std::string kContentLength = "Content-Length";
const std::string kSoapAction = "SOAPAction";
const std::string kHost = "Host";

// According to www.w3.org when placing SOAP messages in HTTP bodies, the HTTP
// Content-type header must be chosen as "application/soap+xml" [RFC 3902].
// But in practice, many web servers cannot understand it.
// See: https://www.w3.org/TR/2007/REC-soap12-part0-20070427/#L26854
const std::string kTextXmlUtf8 = "text/xml; charset=utf-8";

const std::string kTextJsonUtf8 = "text/json; charset=utf-8";

const std::string kHttpHead = "HEAD";
const std::string kHttpGet = "GET";
const std::string kHttpPost = "POST";
const std::string kHttpPatch = "PATCH";
const std::string kHttpPut = "PUT";
const std::string kHttpDelete = "DELETE";

// -----------------------------------------------------------------------------

const char* DescribeError(Error error) {
  switch (error) {
    case kHostResolveError:
      return "Host resolve error";
    case kEndpointConnectError:
      return "Endpoint connect error";
    case kSocketReadError:
      return "Socket read error";
    case kSocketWriteError:
      return "Socket write error";
    case kHttpError:
      return "HTTP error";
    case kXmlError:
      return "XML error";
    default:
      return "";
  }
}

// -----------------------------------------------------------------------------

Parameter::Parameter(const std::string& key, const char* value)
    : key_(key), value_(value) {
}

Parameter::Parameter(const std::string& key, const std::string& value)
    : key_(key), value_(value) {
}

Parameter::Parameter(const std::string& key, std::string&& value)
    : key_(key), value_(std::move(value)) {
}

Parameter::Parameter(const std::string& key, int value)
    : key_(key) {
  value_ = std::to_string(value);
}

Parameter::Parameter(const std::string& key, double value)
    : key_(key) {
  value_ = std::to_string(value);
}

Parameter::Parameter(const std::string& key, bool value)
    : key_(key) {
  value_ = value ? "true" : "false";
}

Parameter::Parameter(Parameter&& rhs)
    : key_(std::move(rhs.key_)),
      value_(std::move(rhs.value_)) {
}

Parameter& Parameter::operator=(Parameter&& rhs) {
  if (this != &rhs) {
    key_ = std::move(rhs.key_);
    value_ = std::move(rhs.value_);
  }
  return *this;
}

std::string Parameter::ToString() const {
  return key_ + "=" + value_;
}

}  // namespace webcc
