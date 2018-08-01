#include "webcc/globals.h"

#include <utility>  // for move()

namespace webcc {

// -----------------------------------------------------------------------------

// NOTE: Field names are case-insensitive.
//   See https://stackoverflow.com/a/5259004 for more details.
const std::string kHost = "Host";
const std::string kContentType = "Content-Type";
const std::string kContentLength = "Content-Length";

#ifdef WEBCC_ENABLE_SOAP
const std::string kSoapAction = "SOAPAction";
#endif  // WEBCC_ENABLE_SOAP

const std::string kAppJsonUtf8 = "application/json; charset=utf-8";

#ifdef WEBCC_ENABLE_SOAP
// According to www.w3.org when placing SOAP messages in HTTP bodies, the HTTP
// Content-type header must be chosen as "application/soap+xml" [RFC 3902].
// But in practice, many web servers cannot understand it.
// See: https://www.w3.org/TR/2007/REC-soap12-part0-20070427/#L26854
const std::string kTextXmlUtf8 = "text/xml; charset=utf-8";
#endif  // WEBCC_ENABLE_SOAP

const std::string kHttpPort   = "80";
const std::string kHttpsPort  = "443";

const std::string kHttpHead   = "HEAD";
const std::string kHttpGet    = "GET";
const std::string kHttpPost   = "POST";
const std::string kHttpPatch  = "PATCH";
const std::string kHttpPut    = "PUT";
const std::string kHttpDelete = "DELETE";

// -----------------------------------------------------------------------------

const char* DescribeError(Error error) {
  switch (error) {
    case kHostResolveError:
      return "Host resolve error";
    case kEndpointConnectError:
      return "Endpoint connect error";
    case kHandshakeError:
      return "Handshake error";
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

}  // namespace webcc
