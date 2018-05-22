#include <iostream>

#include "webcc/logger.h"
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

  LOG_INIT(webcc::VERB, 0);

  unsigned short port = std::atoi(argv[1]);

  std::size_t workers = 2;

  try {
    webcc::RestServer server(port, workers);

    server.Bind(std::make_shared<BookListService>(), "/books", false);

    server.Bind(std::make_shared<BookDetailService>(), "/book/(\\d+)", true);

    server.Run();

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
