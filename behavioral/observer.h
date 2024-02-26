/**
 * Observer is a behavioral design pattern.
 * 1. 상태가 변경될 주체 (감시 당할놈) 인터페이스 정의 Subject
 * 2. Subject 의 상태를 감지할 놈 인터페이스 정의 Observer
 * 3. 각 인터페이스의 실체 구현
 * 한 객체 (Subject) 의 상태 변경 -> 의존하는 다른 객체(OBserver) 모두에 notify
 * & update
 */
#include <algorithm>
#include <deque>
#include <iostream>
#include <string>

class Observer;

// Subject
class Subject {
 public:
  virtual ~Subject(){};

  virtual void Attach(Observer* observer) = 0;
  virtual void Detach(Observer* observer) = 0;
  virtual void Notify() = 0;
};

// Observer
class Observer {
 public:
  virtual ~Observer(){};
  virtual void Update(const std::string& title) = 0;
};

// ConcreteSubject
class NespaperCompny : public Subject {
 public:
  void Attach(Observer* observer) override { observers_.push_back(observer); }

  void Detach(Observer* observer) override {
    auto itr = std::find(observers_.begin(), observers_.end(), observer);
    if (itr != observers_.end()) observers_.erase(itr);
  }

  void Notify() override {
    for (auto& observer : observers_) observer->Update(title_);
  }

  void pulish(const std::string title) {
    title_ = title;
    Notify();  // 상태 변경 시 옵저버들에게 알림
  }

 private:
  std::deque<Observer*> observers_;
  std::string title_;
};

// ConcreteObserver
class Subscriber : public Observer {
 public:
  Subscriber(const std::string& name) : name_(name) {}
  void Update(const std::string& title) override {
    std::cout << name_ << " received news: " << title << std::endl;
  }

 private:
  std::string name_;
};

int main() {
  NespaperCompny newspaper_company;

  Subscriber steve("steve");
  Subscriber peter("peter");
  Subscriber jane("jane");

  newspaper_company.Attach(&steve);
  newspaper_company.Attach(&peter);
  newspaper_company.Attach(&jane);

  newspaper_company.pulish("Breaking News: Important Event!");

  return 0;
}