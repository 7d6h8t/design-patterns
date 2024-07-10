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

// Product : Director 가 Builder로 만들어낸 결과물.
class Character {
 public:
  Character(const int32_t weapon, const int32_t armor)
      : weapon_(weapon), armor_(armor) {}

  void ShowInformation() {
    std::cout << "Weapon power:" << weapon_ << std::endl;
    std::cout << "Armor defence:" << armor_ << std::endl;
  }

 private:
  int32_t weapon_;
  int32_t armor_;
};

// Builder
class CharacterBuilder {
 public:
  virtual int32_t CreateWeapon() = 0;
  virtual int32_t CreateArmor() = 0;
};

// Concrete Builder for archer
class ArcherBuilder : public CharacterBuilder {
 public:
  int32_t CreateWeapon() override { return 200; }
  int32_t CreateArmor() override { return 50; }
};

// Concrete builder for warrior
class WarriorBuilder : public CharacterBuilder {
 public:
  int32_t CreateWeapon() override { return 50; }
  int32_t CreateArmor() override { return 200; }
};

// Director
class CharacterDirector {
 public:
  void SetBuilder(std::unique_ptr<CharacterBuilder>&& builder) {
    builder_ = std::move(builder);
  }

  Character CreateCharacter() {
    return {builder_->CreateWeapon(), builder_->CreateArmor()};
  }

 private:
  std::unique_ptr<CharacterBuilder> builder_ = nullptr;
};

int main() {
  CharacterDirector director;

  director.SetBuilder(std::make_unique<ArcherBuilder>());

  Character character = director.CreateCharacter();
  character.ShowInformation();

  director.SetBuilder(std::make_unique<WarriorBuilder>());

  character = director.CreateCharacter();
  character.ShowInformation();

  return 0;
}