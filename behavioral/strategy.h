/**
 * Strategy is a behavioral design pattern.
 * 런타임에 알고리즘을 동적으로 교체해야할 때 사용.
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

int main() {
  Renderer renderer(std::make_unique<Yuv420ToRgb24>());
  renderer.Draw();

  renderer.SetConverter(std::make_unique<Yuv422ToRgb24>());
  renderer.Draw();
}