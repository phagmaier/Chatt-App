#include "server.h"

Server::Server(asio::io_context &ctx, uint16_t port)
    : acceptor_{ctx, tcp::endpoint(tcp::v4(), port)} {
  accept();
}

void Server::accept() {

  acceptor_.async_accept(
      [this](boost::system::error_code ec, tcp::socket sock) {
        if (!ec)
          std::make_shared<Session>(std::move(sock), )->start();
        accept();
      });
}
