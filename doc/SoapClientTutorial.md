# SOAP Client Tutorial

Firstly, please install **SoapUI** if you don't have it. We need SoapUI to generate sample requests for each web service operation. The open source version is good enough.

Take the calculator web service provided by ParaSoft as an example. Download the WSDL from http://ws1.parasoft.com/glue/calculator.wsdl, create a SOAP project within SoapUI (remember to check "**Create sample requests for all operations?**"), you will see the sample request for "add" operation as the following:
```xml
<soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:cal="http://www.parasoft.com/wsdl/calculator/">
<soapenv:Header/>
<soapenv:Body>
   <cal:add>
      <cal:x>1</cal:x>
      <cal:y>2</cal:y>
   </cal:add>
</soapenv:Body>
</soapenv:Envelope>
```

In order to call the "add" operation, we have to send a HTTP request with the above SOAP envelope as the content. Let's see how to do this with *webcc*.

Firstly, create a class `CalcClient` which is derived from `webcc::SoapClient`:

```cpp
#include <string>
#include "webcc/soap_client.h"

class CalcClient : public webcc::SoapClient {
public:
  CalcClient() {
    Init();
  }
```

Initialize the URL, host, port, etc. in `Init()`:
```cpp
private:
  void Init() {
    url_ = "/glue/calculator";
    host_ = "ws1.parasoft.com";
    port_ = "";  // Default to "80".
    service_ns_ = { "cal", "http://www.parasoft.com/wsdl/calculator/" };
    result_name_ = "Result";
  }
```
 
Because four calculator operations (*add*, *subtract*, *multiply* and *divide*) all have two parameters, we create a wrapper for `SoapClient::Call()`, name is as `Calc`:
```cpp
bool Calc(const std::string& operation,
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
```

Note that the local parameters are moved (with `std::move()`). This is to avoid expensive copy of long string parameters, e.g., XML strings.

Finally, we implement the four operations simply as the following:
```cpp
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
  return Calc("divide", "numerator", "denominator", x, y, result);
}
```
See? It's not that complicated. Check folder ***demo/calculator_client*** for the full example.
