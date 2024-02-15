/**
 * proxy is a structural design pattern.
 * 1. 대상 클래스의 인터페이스 Subject 를 추가.
 * 2. Subject 인터페이스를 구현한 실제 대상 클래스 RealSubject 정의.
 * 3. RealSubject 와 간접적으로 연결하기 위해 Subject 를 구현한 Proxy 추가.
 * Proxy 에 추가 기능 (보안, 캐싱, 로깅 등등) 구현 후, 복합객체 (맴버변수)
 * RealSubject 의 함수로 위임.
 * 기존 클래스를 변형하지 않고 기능 (보안, 캐싱,로깅 등)을 추가하고자 할때
 * 중간에 프록시를 추가하여 기능을 더하기 위해 사용.
 */
#include <iostream>
#include <memory>

// Subject
class Subject {
 public:
  virtual void Request() const = 0;
};

// RealSubject
class RealSubject : public Subject {
 public:
  void Request() const override {
    std::cout << "real subject : request" << std::endl;
  }
};

// Proxy
class Proxy : public Subject {
 public:
  Proxy(std::unique_ptr<RealSubject>&& real_subject)
      : real_subject_(std::move(real_subject)) {}

  void Request() const override {
    if (CheckAccess()) {
      real_subject_->Request();
      Logging();
    }
  }

 private:
  bool CheckAccess() const { return true; }
  void Logging() const { std::cout << "logging..." << std::endl; }

  std::unique_ptr<RealSubject> real_subject_;
};

/*
int main() {
  std::unique_ptr<RealSubject> subject = std::make_unique<RealSubject>();
  subject->Request();

  std::unique_ptr<Subject> proxy = std::make_unique<Proxy>(std::move(subject));
  proxy->Request();
}
*/