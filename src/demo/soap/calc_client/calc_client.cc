#include "calc_client.h"
#include <iostream>
#include "boost/lexical_cast.hpp"

CalcClient::CalcClient() {
  Init();
}

bool CalcClient::Add(double x, double y, double* result) {
  return Calc("add", "x", "y", x, y, result);
}

bool CalcClient::Subtract(double x, double y, double* result) {
  return Calc("subtract", "x", "y", x, y, result);
}

bool CalcClient::Multiply(double x, double y, double* result) {
  return Calc("multiply", "x", "y", x, y, result);
}

bool CalcClient::Divide(double x, double y, double* result) {
  return Calc("divide", "numerator", "denominator", x, y, result);
}

// Set to 0 to test our own calculator server created with webcc.
#define ACCESS_PARASOFT 0

void CalcClient::Init() {
#if ACCESS_PARASOFT
  url_ = "/glue/calculator";
  host_ = "ws1.parasoft.com";
  port_ = "";  // Default to "80".
  service_ns_ = { "cal", "http://www.parasoft.com/wsdl/calculator/" };
  result_name_ = "Result";
#else
  url_ = "/calculator";
  host_ = "localhost";
  port_ = "8080";
  service_ns_ = { "ser", "http://www.example.com/calculator/" };
  result_name_ = "Result";
#endif
}

bool CalcClient::Calc(const std::string& operation,
                      const std::string& x_name,
                      const std::string& y_name,
                      double x,
                      double y,
                      double* result) {
  // Prepare parameters.
  std::vector<webcc::Parameter> parameters{
    { x_name, x },
    { y_name, y }
  };

  // Make the call.
  std::string result_str;
  webcc::Error error = Call(operation, std::move(parameters), &result_str);

  // Error handling if any.
  if (error != webcc::kNoError) {
    std::cerr << "Error: " << error;
    std::cerr << ", " << webcc::GetErrorMessage(error) << std::endl;
    return false;
  }

  // Convert the result from string to double.
  try {
    *result = boost::lexical_cast<double>(result_str);
  } catch (boost::bad_lexical_cast&) {
    return false;
  }

  return true;
}
