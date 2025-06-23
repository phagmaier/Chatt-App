#pragma once
#include "db.h"
#include <algorithm>
#include <boost/asio.hpp>
#include <cstdint>
#include <deque>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using boost::asio::ip::tcp;
namespace asio = boost::asio;

constexpr std::size_t HEADER_LEN = 2;
constexpr std::size_t MAX_BODY = 4096;

class Session : public std::enable_shared_from_this<Session> {
public:
  Session(tcp::socket sock, Db &db);
  void start();

private:
  void read_header();
  void read_msg();
  void login_user();
  void insert_user();
  void write_message();
  void parse_header();

  tcp::socket socket_;
  asio::strand<asio::any_io_executor> strand_;

  std::array<char, HEADER_LEN> header_buf_{};
  std::vector<char> body_buf_;
  std::deque<std::string> out_queue_;
};

class Server {
public:
  Server(asio::io_context &ctx, uint16_t port);

private:
  void accept();
  tcp::acceptor acceptor_;
  Db db;
};
