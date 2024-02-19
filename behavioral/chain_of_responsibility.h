/**
 * Chain of Responsibility is a behavioral design pattern.
 * 1. 기능 정의 + 체인 연결 인터페이스 정의 Handler
 * 2. 각 체인의 기능 구현 ConcreteHandler
 * 기능을 여러 객체로 나눔 + 체인으로 연결 -> 각 객체가 자신의 기능만 담당
 */
#include <iostream>
#include <memory>
#include <string>

// Handler
class Handler {
 public:
  virtual Handler* SetNext(Handler* handler) {
    next_handler_ = handler;
    return handler;
  }

  virtual void Handle(const std::string& url) = 0;
  void run(const std::string& url) {
    Handle(url);

    if (next_handler_ != nullptr) next_handler_->run(url);
  }

 protected:
  Handler* next_handler_ = nullptr;
};

// ConcreteHandler
class ProtocolHandler : public Handler {
 public:
  void Handle(const std::string& url) override {
    size_t index = url.find_first_of("://");
    if (index != std::string::npos)
      std::cout << "PROTOCOL : " << url.substr(0, index) << std::endl;
    else
      std::cout << "NO PROTOCOL" << std::endl;
  }
};

// ConcreteHandler
class DomainHandler : public Handler {
 public:
  void Handle(const std::string& url) override {
    size_t start_index = url.find_first_of("://");
    size_t last_index = url.find_last_of(":");

    std::cout << "DOMAIN : ";
    if (start_index != std::string::npos) {
      if (last_index != std::string::npos)
        std::cout << url << std::endl;
      else
        std::cout << url.substr(0, last_index) << std::endl;
    } else if (start_index != last_index) {
      std::cout << url.substr(start_index + 3, last_index) << std::endl;
    } else {
      std::cout << url.substr(start_index + 3) << std::endl;
    }
  }
};

// ConcreteHandler
class PortHandler : public Handler {
 public:
  void Handle(const std::string& url) override {
    size_t index = url.find_last_of(":");

    if (index != std::string::npos)
      std::cout << url.substr(index + 1) << std::endl;
  }
};

int main() {
  Handler* handler1 = new ProtocolHandler();
  Handler* handler2 = new DomainHandler();
  Handler* handler3 = new PortHandler();

  handler1->SetNext(handler2)->SetNext(handler3);

  std::string url1 = "http://www.youtube.com:80";
  std::cout << "INPUT: " << url1 << std::endl;
  handler1->run(url1);

  std::cout << std::endl;

  std::string url2 = "https://www.essential.life.com:443";
  std::cout << "INPUT: " << url2 << std::endl;
  handler1->run(url2);

  std::cout << std::endl;

  std::string url3 = "http://www.localhost:8080";
  std::cout << "INPUT: " << url3 << std::endl;
  handler1->run(url3);
}