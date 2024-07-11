/**
 * Builder is a creational design pattern.
 * 1. 복잡한 인스턴스 생성 단계를 builder 클래스에 위임.
 * 2. Director 클래스가 builder 의 단계들을 조합하여 최종 결과물 생성.
 * 클래스의 인스턴스생성이 매우 복잡하며 많은 설정이 필요한 경우 -> 생성을
 * 단계적으로 수행(유연성) 하고 각 단계에서 각각의 설정을 단순화(복잡성 해결)
 * 하기위해 사용됨
 */
#include <iostream>
#include <memory>
#include <string>

// Product : Director 가 Builder로 만들어낸 결과물.
class Character {
 public:
  void SetWeapon(const int32_t weapon) { weapon_ = weapon; }
  void SetArmor(const int32_t armor) { armor_ = armor; }
  void SetName(const std::string& name) { name_ = name; }

  void ShowInformation() {
    std::cout << "Name :" << name_ << std::endl;
    std::cout << "Weapon power:" << weapon_ << std::endl;
    std::cout << "Armor defence:" << armor_ << std::endl;
  }

 private:
  int32_t weapon_;
  int32_t armor_;
  std::string name_;
};

// Builder
class CharacterBuilder {
 public:
  virtual void BuildWeapon(const int32_t weapon) = 0;
  virtual void BuildArmor(const int32_t armor) = 0;
  virtual void BuildName(const std::string& name) = 0;
  virtual Character GetResult() = 0;
};

// Concrete Builder for archer
class TeranBuilder : public CharacterBuilder {
 public:
  void BuildWeapon(const int32_t weapon) override {
    character_.SetWeapon(weapon);
  }
  void BuildArmor(const int32_t armor) override { character_.SetArmor(armor); }
  void BuildName(const std::string& name) override { character_.SetName(name); }

  Character GetResult() override { return character_; }

 private:
  Character character_;
};

// Director
class CharacterDirector {
 public:
  Character CreateMarine(std::unique_ptr<CharacterBuilder>&& builder) {
    builder->BuildWeapon(10);
    builder->BuildArmor(5);
    builder->BuildName("Marine");
    return builder->GetResult();
  }

  Character CreateMedic(std::unique_ptr<CharacterBuilder>&& builder) {
    builder->BuildWeapon(0);
    builder->BuildArmor(6);
    builder->BuildName("Medic");
    return builder->GetResult();
  }
};

int main() {
  CharacterDirector barak;

  Character marine = barak.CreateMarine(std::make_unique<TeranBuilder>());
  Character medic = barak.CreateMedic(std::make_unique<TeranBuilder>());

  marine.ShowInformation();
  medic.ShowInformation();
  return 0;
}