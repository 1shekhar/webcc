#include "webcc/soap_server.h"

#include "webcc/logger.h"
#include "webcc/soap_request.h"
#include "webcc/soap_response.h"

namespace webcc {

////////////////////////////////////////////////////////////////////////////////

bool SoapRequestHandler::RegisterService(SoapServicePtr service,
                                         const std::string& url) {
  assert(service);

  url_service_map_[url] = service;
  return true;
}

void SoapRequestHandler::HandleSession(HttpSessionPtr session) {
  SoapServicePtr service = GetServiceByUrl(session->request().url());
  if (!service) {
    session->SendResponse(HttpStatus::kBadRequest);
    return;
  }

  // Parse the SOAP request XML.
  SoapRequest soap_request;
  if (!soap_request.FromXml(session->request().content())) {
    session->SendResponse(HttpStatus::kBadRequest);
    return;
  }

  SoapResponse soap_response;
  if (!service->Handle(soap_request, &soap_response)) {
    session->SendResponse(HttpStatus::kBadRequest);
    return;
  }

  std::string content;
  soap_response.ToXml(&content);
  session->SetResponseContent(std::move(content), kTextXmlUtf8);
  session->SendResponse(HttpStatus::kOK);
}

SoapServicePtr SoapRequestHandler::GetServiceByUrl(const std::string& url) {
  UrlServiceMap::const_iterator it = url_service_map_.find(url);

  if (it != url_service_map_.end()) {
    LOG_VERB("Service matches the URL: %s", url.c_str());
    return it->second;
  }

  LOG_WARN("No service matches the URL: %s", url.c_str());

  return SoapServicePtr();
}

////////////////////////////////////////////////////////////////////////////////

SoapServer::SoapServer(unsigned short port, std::size_t workers)
    : HttpServer(port, workers)
    , request_handler_(new SoapRequestHandler()) {
}

SoapServer::~SoapServer() {
  delete request_handler_;
}

bool SoapServer::RegisterService(SoapServicePtr service,
                                 const std::string& url) {
  return request_handler_->RegisterService(service, url);
}

}  // namespace webcc
