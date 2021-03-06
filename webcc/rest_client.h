#ifndef WEBCC_REST_CLIENT_H_
#define WEBCC_REST_CLIENT_H_

#include <cassert>
#include <string>
#include <utility>  // for move()

#include "webcc/globals.h"
#include "webcc/http_response.h"

namespace webcc {

class RestClient {
 public:
  // If |port| is empty, |host| will be checked to see if it contains port or
  // not (separated by ':').
  RestClient(const std::string& host, const std::string& port = "");

  ~RestClient() = default;

  DELETE_COPY_AND_ASSIGN(RestClient);

  void set_timeout_seconds(int timeout_seconds) {
    timeout_seconds_ = timeout_seconds;
  }

  // HTTP GET request.
  // The return value indicates if the socket communication is successful
  // or not. If it's failed, check error() and timed_out() for more details.
  // For HTTP status, check response_status() instead.
  inline bool Get(const std::string& url) {
    return Request(kHttpGet, url, "");
  }

  // HTTP POST request.
  inline bool Post(const std::string& url, std::string&& content) {
    return Request(kHttpPost, url, std::move(content));
  }

  // HTTP PUT request.
  inline bool Put(const std::string& url, std::string&& content) {
    return Request(kHttpPut, url, std::move(content));
  }

  // HTTP PATCH request.
  inline bool Patch(const std::string& url, std::string&& content) {
    return Request(kHttpPatch, url, std::move(content));
  }

  // HTTP DELETE request.
  inline bool Delete(const std::string& url) {
    return Request(kHttpDelete, url, "");
  }

  HttpResponsePtr response() const { return response_; }

  int response_status() const {
    assert(response_);
    return response_->status();
  }

  const std::string& response_content() const {
    assert(response_);
    return response_->content();
  }

  bool timed_out() const { return timed_out_; }

  Error error() const { return error_; }

 private:
  bool Request(const std::string& method, const std::string& url,
               std::string&& content);

  std::string host_;
  std::string port_;

  // Timeout in seconds; only effective when > 0.
  int timeout_seconds_;

  HttpResponsePtr response_;

  // If the error was caused by timeout or not.
  bool timed_out_;

  Error error_;
};

}  // namespace webcc

#endif  // WEBCC_REST_CLIENT_H_
