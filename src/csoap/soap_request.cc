#include "csoap/soap_request.h"
#include "csoap/xml.h"

namespace csoap {

void SoapRequest::AddParameter(const Parameter& parameter) {
  parameters_.push_back(parameter);
}

void SoapRequest::AddParameter(Parameter&& parameter) {
  parameters_.push_back(std::move(parameter));
}

std::string SoapRequest::GetParameter(const std::string& key) const {
  for (const Parameter& p : parameters_) {
    if (p.key() == key) {
      return p.value();
    }
  }
  return "";
}

void SoapRequest::ToXmlBody(pugi::xml_node xbody) {
  pugi::xml_node xop = xml::AddChild(xbody, service_ns_.name, operation_);
  xml::AddNSAttr(xop, service_ns_.name, service_ns_.url);

  for (Parameter& p : parameters_) {
    pugi::xml_node xparam = xml::AddChild(xop, service_ns_.name, p.key());
    xparam.text().set(p.value().c_str());
  }
}

bool SoapRequest::FromXmlBody(pugi::xml_node xbody) {
  pugi::xml_node xoperation = xbody.first_child();
  if (!xoperation) {
    return false;
  }

  xml::SplitName(xoperation, &service_ns_.name, &operation_);
  service_ns_.url = xml::GetNSAttr(xoperation, service_ns_.name);

  pugi::xml_node xparameter = xoperation.first_child();
  while (xparameter) {
    parameters_.push_back({
      xml::GetNameNoPrefix(xparameter),
      std::string(xparameter.text().as_string())
    });

    xparameter = xparameter.next_sibling();
  }

  return true;
}

}  // namespace csoap
