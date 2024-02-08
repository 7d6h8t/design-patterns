/**
 * decorator is a structural design pattern.
 * 1. 기본 기능 정의된 인터페이스 Component
 * 2. Component 를 구현한 기본 객체 Concrete Component 추가
 * 3. Component 의 기능을 하위 객체에 위임하기 위한 Decorator 추가
 * 4. Decorator 를 상속받아 실제 추가 기능을 Concrete Decorator 에 구현
 * 기존 코드를 수정x + 추가기능 확장 가능 (여러 조합을 통해)
 */
#include <iostream>
#include <memory>

// Component : 기본 기능 정의
class Weapon {
 public:
  virtual ~Weapon() = default;
  virtual void Fire() const = 0;

 protected:
  Weapon() = default;
};

// Concrete Component : 기본 기능을 구현한 객체 정의 (여기에 Decorator 들이
// 추가로 붙게됨)
class BaseWeapon : public Weapon {
 public:
  void Fire() const override { std::cout << "fire" << std::endl; }
};

// Decorator : 추가될 기능 정의
class Accessory : public Weapon {
 public:
  Accessory(std::unique_ptr<Weapon>&& rifle) : rifle_(std::move(rifle)) {}

  void Fire() const override { rifle_->Fire(); }

 private:
  std::unique_ptr<Weapon> rifle_;
};

// Concrete Decorator : 실제로 추가될 기능 구현
class Generade : public Accessory {
 public:
  Generade(std::unique_ptr<Weapon>&& rifle) : Accessory(std::move(rifle)) {}

  void Fire() const override {
    Accessory::Fire();
    GeneradeFire();
  }

  void GeneradeFire() const { std::cout << "generade fire" << std::endl; }
};

// Concrete Decorator : 실제로 추가될 기능 구현
class Scoped : public Accessory {
 public:
  Scoped(std::unique_ptr<Weapon>&& rifle) : Accessory(std::move(rifle)) {}

  void Fire() const override {
    Aiming();
    Accessory::Fire();
  }

  void Aiming() const { std::cout << "Aiming..." << std::endl; }
};

// Concrete Decorator : 실제로 추가될 기능 구현
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