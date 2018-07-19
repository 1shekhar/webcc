#ifndef WEBCC_SOAP_CLIENT_H_
#define WEBCC_SOAP_CLIENT_H_

#include <string>
#include <vector>

#include "webcc/soap_message.h"
#include "webcc/soap_parameter.h"

namespace webcc {

class SoapClient {
 public:
  SoapClient(const std::string& host, const std::string& port);

  ~SoapClient() = default;

  DELETE_COPY_AND_ASSIGN(SoapClient);

  void set_timeout_seconds(int timeout_seconds) {
    timeout_seconds_ = timeout_seconds;
  }

  void set_url(const std::string& url) { url_ = url; }

  void set_service_ns(const SoapNamespace& service_ns) {
    service_ns_ = service_ns;
  }

  void set_result_name(const std::string& result_name) {
    result_name_ = result_name;
  }

  void set_format_raw(bool format_raw) { format_raw_ = format_raw; }

  void set_indent_str(const std::string& indent_str) {
    indent_str_ = indent_str;
  }

  bool timed_out() const { return timed_out_; }

  Error error() const { return error_; }

  bool Request(const std::string& operation,
               std::vector<SoapParameter>&& parameters,
               std::string* result);

 private:
  std::string host_;
  std::string port_;  // Leave this empty to use default 80.

  // Request URL.
  std::string url_;

  SoapNamespace soapenv_ns_;  // SOAP envelope namespace.
  SoapNamespace service_ns_;  // Namespace for your web service.

  // Response result XML node name.
  // E.g., "Result".
  std::string result_name_;

  // Format request XML without any indentation or line breaks.
  bool format_raw_;

  // Indent string for request XML.
  // Applicable when |format_raw_| is false.
  std::string indent_str_;

  // Timeout in seconds; only effective when > 0.
  int timeout_seconds_;

  // If the error was caused by timeout or not.
  bool timed_out_;

  Error error_;
};

}  // namespace webcc

#endif  // WEBCC_SOAP_CLIENT_H_
