#pragma once
#include <sqlite3.h>
#include <string>
#include <vector>
struct Db {
  Db(const std::string &path);
  ~Db();

  bool insertMessage(const std::string &room, const std::string &user,
                     const std::string &text);
  bool insertMessage(const std::string &room, const std::string &user,
                     const char *text);

  std::string loadMessages(const std::string &room);

  int getRoomId(const std::string &name);
  int createRoom(const std::string &name);

  int getUserId(const std::string &username);
  int createUser(const std::string &username, const std::string &password);

  bool verifyLogin(const char *username, const char *password);
  std::vector<std::string> get_logs(int lim, int roomId);

  sqlite3 *db = nullptr;
};
