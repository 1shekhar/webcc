#ifndef CSOAP_COMMON_H_
#define CSOAP_COMMON_H_

// Common definitions.

#include <string>
#include <vector>

namespace csoap {

////////////////////////////////////////////////////////////////////////////////

// Buffer size for sending HTTP request and receiving HTTP response.
// TODO: Configurable for client and server separately.
const std::size_t kBufferSize = 1024;

const std::size_t kInvalidLength = static_cast<std::size_t>(-1);

extern const std::string kContentType;
extern const std::string kContentLength;
extern const std::string kSoapAction;
extern const std::string kHost;

extern const std::string kTextXmlUtf8;
extern const std::string kTextJsonUtf8;

////////////////////////////////////////////////////////////////////////////////

// Error codes.
enum Error {
  kNoError = 0,  // OK

  kHostResolveError,
  kEndpointConnectError,

  kSocketTimeoutError,

  kSocketReadError,
  kSocketWriteError,

  // Invalid start line in the HTTP response.
  kHttpStartLineError,

  // Status is not 200 in the HTTP response.
  kHttpStatusError,

  // Invalid or missing Content-Length in the HTTP response.
  kHttpContentLengthError,

  kXmlError,
};

// Return a descriptive message for the given error code.
const char* GetErrorMessage(Error error);

////////////////////////////////////////////////////////////////////////////////

// HTTP methods (verbs).
// NOTE: Don't use enum to avoid converting back and forth.
const std::string kHttpHead = "HEAD";
const std::string kHttpGet = "GET";
const std::string kHttpPost = "POST";
const std::string kHttpPatch = "PATCH";
const std::string kHttpPut = "PUT";
const std::string kHttpDelete = "DELETE";

//enum class HttpMethod : int {
//  kUnknown,
//  kHead,
//  kGet,
//  kPost,
//  kPatch,
//  kPut,
//  kDelete,
//};

// HTTP response status.
// This is not a full list.
// Full list: https://en.wikipedia.org/wiki/List_of_HTTP_status_codes
// NTOE: Don't use enum class because we want to convert to/from int easily.
struct HttpStatus {
  enum Enum {
    kOK = 200,
    kCreated = 201,
    kAccepted = 202,
    kNoContent = 204,
    kNotModified = 304,
    kBadRequest = 400,
    kNotFound = 404,
    InternalServerError = 500,
    kNotImplemented = 501,
    kServiceUnavailable = 503,
  };
};

////////////////////////////////////////////////////////////////////////////////

// XML namespace name/url pair.
// E.g., { "soap", "http://schemas.xmlsoap.org/soap/envelope/" }
// TODO: Rename (add soap prefix)
class Namespace {
public:
  std::string name;
  std::string url;

  bool IsValid() const {
    return !name.empty() && !url.empty();
  }
};

// CSoap's default namespace for SOAP Envelope.
extern const Namespace kSoapEnvNamespace;

////////////////////////////////////////////////////////////////////////////////

// Parameter in the SOAP request envelope.
// TODO: Rename (add soap prefix)
class Parameter {
public:
  Parameter() = default;
  Parameter(const Parameter& rhs) = default;
  Parameter& operator=(const Parameter& rhs) = default;

  Parameter(const std::string& key, const char* value);
  Parameter(const std::string& key, const std::string& value);
  Parameter(const std::string& key, std::string&& value);
  Parameter(const std::string& key, int value);
  Parameter(const std::string& key, double value);
  Parameter(const std::string& key, bool value);

  // Use "= default" if drop the support of VS 2013.
  Parameter(Parameter&& rhs);

  // Use "= default" if drop the support of VS 2013.
  Parameter& operator=(Parameter&& rhs);

  const std::string& key() const {
    return key_;
  }

  const std::string& value() const {
    return value_;
  }

  const char* c_key() const {
    return key_.c_str();
  }

  const char* c_value() const {
    return value_.c_str();
  }

private:
  std::string key_;
  std::string value_;
};

}  // namespace csoap

#endif  // CSOAP_COMMON_H_
