/**
 * decorator is a structural design pattern.
 */
#include <iostream>

class Weapon {
 public:
  virtual void AimAndFire() = 0;
};

class BaseWeapon : public Weapon {
 public:
  void AimAndFire() { std::cout << "fire" << std::endl; }
};

// Decorator
class Accessory : public Weapon {
 public:
  Accessory(Weapon* rifle) : rifle_(rifle) {}

  void AimAndFire() override { rifle_->AimAndFire(); }

 private:
  Weapon* rifle_;
};

// Concrete Decorator
class Generade : public Accessory {
 public:
  Generade(Weapon* rifle) : Accessory(rifle) {}

  void AimAndFire() {
    AimAndFire();
    GeneradeFire();
  }

  void GeneradeFire() { std::cout << "generade fire" << std::endl; }
};

// Concrete Decorator
class Scoped : public Accessory {
 public:
  Scoped(Weapon* rifle) : Accessory(rifle) {}

  void AimAndFire() {
    Aiming();
    AimAndFire();
  }

  void Aiming() { std::cout << "Aiming..." << std::endl; }
};

// Concrete Decorator
class Buttstock : public Accessory {
 public:
  Buttstock(Weapon* rifle) : Accessory(rifle) {}

  void AimAndFire() {
    Holding();
    AimAndFire();
  }

  void Holding() { std::cout << "holding compelete" << std::endl; }
};

int main() {
  // 1. 유탄발사기가 달린 총
  Weapon* generade_rifle = new Generade(new BaseWeapon());
  generade_rifle->AimAndFire();

  // 2. 개머리판을 장착하고 스코프를 달은 총
  Weapon* buttstock_scoped_rifle =
      new Buttstock(new Generade(new BaseWeapon()));
  buttstock_scoped_rifle->AimAndFire();

  // 3. 유탄발사기 + 개머리판 + 스코프가 달린 총
  Weapon* buttstock_scoped_generade_rifle =
      new Buttstock(new Scoped(new Generade(new BaseWeapon())));
  buttstock_scoped_generade_rifle->AimAndFire();
  return 0;
}