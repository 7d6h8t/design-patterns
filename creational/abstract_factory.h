/**
 * Abstract Factory is a creational design pattern.
 * 1. 객체 생성 역할을 factory 클래스로 몰아넣음 (의존성 제거).
 * 2. 확장성과 factory 클래스의 변동 억제(OCP 원칙)를 위해 서브클래스로 분리.
 * 3. 관련된 집합 객체의 생성을 한곳에 모아둠.
 * 호출 코드에서 생성 루틴에 대한 의존성 제거 + 확장성이 필요 + 관련 객체가 함께
 * 사용되어야 할때 사용됨.
 */

#include <iostream>
#include <memory>

// Product1
class Manager {
 protected:
  Manager() = default;
  virtual ~Manager() = default;

 public:
  virtual void function() = 0;
};

// Concrete Product1
class SoccerManager : public Manager {
 public:
  void function() override { std::cout << "soccer manager" << std::endl; }
};

class TennisManager : public Manager {
 public:
  void function() override { std::cout << "tennis manager" << std::endl; }
};

// Product2
class Player {
 protected:
  Player() = default;
  virtual ~Player() = default;

 public:
  virtual void function() = 0;
};

// Concrete Product2
class SoccerPlayer : public Player {
 public:
  void function() override { std::cout << "soccer player" << std::endl; }
};

class TennisPlayer : public Player {
 public:
  void function() override { std::cout << "tennis player" << std::endl; }
};

// Creator
class AbstractFactory {
 public:
  virtual std::unique_ptr<Manager> createManager() const = 0;
  virtual std::unique_ptr<Player> createPlayer() const = 0;
};

// Concrete Creator
class SoccerFactory : public AbstractFactory {
 public:
  std::unique_ptr<Manager> createManager() const override {
    return std::make_unique<SoccerManager>();
  }

  std::unique_ptr<Player> createPlayer() const override {
    return std::make_unique<SoccerPlayer>();
  }
};

class TennisFactory : public AbstractFactory {
 public:
  std::unique_ptr<Manager> createManager() const override {
    return std::make_unique<TennisManager>();
  }

  std::unique_ptr<Player> createPlayer() const override {
    return std::make_unique<TennisPlayer>();
  }
};

/*
int main() {
  std::unique_ptr<AbstractFactory> soccer = std::make_unique<SoccerFactory>();
  std::unique_ptr<AbstractFactory> tennis = std::make_unique<TennisFactory>();

  doSomething(*soccer);
  doSomething(*tennis);
  return 0;
}

void doSomething(const AbstractFactory& factory) {
  std::unique_ptr<Manager> manager = factory.createManager();
  std::unique_ptr<Player> player = factory.createPlayer();

  manager->function();
  player->function();
}
*/