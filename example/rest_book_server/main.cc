#include <iostream>
#include "webcc/rest_server.h"
#include "book_services.h"

static void Help(const char* argv0) {
  std::cout << "Usage: " << argv0 << " <port>" << std::endl;
  std::cout << "  E.g.," << std::endl;
  std::cout << "    " << argv0 << " 8080" << std::endl;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    Help(argv[0]);
    return 1;
  }

  unsigned short port = std::atoi(argv[1]);

  std::size_t workers = 2;

  try {
    webcc::RestServer server(port, workers);

    server.RegisterService(std::make_shared<BookListService>(),
                           "/books");

    server.RegisterService(std::make_shared<BookDetailService>(),
                           "/books/(\\d+)");

    // For test purpose.
    // Timeout like 60s makes more sense in a real product.
    // Leave it as default (0) for no timeout control.
    server.set_timeout_seconds(1);

    server.Run();

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}