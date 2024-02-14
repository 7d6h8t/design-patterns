/**
 * facade is a structural design pattern.
 * 시스템이 복잡하여 간단한 인터페이스를 통해 접근하고 싶을때 사용.
 */
#include <chrono>
#include <exception>
#include <map>
#include <string>
#include <thread>

class Row {
 public:
  Row(const std::string& name, const std::string& birthday,
      const std::string& email)
      : name_(name), birthday_(birthday), email_(email) {}

  std::string GetName() const { return name_; }
  std::string GetBirthday() const { return birthday_; }
  std::string GetEmail() const { return email_; }

 private:
  std::string name_;
  std::string birthday_;
  std::string email_;
};

class DBMS {
 public:
  void Put(const std::string& name, const Row& row) { db_[name] = row; }
  Row Query(const std::string& name) {
    try {
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    } catch (...) {
    }

    return db_[name];
  }

 private:
  std::map<std::string, Row> db_;
};

class Cache {
 public:
  void Put(const Row& row) { cache_[row.GetName()] = row; }
  Row Get(const std::string& name) { return cache_[name]; }

 private:
  std::map<std::string, Row> cache_;
};

class Message {
 public:
  Message(const Row& row) : row_(row) {}

  std::string MakeName() { return "Name : \"" + row_.GetName() + "\""; }
  std::string MakeBirthday() { return "Birthday : " + row_.GetBirthday(); }
  std::string MakeEmail() { return "Email : " + row_.GetEmail(); }

 private:
  Row row_;
};

class Facade {
 public:
  void Insert() {
    dbms_.Put("aaa", Row{"aaa", "1890-02-14", "aaa@naver.com"});
    dbms_.Put("bbb", Row{"bbb", "1820-11-02", "bbb@naver.com"});
    dbms_.Put("ccc", Row{"ccc", "710-08-27", "ccc@naver.com"});
  }

  void Run(const std::string& name) {
    Row row = cache_.Get(name);
    if (row == null) }

 private:
  DBMS dbms_;
  Cache cache_;
};

int main() {
  Facade facade;
  facade.Insert();
  facade.Run("aaa");

  return 0;
}
