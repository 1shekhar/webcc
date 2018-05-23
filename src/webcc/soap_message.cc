#include "webcc/soap_message.h"

#include <cassert>

#include "webcc/soap_xml.h"

namespace webcc {

const SoapNamespace kSoapEnvNamespace{
  "soap",
  "http://schemas.xmlsoap.org/soap/envelope/"
};

void SoapMessage::ToXml(std::string* xml_string) {
  assert(soapenv_ns_.IsValid() &&
         service_ns_.IsValid() &&
         !operation_.empty());

  pugi::xml_document xdoc;

  // TODO(Adam):
  // When save with format_default, declaration will be generated
  // automatically but without encoding.
  //   pugi::xml_node xdecl = xdoc.prepend_child(pugi::node_declaration);
  //   xdecl.append_attribute("version").set_value("1.0");

  pugi::xml_node xroot = soap_xml::AddChild(soapenv_ns_.name, "Envelope",
                                            &xdoc);

  soap_xml::AddNSAttr(xroot, soapenv_ns_.name, soapenv_ns_.url);

  pugi::xml_node xbody = soap_xml::AddChild(soapenv_ns_.name, "Body", &xroot);

  ToXmlBody(xbody);

  soap_xml::XmlStrRefWriter writer(xml_string);
  xdoc.save(writer, "\t", pugi::format_default, pugi::encoding_utf8);
}

bool SoapMessage::FromXml(const std::string& xml_string) {
  pugi::xml_document xdoc;
  pugi::xml_parse_result result = xdoc.load_string(xml_string.c_str());

  if (!result) {
    return false;
  }

  pugi::xml_node xroot = xdoc.document_element();

  soapenv_ns_.name = soap_xml::GetPrefix(xroot);
  soapenv_ns_.url = soap_xml::GetNSAttr(xroot, soapenv_ns_.name);

  pugi::xml_node xbody = soap_xml::GetChild(xroot, soapenv_ns_.name, "Body");
  if (xbody) {
    return FromXmlBody(xbody);
  }

  return false;
}

}  // namespace webcc
