#ifndef WEBCC_HTTP_SESSION_H_
#define WEBCC_HTTP_SESSION_H_

#include <array>
#include <memory>

#include "boost/asio/ip/tcp.hpp"  // for ip::tcp::socket
#include "boost/asio/deadline_timer.hpp"

#include "webcc/common.h"
#include "webcc/http_request.h"
#include "webcc/http_request_parser.h"
#include "webcc/http_response.h"

namespace webcc {

class HttpRequestHandler;

class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
  HttpSession(const HttpSession&) = delete;
  HttpSession& operator=(const HttpSession&) = delete;

  HttpSession(boost::asio::ip::tcp::socket socket,
              HttpRequestHandler* handler);

  ~HttpSession();

  const HttpRequest& request() const {
    return request_;
  }

  // Start the session with an optional timeout.
  void Start(long timeout_seconds = 0);

  void Stop();

  void SetResponseStatus(int status) {
    response_.set_status(status);
  }

  void SetResponseContent(const std::string& content_type,
                          std::size_t content_length,
                          std::string&& content);

  // Write response back to the client.
  void SendResponse();

private:
  void DoRead();

  void DoWrite();

  void HandleRead(boost::system::error_code ec, std::size_t length);
  void HandleWrite(boost::system::error_code ec, std::size_t length);

  void HandleTimer(boost::system::error_code ec);

  void CancelTimer();

private:
  // Socket for the connection.
  boost::asio::ip::tcp::socket socket_;

  // Timeout timer (optional).
  std::unique_ptr<boost::asio::deadline_timer> timer_;

  // The handler used to process the incoming request.
  HttpRequestHandler* request_handler_;

  // Buffer for incoming data.
  std::array<char, kBufferSize> buffer_;

  // The incoming request.
  HttpRequest request_;

  // The parser for the incoming request.
  HttpRequestParser request_parser_;

  // The response to be sent back to the client.
  HttpResponse response_;
};

typedef std::shared_ptr<HttpSession> HttpSessionPtr;

}  // namespace webcc

#endif  // WEBCC_HTTP_SESSION_H_
