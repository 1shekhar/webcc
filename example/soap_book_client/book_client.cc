#include "example/soap_book_client/book_client.h"

#include <iostream>

#include "webcc/logger.h"

#include "example/common/book_xml.h"

static void PrintSeparateLine() {
  std::cout << "--------------------------------";
  std::cout << "--------------------------------";
  std::cout << std::endl;
}

BookClient::BookClient(const std::string& host, const std::string& port)
    : soap_client_(host, port), code_(0) {
  soap_client_.set_url("/book");
  soap_client_.set_service_ns({ "ser", "http://www.example.com/book/" });
  soap_client_.set_result_name("Result");

  // Customize response XML format.
  soap_client_.set_format_raw(false);
  soap_client_.set_indent_str("  ");
}

bool BookClient::CreateBook(const std::string& title, double price,
                            std::string* id) {
  PrintSeparateLine();
  std::cout << "CreateBook: " << title << ", " << price << std::endl;

  webcc::SoapParameter parameter{
    "book",
    BookToXmlString({ "", title, price }),
    true,  // as_cdata
  };
  std::string result_xml;
  if (!Call1("CreateBook", std::move(parameter), &result_xml)) {
    return false;
  }

  auto callback = [id](pugi::xml_node xnode) {
    *id = xnode.child("book").child("id").text().as_string();
    return !id->empty();
  };
  return ParseResultXml(result_xml, callback);
}

bool BookClient::GetBook(const std::string& id, Book* book) {
  PrintSeparateLine();
  std::cout << "GetBook: " << id << std::endl;

  std::string result_xml;
  if (!Call1("GetBook", { "id", id }, &result_xml)) {
    return false;
  }

  auto callback = [book](pugi::xml_node xnode) {
    return XmlToBook(xnode.child("book"), book);
  };
  return ParseResultXml(result_xml, callback);
}

bool BookClient::ListBooks(std::list<Book>* books) {
  PrintSeparateLine();
  std::cout << "ListBooks" << std::endl;

  std::string result_xml;
  if (!Call0("ListBooks", &result_xml)) {
    return false;
  }

  auto callback = [books](pugi::xml_node xnode) {
    return XmlToBookList(xnode.child("books"), books);
  };
  return ParseResultXml(result_xml, callback);
}

bool BookClient::DeleteBook(const std::string& id) {
  PrintSeparateLine();
  std::cout << "DeleteBook: " << id << std::endl;

  std::string result_xml;
  if (!Call1("DeleteBook", { "id", id }, &result_xml)) {
    return false;
  }

  return ParseResultXml(result_xml, {});
}

bool BookClient::Call0(const std::string& operation, std::string* result_str) {
  return Call(operation, {}, result_str);
}


bool BookClient::Call1(const std::string& operation,
                       webcc::SoapParameter&& parameter,
                       std::string* result_str) {
  std::vector<webcc::SoapParameter> parameters{
    { std::move(parameter) }
  };
  return Call(operation, std::move(parameters), result_str);
}

bool BookClient::Call(const std::string& operation,
                      std::vector<webcc::SoapParameter>&& parameters,
                      std::string* result_str) {
  if (!soap_client_.Request(operation, std::move(parameters), result_str)) {
    PrintError();
    return false;
  }
  return true;
}

void BookClient::PrintError() {
  std::cout << webcc::DescribeError(soap_client_.error());
  if (soap_client_.timed_out()) {
    std::cout << " (timed out)";
  }
  std::cout << std::endl;
}

bool BookClient::ParseResultXml(const std::string& result_xml,
                                std::function<bool(pugi::xml_node)> callback) {
  pugi::xml_document xdoc;
  if (!xdoc.load_string(result_xml.c_str())) {
    return false;
  }

  pugi::xml_node xwebcc = xdoc.document_element();

  pugi::xml_node xstatus = xwebcc.child("status");
  code_ = xstatus.attribute("code").as_int();
  message_ = xstatus.attribute("message").as_string();

  if (callback) {
    return callback(xwebcc);
  }

  return true;
}
