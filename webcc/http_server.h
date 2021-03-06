#ifndef WEBCC_HTTP_SERVER_H_
#define WEBCC_HTTP_SERVER_H_

#include <string>

#include "boost/asio/io_context.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/asio/signal_set.hpp"
#include "boost/scoped_ptr.hpp"

#include "webcc/globals.h"
#include "webcc/http_connection.h"

namespace webcc {

class HttpRequestHandler;

// HTTP server accepts TCP connections from TCP clients.
// NOTE: Only support IPv4.
class HttpServer {
 public:
  HttpServer(std::uint16_t port, std::size_t workers);

  virtual ~HttpServer() = default;

  DELETE_COPY_AND_ASSIGN(HttpServer);

  // Run the server's io_service loop.
  void Run();

 private:
  // Initiate an asynchronous accept operation.
  void AsyncAccept();

  // Wait for a request to stop the server.
  void AsyncAwaitStop();

  // Get the handler for incoming requests.
  virtual HttpRequestHandler* GetRequestHandler() = 0;

  // The number of worker threads.
  std::size_t workers_;

  // The io_context used to perform asynchronous operations.
  boost::asio::io_context io_context_;

  // The signal_set is used to register for process termination notifications.
  boost::asio::signal_set signals_;

  // Acceptor used to listen for incoming connections.
  boost::scoped_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
};

}  // namespace webcc

#endif  // WEBCC_HTTP_SERVER_H_
