#include "db.h"

/* constructor / destructor ---------------------------------------- */
Db::Db(const std::string &path) {
  if (!ok(sqlite3_open(path.c_str(), &db_)))
    db_ = nullptr;
  if (db_) {
    sqlite3_exec(db_, "PRAGMA journal_mode=WAL;", nullptr, nullptr, nullptr);
    sqlite3_busy_timeout(db_, 10'000); // 10-second grace
  }
}

Db::~Db() {
  if (db_)
    sqlite3_close(db_);
}

/* ------------ get_rooms ------------------------------------------ */
std::pair<bool, std::vector<std::string>> Db::get_rooms() {
  std::lock_guard lg{m_};
  std::vector<std::string> out;
  if (!db_)
    return {false, {}};

  sqlite3_stmt *st = nullptr;
  if (!ok(sqlite3_prepare_v2(db_, "SELECT name FROM rooms", -1, &st, nullptr)))
    return {false, {}};

  while (sqlite3_step(st) == SQLITE_ROW)
    out.emplace_back(
        reinterpret_cast<const char *>(sqlite3_column_text(st, 0)));

  sqlite3_finalize(st);
  return {true, std::move(out)};
}

/* ------------ insert_message (string) ---------------------------- */
bool Db::insert_message(const std::string &room, const std::string &user,
                        const std::string &text) {
  std::lock_guard lg{m_};
  if (!db_)
    return false;

  int r = get_room_id(room);
  int u = get_user_id(user);
  if (r < 0 || u < 0)
    return false;

  sqlite3_stmt *st = nullptr;
  if (!ok(sqlite3_prepare_v2(
          db_, "INSERT INTO messages(room_id,user_id,text) VALUES(?,?,?)", -1,
          &st, nullptr)))
    return false;

  sqlite3_bind_int(st, 1, r);
  sqlite3_bind_int(st, 2, u);
  sqlite3_bind_text(st, 3, text.c_str(), -1, SQLITE_TRANSIENT);

  bool good = ok(sqlite3_step(st));
  sqlite3_finalize(st);
  return good;
}

/* ------------ insert_message (const-char* overload) -------------- */
bool Db::insert_message(const std::string &room, const std::string &user,
                        const char *text) {
  return insert_message(room, user, std::string{text});
}

/* ------------ load_messages -------------------------------------- */
std::pair<bool, std::string> Db::load_messages(const std::string &room) {
  std::lock_guard lg{m_};
  std::ostringstream out;
  if (!db_)
    return {false, ""};

  int r = get_room_id(room);
  if (r < 0)
    return {false, ""};

  sqlite3_stmt *st = nullptr;
  const char *SQL = "SELECT u.username,m.text,m.ts "
                    "FROM messages m JOIN users u ON u.id=m.user_id "
                    "WHERE m.room_id=? ORDER BY m.ts ASC";
  if (!ok(sqlite3_prepare_v2(db_, SQL, -1, &st, nullptr)))
    return {false, ""};

  sqlite3_bind_int(st, 1, r);

  while (sqlite3_step(st) == SQLITE_ROW) {
    auto *u = sqlite3_column_text(st, 0);
    auto *t = sqlite3_column_text(st, 1);
    auto *ts = sqlite3_column_text(st, 2);
    out << '[' << ts << "] " << u << ": " << t << '\n';
  }
  sqlite3_finalize(st);
  return {true, out.str()};
}

/* ------------ is_unique ------------------------------------------ */
std::pair<bool, bool> Db::is_unique(const char *user) {
  std::lock_guard lg{m_};
  if (!db_)
    return {false, false};

  sqlite3_stmt *st = nullptr;
  if (!ok(sqlite3_prepare_v2(db_,
                             "SELECT 1 FROM users WHERE username=? LIMIT 1", -1,
                             &st, nullptr)))
    return {false, false};

  sqlite3_bind_text(st, 1, user, -1, SQLITE_STATIC);
  int rc = sqlite3_step(st);
  sqlite3_finalize(st);

  if (rc == SQLITE_ROW)
    return {true, false}; // found duplicate
  if (rc == SQLITE_DONE)
    return {true, true}; // unique
  return {false, false}; // any error/timeout
}

/* ------------ verify_login --------------------------------------- */
bool Db::verify_login(const char *u, const char *p) {
  std::lock_guard lg{m_};
  if (!db_)
    return false;

  sqlite3_stmt *st = nullptr;
  if (!ok(sqlite3_prepare_v2(
          db_, "SELECT 1 FROM users WHERE username=? AND password=?", -1, &st,
          nullptr)))
    return false;

  sqlite3_bind_text(st, 1, u, -1, SQLITE_STATIC);
  sqlite3_bind_text(st, 2, p, -1, SQLITE_STATIC);
  bool ok_login = (sqlite3_step(st) == SQLITE_ROW);
  sqlite3_finalize(st);
  return ok_login;
}

/* ------------ get_logs (last N) ---------------------------------- */
std::pair<bool, std::vector<std::string>> Db::get_logs(int lim, int roomId) {
  std::lock_guard lg{m_};
  std::vector<std::string> logs;
  if (!db_)
    return {false, {}};

  sqlite3_stmt *st = nullptr;
  const char *SQL = "SELECT username,text FROM( "
                    "  SELECT u.username,m.text,m.ts "
                    "  FROM messages m JOIN users u ON u.id=m.user_id "
                    "  WHERE m.room_id=? ORDER BY m.ts DESC LIMIT ?)"
                    "ORDER BY ts ASC";
  if (!ok(sqlite3_prepare_v2(db_, SQL, -1, &st, nullptr)))
    return {false, {}};

  sqlite3_bind_int(st, 1, roomId);
  sqlite3_bind_int(st, 2, lim);

  while (sqlite3_step(st) == SQLITE_ROW) {
    auto *n = sqlite3_column_text(st, 0);
    auto *t = sqlite3_column_text(st, 1);
    logs.emplace_back('[' + std::string{reinterpret_cast<const char *>(n)} +
                      "]: " + reinterpret_cast<const char *>(t));
  }
  sqlite3_finalize(st);
  return {true, std::move(logs)};
}

/* ------------ ID helpers (same pattern) -------------------------- */
int Db::get_room_id(const std::string &name) {
  std::lock_guard lg{m_};
  if (!db_)
    return -1;

  sqlite3_stmt *st = nullptr;
  if (!ok(sqlite3_prepare_v2(db_, "SELECT id FROM rooms WHERE name=?", -1, &st,
                             nullptr)))
    return -1;

  sqlite3_bind_text(st, 1, name.c_str(), -1, SQLITE_TRANSIENT);
  int id = (sqlite3_step(st) == SQLITE_ROW) ? sqlite3_column_int(st, 0) : -1;
  sqlite3_finalize(st);
  return id;
}

int Db::create_room(const std::string &name) {
  std::lock_guard lg{m_};
  if (!db_)
    return -1;

  sqlite3_stmt *st = nullptr;
  if (!ok(sqlite3_prepare_v2(db_, "INSERT INTO rooms(name) VALUES(?)", -1, &st,
                             nullptr)))
    return -1;

  sqlite3_bind_text(st, 1, name.c_str(), -1, SQLITE_TRANSIENT);
  if (!ok(sqlite3_step(st))) {
    sqlite3_finalize(st);
    return -1;
  }
  sqlite3_finalize(st);
  return get_room_id(name);
}

int Db::get_user_id(const std::string &u) {
  std::lock_guard lg{m_};
  if (!db_)
    return -1;

  sqlite3_stmt *st = nullptr;
  if (!ok(sqlite3_prepare_v2(db_, "SELECT id FROM users WHERE username=?", -1,
                             &st, nullptr)))
    return -1;

  sqlite3_bind_text(st, 1, u.c_str(), -1, SQLITE_TRANSIENT);
  int id = (sqlite3_step(st) == SQLITE_ROW) ? sqlite3_column_int(st, 0) : -1;
  sqlite3_finalize(st);
  return id;
}

int Db::create_user(const char *u, const char *p) {
  std::lock_guard lg{m_};
  if (!db_)
    return -1;

  sqlite3_stmt *st = nullptr;
  if (!ok(sqlite3_prepare_v2(db_,
                             "INSERT INTO users(username,password) VALUES(?,?)",
                             -1, &st, nullptr)))
    return -1;
  sqlite3_bind_text(st, 1, u, -1, SQLITE_STATIC);
  sqlite3_bind_text(st, 2, p, -1, SQLITE_STATIC);
  if (!ok(sqlite3_step(st))) {
    sqlite3_finalize(st);
    return -1;
  }
  sqlite3_finalize(st);
  return get_user_id(u);
}
