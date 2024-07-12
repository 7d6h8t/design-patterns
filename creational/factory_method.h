/**
 * Factory Medthod is a creational design pattern.
 * 1. 객체 생성 역할을 factory 클래스로 몰아넣음 (의존성 제거).
 * 2. 확장성과 factory 클래스의 변동 억제(OCP 원칙)를 위해 서브클래스로 분리.
 * 호출 코드에서 생성 루틴에 대한 의존성 제거 + 확장성이 필요한 경우 사용됨.
 */
#include <iostream>
#include <memory>
#include <string>

// Product
class Log {
 public:
  virtual void Write(const std::string& log) = 0;
  // do somthing
};

// Concrete Product
class ConsoleLog : public Log {
 public:
  void Write(const std::string& log) override {}
  // do somthing
};

class FileLog : public Log {
 public:
  void Write(const std::string& log) override {}
  // do somthing
};

// Creator
class Logger {
 public:
  void Write(const std::string& log) {
    std::unique_ptr<Log> logger = CreateLog();
    logger->Write(log);
    // do somthing
    // etc..
  }

 protected:
  // Factory Method
  virtual std::unique_ptr<Log> CreateLog() = 0;
};

// Concrete Creator
class ConsoleLogFactory : public Logger {
 private:
  std::unique_ptr<Log> CreateLog() override {
    return std::make_unique<ConsoleLog>();
  }
};

class FileLogFactory : public Logger {
 private:
  std::unique_ptr<Log> CreateLog() override {
    return std::make_unique<FileLog>();
  }
};

int main() {
  std::unique_ptr<Logger> logger = std::make_unique<ConsoleLogFactory>();
  logger->Write("");

  logger = std::make_unique<FileLogFactory>();
  logger->Write("");
}