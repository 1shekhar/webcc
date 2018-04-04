#ifndef WEBCC_SOAP_MESSAGE_H_
#define WEBCC_SOAP_MESSAGE_H_

#include <string>
#include "pugixml/pugixml.hpp"
#include "webcc/common.h"

namespace webcc {

// Base class for SOAP request and response.
class SoapMessage {
public:
  // E.g., set as kSoapEnvNamespace.
  void set_soapenv_ns(const Namespace& soapenv_ns) {
    soapenv_ns_ = soapenv_ns;
  }

  void set_service_ns(const Namespace& service_ns) {
    service_ns_ = service_ns;
  }

  const std::string& operation() const {
    return operation_;
  }

  void set_operation(const std::string& operation) {
    operation_ = operation;
  }

  // Convert to SOAP request XML.
  void ToXml(std::string* xml_string);

  // Parse from SOAP request XML.
  bool FromXml(const std::string& xml_string);

protected:
  SoapMessage() {
  }

  // Convert to SOAP body XML.
  virtual void ToXmlBody(pugi::xml_node xbody) = 0;

  // Parse from SOAP body XML.
  virtual bool FromXmlBody(pugi::xml_node xbody) = 0;

protected:
  Namespace soapenv_ns_;  // SOAP envelope namespace.
  Namespace service_ns_;  // Namespace for your web service.

  std::string operation_;
};

}  // namespace webcc

#endif  // WEBCC_SOAP_MESSAGE_H_
