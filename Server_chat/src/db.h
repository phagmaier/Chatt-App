#pragma once
#include <mutex>
#include <sqlite3.h>
#include <sstream>
#include <string>
#include <vector>

class Db {
public:
  explicit Db(const std::string &path);
  ~Db();

  /* pure queries */
  std::pair<bool, std::vector<std::string>> get_rooms();
  std::pair<bool, std::string> load_messages(const std::string &room);
  std::pair<bool, std::vector<std::string>> get_logs(int limit, int roomId);
  std::pair<bool, bool> is_unique(const char *username);
  bool verify_login(const char *u, const char *p);

  /* actions that mutate */
  bool insert_message(const std::string &room, const std::string &user,
                      const std::string &text);
  bool insert_message(const std::string &room, const std::string &user,
                      const char *text);

  /* helpers exposed only if you really need them elsewhere */
  int get_room_id(const std::string &name);
  int create_room(const std::string &name);
  int get_user_id(const std::string &user);
  int create_user(const char *u, const char *p);

private:
  static bool ok(int rc) { // treat BUSY/LOCKED as failure
    return rc == SQLITE_OK || rc == SQLITE_DONE || rc == SQLITE_ROW;
  }

  sqlite3 *db_{nullptr};
  mutable std::mutex m_;
};
