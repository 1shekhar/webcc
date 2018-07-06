#ifndef WEBCC_SOAP_RESPONSE_H_
#define WEBCC_SOAP_RESPONSE_H_

#include <string>
#include <utility>  // for move()

#include "webcc/soap_message.h"

namespace webcc {

// SOAP response.
class SoapResponse : public SoapMessage {
 public:
  // Could be "Price" for an operation/method like "GetXyzPrice".
  // Really depend on the service.
  // Most services use a general name "Result".
  void set_result_name(const std::string& result_name) {
    result_name_ = result_name;
  }

  void set_result(const std::string& result) {
    result_ = result;
  }

  void set_result(std::string&& result) {
    result_ = std::move(result);
  }

  std::string result_moved() {
    return std::move(result_);
  }

 protected:
  void ToXmlBody(pugi::xml_node xbody) override;

  bool FromXmlBody(pugi::xml_node xbody) override;

 private:
  // NOTE:
  // Multiple results might be necessary. But for most cases, single result
  // should be enough, because an API normally returns only one value.

  // Result XML node name.
  // Used to parse the response XML from client side.
  std::string result_name_;

  // Result value.
  std::string result_;
};

}  // namespace webcc

#endif  // WEBCC_SOAP_RESPONSE_H_
