/**
 * decorator is a structural design pattern.
 */
#include <iostream>
#include <memory>

// Component
class Weapon {
 public:
  virtual ~Weapon() = default;
  virtual void Fire() const = 0;

 protected:
  Weapon() = default;
};

// Concrete Component
class BaseWeapon : public Weapon {
 public:
  void Fire() const override { std::cout << "fire" << std::endl; }
};

// Decorator
class Accessory : public Weapon {
 public:
  Accessory(std::unique_ptr<Weapon>&& rifle) : rifle_(std::move(rifle)) {}

  void Fire() const override { rifle_->Fire(); }

 private:
  std::unique_ptr<Weapon> rifle_;
};

// Concrete Decorator
class Generade : public Accessory {
 public:
  Generade(std::unique_ptr<Weapon>&& rifle) : Accessory(std::move(rifle)) {}

  void Fire() const override {
    Accessory::Fire();
    GeneradeFire();
  }

  void GeneradeFire() const { std::cout << "generade fire" << std::endl; }
};

// Concrete Decorator
class Scoped : public Accessory {
 public:
  Scoped(std::unique_ptr<Weapon>&& rifle) : Accessory(std::move(rifle)) {}

  void Fire() const override {
    Aiming();
    Accessory::Fire();
  }

  void Aiming() const { std::cout << "Aiming..." << std::endl; }
};

// Concrete Decorator
class Buttstock : public Accessory {
 public:
  Buttstock(std::unique_ptr<Weapon>&& rifle) : Accessory(std::move(rifle)) {}

  void Fire() const override {
    Holding();
    Accessory::Fire();
  }

  void Holding() const { std::cout << "holding compelete" << std::endl; }
};

int main() {
  // 1. 유탄발사기가 달린 총
  std::unique_ptr<Weapon> generade_rifle =
      std::make_unique<Generade>(std::make_unique<BaseWeapon>());
  generade_rifle->Fire();

  // 2. 개머리판을 장착하고 스코프를 달은 총
  std::unique_ptr<Weapon> buttstock_scoped_rifle = std::make_unique<Buttstock>(
      std::make_unique<Generade>(std::make_unique<BaseWeapon>()));
  buttstock_scoped_rifle->Fire();

  // 3. 유탄발사기 + 개머리판 + 스코프가 달린 총
  std::unique_ptr<Weapon> buttstock_scoped_generade_rifle =
      std::make_unique<Buttstock>(std::make_unique<Scoped>(
          std::make_unique<Generade>(std::make_unique<BaseWeapon>())));
  buttstock_scoped_generade_rifle->Fire();
  return 0;
}