/**
 * Strategy is a behavioral design pattern.
 * 1. 알고리즘을 묶는 인터페이스 정의 Strategy
 * 2. 각 알고리즘을 구체화하여 구현 ConcreteStrategy
 * 3. 알고리즘 인터페이스를 맴버변수로 사용하여 실행하는 주체 구현 Context
 * 각 알고리즘을 캡슐화 -> 동적으로 교체 가능하게 만듬.
 * (알고리즘 변형이 빈번하게 필요한 경우)
 */
#include <iostream>
#include <memory>

// Strategy
class Converter {
 public:
  virtual ~Converter(){};
  virtual void Convert(/*image data*/) = 0;
};

// ConcreteStrategy
class Yuv420ToRgb24 : public Converter {
 public:
  void Convert(/*image data*/) override {
    std::cout << "convert yuv420 to rgb24" << std::endl;
  }
};

// ConcreteStrategy
class Yuv422ToRgb24 : public Converter {
 public:
  void Convert(/*image data*/) override {
    std::cout << "convert yuv422 to rgb24" << std::endl;
  }
};

// Context
class Renderer {
 public:
  explicit Renderer(std::unique_ptr<Converter>&& converter)
      : converter_(std::move(converter)) {}

  void SetConverter(std::unique_ptr<Converter>&& converter) {
    converter_ = std::move(converter);
  }

  void Draw(/*image data*/) const {
    if (converter_) converter_->Convert(/*image data*/);
  }

 private:
  std::unique_ptr<Converter> converter_;
};

/*
int main() {
  Renderer renderer(std::make_unique<Yuv420ToRgb24>());
  renderer.Draw();

  renderer.SetConverter(std::make_unique<Yuv422ToRgb24>());
  renderer.Draw();
}
*/