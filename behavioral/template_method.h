/**
 * Tempate Method is a behavioral design pattern.
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

int main() {
  std::cout << "[Make Americano]" << std::endl;
  Americano americano;
  americano.Make();

  std::cout << "[Make Latte]" << std::endl;
  Latte latte;
  latte.Make();
}