#ifndef CSOAP_SOAP_REQUEST_H_
#define CSOAP_SOAP_REQUEST_H_

#include <vector>
#include "csoap/soap_message.h"

namespace csoap {

// SOAP request.
// Used to compose the SOAP request envelope XML which will be sent as the HTTP
// request body.
class SoapRequest : public SoapMessage {
public:
  void AddParameter(const Parameter& parameter);

  void AddParameter(Parameter&& parameter);

  // Get parameter value by key.
  std::string GetParameter(const std::string& key) const;

protected:
  void ToXmlBody(pugi::xml_node xbody) override;
  bool FromXmlBody(pugi::xml_node xbody) override;

private:
  std::vector<Parameter> parameters_;
};

}  // namespace csoap

#endif  // CSOAP_SOAP_REQUEST_H_
