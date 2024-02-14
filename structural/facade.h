/**
 * facade is a structural design pattern.
 * 1. 다양한 기능의 기존 클래스 (Subsystem)
 * 2. 기존 Subsystem 을 감싸 클라이언트에게 제공 (Facade)
 * Facade + Subsytem 으로 구성하여 사용자에게 Facade 를 통해 접근하도록 구현.
 * 복잡한 시스템을 간단한 인터페이스로 감싸서 간단하게 사용하고 싶을때 사용.
 */
#include <iostream>
#include <memory>
#include <string>

class Subsystem1 {
 public:
  std::string Operation1() const { return "Subsystem1: Ready!\n"; }
  // ...
  std::string OperationN() const { return "Subsystem1: GO!\n"; }
};

class Subsystem2 {
 public:
  std::string Operation1() const { return "Subsystem2: Get ready!\n"; }
  // ...
  std::string OperationZ() const { return "Subsystem2: Fire!\n"; }
};

class Facade {
 public:
  Facade(std::unique_ptr<Subsystem1>&& subsystem1 = nullptr,
         std::unique_ptr<Subsystem2>&& subsystem2 = nullptr) {
    subsystem1_ =
        subsystem1 ? std::move(subsystem1) : std::make_unique<Subsystem1>();
    subsystem2_ =
        subsystem2 ? std::move(subsystem2) : std::make_unique<Subsystem2>();
  }

  virtual ~Facade() = default;

  std::string Operation() {
    std::string result = "Facade initializes subsystems:\n";
    result += subsystem1_->Operation1();
    result += subsystem2_->Operation1();
    result += "Facade orders subsystems to perform the action:\n";
    result += subsystem1_->OperationN();
    result += subsystem2_->OperationZ();
    return result;
  }

 protected:
  std::unique_ptr<Subsystem1> subsystem1_;
  std::unique_ptr<Subsystem2> subsystem2_;
};

int main() {
  Facade facade(std::make_unique<Subsystem1>(), std::make_unique<Subsystem2>());
  std::cout << facade.Operation();

  return 0;
}
