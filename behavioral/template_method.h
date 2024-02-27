/**
 * Tempate Method is a behavioral design pattern.
 * 1. 공통 기능 구현 + 다른 부분은 추상함수로 부모 클래스 정의 AbstractClass
 * 2. 부모 클래스를 상속받아 다른 부분으로 정의된 추상함수 구현 ConcreteClass
 * 부모 클래스에서 처리 흐름 제어 -> 자식 클래스에서 처리 내용 구현.
 * 동일한 코드에서 몇개의 다른 부분만 필요한 경우, 코드 중복을 위해 사용.
 */

#include <iostream>

// AbstractClass
class CoffeeMaker {
 public:
  virtual ~CoffeeMaker() {}
  void Make() {
    std::cout << "- Boiling water" << std::endl;
    BrewCoffeeGrounds();
    std::cout << "- Pouring coffe into cup" << std::endl;
    AddCondiments();
    Hook();
  }

 protected:
  virtual void BrewCoffeeGrounds() = 0;
  virtual void AddCondiments() = 0;
  virtual void Hook() {
    std::cout << "* Optional step after making coffee" << std::endl;
  }
};

// ConcreteClass
class Americano : public CoffeeMaker {
 public:
  void BrewCoffeeGrounds() override {
    std::cout << "- Brewing Americano coffee grounds" << std::endl;
  }
  void AddCondiments() override {}
};

// ConcreteClass
class Latte : public CoffeeMaker {
 public:
  void BrewCoffeeGrounds() override {
    std::cout << "- Brewing espresso coffee grounds" << std::endl;
  }

  void AddCondiments() override {
    std::cout << "- Adding milk to make a latte" << std::endl;
  }

  void Hook() override {
    std::cout << "- Frothing milk for a perfect latte" << std::endl;
  }
};

/*
int main() {
  std::cout << "[Make Americano]" << std::endl;
  Americano americano;
  americano.Make();

  std::cout << "[Make Latte]" << std::endl;
  Latte latte;
  latte.Make();
}
*/