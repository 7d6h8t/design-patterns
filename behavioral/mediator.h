/**
 * Mediator is a behavioral design pattern.
 * 1. 상호 작용을 중재할 인터페이스 정의 Mediator
 * 2. Mediator 에 등록되어 상호작용 할 클래스 정의 Colleague
 * 3. Colleague 를 관리하고 중재하는 클래스 구현 ConcreteMediator
 * 클래스간 상호작용을 캡슐화 -> 하나의 클래스에 위임.
 */
#include <algorithm>
#include <deque>
#include <iostream>
#include <string>

class User;

// Mediator
class Messenger {
 public:
  virtual void AddUser(User* user) = 0;
  virtual void RemoveUser(User* user) = 0;
  virtual void Mediate(const std::string& message, User* sender) = 0;
};

// Colleague
class User {
 public:
  User(Messenger* messenger) : messenger_(messenger) {
    messenger_->AddUser(this);
  }
  virtual ~User() {}

  void Send(const std::string& message) {
    std::cout << "[" << GetID() << "] -> " << message << std::endl;
    messenger_->Mediate(message, this);
  }

  void Receive(const std::string& message) {
    std::cout << message << " -> [" << GetID() << "]" << std::endl;
  }
  virtual std::string GetID() = 0;

 protected:
  Messenger* messenger_;
};

// ConcreteMediator
class KakaoTalk : public Messenger {
 public:
  void AddUser(User* user) override { user_queue_.push_back(user); }
  void RemoveUser(User* user) override {
    auto itr = std::find(user_queue_.begin(), user_queue_.end(), user);
    if (itr != user_queue_.end()) user_queue_.erase(itr);
  }

  void Mediate(const std::string& message, User* sender) override {
    for (User* user : user_queue_) {
      if (user != sender) user->Receive(message);
    }
  }

 private:
  std::deque<User*> user_queue_;
};

// ConcreteColleague
class ConcreteUser : public User {
 public:
  ConcreteUser(Messenger* tower, const std::string id) : User(tower), id_(id) {}
  virtual std::string GetID() override { return id_; }

 private:
  std::string id_;
};

int main() {
  KakaoTalk messenger;

  ConcreteUser jack(&messenger, "jack");
  ConcreteUser jennie(&messenger, "jennie");
  ConcreteUser him(&messenger, "him");

  jack.Send("hello");
  jennie.Send("hi");

  return 0;
}