#ifndef WEBCC_SOAP_SERVICE_H_
#define WEBCC_SOAP_SERVICE_H_

#include <memory>
#include "webcc/common.h"

namespace webcc {

class SoapRequest;
class SoapResponse;

// Base class for your SOAP service.
class SoapService {
public:
  virtual ~SoapService() {
  }

  // Handle SOAP request, output the response.
  virtual bool Handle(const SoapRequest& soap_request,
                      SoapResponse* soap_response) = 0;

protected:
  HttpStatus::Enum http_status_ = HttpStatus::kOK;
};

typedef std::shared_ptr<SoapService> SoapServicePtr;

}  // namespace webcc

#endif  // WEBCC_SOAP_SERVICE_H_
