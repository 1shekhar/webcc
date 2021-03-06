#include <iostream>

#include "webcc/logger.h"
#include "webcc/soap_client.h"

// -----------------------------------------------------------------------------

class CalcClient {
 public:
  CalcClient(const std::string& host, const std::string& port)
      : soap_client_(host, port) {
    soap_client_.set_timeout_seconds(5);

    soap_client_.set_url("/calculator");
    soap_client_.set_service_ns({
      "ser", "http://www.example.com/calculator/"
    });
    soap_client_.set_result_name("Result");

    // Customize request XML format.
    soap_client_.set_format_raw(false);
    soap_client_.set_indent_str("  ");
  }

  bool Add(double x, double y, double* result) {
    return Calc("add", "x", "y", x, y, result);
  }

  bool Subtract(double x, double y, double* result) {
    return Calc("subtract", "x", "y", x, y, result);
  }

  bool Multiply(double x, double y, double* result) {
    return Calc("multiply", "x", "y", x, y, result);
  }

  bool Divide(double x, double y, double* result) {
    return Calc("divide", "x", "y", x, y, result);
  }

  // Only for testing purpose.
  bool Unknown(double x, double y, double* result) {
    return Calc("unknown", "x", "y", x, y, result);
  }

 private:
  bool Calc(const std::string& operation,
            const std::string& x_name, const std::string& y_name,
            double x, double y,
            double* result) {
    std::vector<webcc::SoapParameter> parameters{
      { x_name, x },
      { y_name, y }
    };

    std::string result_str;
    if (!soap_client_.Request(operation, std::move(parameters), &result_str)) {
      PrintError();
      return false;
    }

    try {
      *result = std::stod(result_str);
    } catch (const std::exception&) {
      return false;
    }

    return true;
  }

  void PrintError() {
    std::cout << webcc::DescribeError(soap_client_.error());
    if (soap_client_.timed_out()) {
      std::cout << " (timed out)";
    }
    std::cout << std::endl;
  }

  webcc::SoapClient soap_client_;
};

// -----------------------------------------------------------------------------

void Help(const char* argv0) {
  std::cout << "Usage: " << argv0 << " <host> <port>" << std::endl;
  std::cout << "  E.g.," << std::endl;
  std::cout << "    " << argv0 << " localhost 8080" << std::endl;
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    Help(argv[0]);
    return 1;
  }

  WEBCC_LOG_INIT("", webcc::LOG_CONSOLE);

  std::string host = argv[1];
  std::string port = argv[2];

  CalcClient calc(host, port);

  double x = 1.0;
  double y = 2.0;
  double result = 0.0;

  if (calc.Add(x, y, &result)) {
    printf("add: %.1f\n", result);
  }

  if (calc.Subtract(x, y, &result)) {
    printf("subtract: %.1f\n", result);
  }

  if (calc.Multiply(x, y, &result)) {
    printf("multiply: %.1f\n", result);
  }

  if (calc.Divide(x, y, &result)) {
    printf("divide: %.1f\n", result);
  }

  calc.Unknown(x, y, &result);

  return 0;
}
