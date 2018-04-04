#ifndef CSOAP_HTTP_SERVER_H_
#define CSOAP_HTTP_SERVER_H_

#include <string>
#include <vector>

#include "boost/scoped_ptr.hpp"
#include "boost/thread/thread.hpp"

#include "boost/asio/io_context.hpp"
#include "boost/asio/signal_set.hpp"
#include "boost/asio/ip/tcp.hpp"

#include "csoap/http_session.h"

namespace csoap {

class HttpRequestHandler;

// HTTP server accepts TCP connections from TCP clients.
// NOTE: Only support IPv4.
class HttpServer {
public:
  HttpServer(const HttpServer&) = delete;
  HttpServer& operator=(const HttpServer&) = delete;

  HttpServer(unsigned short port, std::size_t workers);

  virtual ~HttpServer();

  // Run the server's io_service loop.
  void Run();

private:
  // Initiate an asynchronous accept operation.
  void DoAccept();

  // Wait for a request to stop the server.
  void DoAwaitStop();

protected:
  // The handler for all incoming requests.
  // TODO: Replace with virtual GetRequestHandler()?
  HttpRequestHandler* request_handler_;

private:
  // The number of worker threads.
  std::size_t workers_;

  // The io_context used to perform asynchronous operations.
  boost::asio::io_context io_context_;

  // The signal_set is used to register for process termination notifications.
  boost::asio::signal_set signals_;

  // Acceptor used to listen for incoming connections.
  boost::scoped_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
};

}  // namespace csoap

#endif  // CSOAP_HTTP_SERVER_H_
