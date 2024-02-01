/**
 * Factory Medthod is a creational design pattern.
 * 1. 객체 생성 역할을 factory 클래스로 몰아넣음 (의존성 제거).
 * 2. 확장성과 factory 클래스의 변동 억제(OCP 원칙)를 위해 서브클래스로 분리.
 * 호출 코드에서 생성 루틴에 대한 의존성 제거 + 확장성이 필요한 경우 사용됨.
 */

#include <iostream>
#include <memory>

// Product
class User {
 protected:
  User() = default;

 public:
  virtual ~User() = default;
  virtual void signUp() = 0;
};

// Concrete Product
class NaverUser : public User {
 public:
  void signUp() override { std::cout << "sign up with naver id" << std::endl; }
};

class KakaoUser : public User {
 public:
  void signUp() override { std::cout << "sign up with kakao id" << std::endl; }
};

// Creator
class Factory {
 public:
  std::unique_ptr<User> newInstance() {
    std::unique_ptr<User> user = std::move(create());
    user->signUp();
    return std::move(user);
  }

 protected:
  virtual std::unique_ptr<User> create() = 0;
};

// Concrete Creator
class NaverFactory : public Factory {
  std::unique_ptr<User> create() { return std::make_unique<NaverUser>(); }
};

class KakaoFactory : public Factory {
  std::unique_ptr<User> create() { return std::make_unique<KakaoUser>(); }
};

/*
int main() {
  std::unique_ptr<Factory> naverFactory = std::make_unique<NaverFactory>();
  std::unique_ptr<User> naverUser = std::move(naverFactory->newInstance());

  std::unique_ptr<Factory> kakoFactory = std::make_unique<KakaoFactory>();
  std::unique_ptr<User> kakaoUser = std::move(kakoFactory->newInstance());

  return 0;
}
*/