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
class House {
 public:
  House(const uint32_t door, const uint32_t window, const uint32_t pool,
        const uint32_t garage, const uint32_t garden)
      : door_(door),
        window_(window),
        pool_(pool),
        garage_(garage),
        garden_(garden) {}

 private:
  uint32_t door_;
  uint32_t window_;
  uint32_t pool_;
  uint32_t garage_;
  uint32_t garden_;
};

// Builder
class Builder {
 public:
  virtual Builder& BuildDoor(const uint32_t door) = 0;
  virtual Builder& BuildWindow(const uint32_t window) = 0;
  virtual Builder& BuildPool(const uint32_t pool) = 0;
  virtual Builder& BuildGarage(const uint32_t garage) = 0;
  virtual Builder& BuildGarden(const uint32_t garden) = 0;
  virtual House Create() = 0;
};

// Concrete Builder for House
class HouseBuilder : public Builder {
 public:
  Builder& BuildDoor(const uint32_t door) override {
    door_ = door;
    return *this;
  }
  Builder& BuildWindow(const uint32_t window) override {
    window_ = window;
    return *this;
  }
  Builder& BuildPool(const uint32_t pool) override {
    pool_ = pool;
    return *this;
  }
  Builder& BuildGarage(const uint32_t garage) override {
    garage_ = garage;
    return *this;
  }
  Builder& BuildGarden(const uint32_t garden) override {
    garden_ = garden;
    return *this;
  }
  House Create() override {
    return House(door_, window_, pool_, garage_, garden_);
  }

 private:
  uint32_t door_;
  uint32_t window_;
  uint32_t pool_;
  uint32_t garage_;
  uint32_t garden_;
};

// Director
class Director {
 public:
  House CreateNormalHouse(std::unique_ptr<Builder>&& builder) {
    return builder->BuildDoor(1).BuildWindow(2).Create();
  }

  House CreatePoolHouse(std::unique_ptr<Builder>&& builder) {
    return builder->BuildDoor(3)
        .BuildWindow(3)
        .BuildGarden(1)
        .BuildGarage(1)
        .BuildPool(1)
        .Create();
  }
};

int main() {
  Director house_maker;

  House normal_house =
      house_maker.CreateNormalHouse(std::make_unique<HouseBuilder>());
  House pool_house =
      house_maker.CreatePoolHouse(std::make_unique<HouseBuilder>());

  return 0;
}