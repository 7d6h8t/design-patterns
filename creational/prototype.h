/**
 * Prototype is a creational design pattern.
 * 1. 복사생성자 || cloneable 인터페이스(abstract clone() 함수) 를 사용하여
 * 구현가능.
 * 2. deep copy (맴버 변수로 포인터가 있는경우 동일한 주소 가르키지 않도록 새로
 * 할당하여 복사) 구현 필요. 어떤 객체에 대한 프로토타입을 만들어 놓고 그것을
 * 복사하는 패턴 (새로 생성하고 맴버변수 설정하는 작업을 생략함).
 * 객체를 복사하여 새로운 객체를 만들 때 사용.
 */
#include <iostream>
#include <memory>
#include <vector>

// cloneable 인터페이스
class Object {
 protected:
  virtual ~Object() = default;

 public:
  virtual void draw() const = 0;
  virtual std::unique_ptr<Object> clone() = 0;
};

class Rect : public Object {
 public:
  Rect(const int32_t x, const int32_t y) : x_(x), y_(y) {}

  void draw() const {
    std::cout << "draw rect : (" << x_ << ", " << y_ << ")" << std::endl;
  }

  std::unique_ptr<Object> clone() { return std::make_unique<Rect>(*this); }

 private:
  int32_t x_;
  int32_t y_;
};

class Triangle : public Object {
 public:
  Triangle(const int32_t x, const int32_t y) : x_(x), y_(y) {}

  void draw() const {
    std::cout << "draw triangle : (" << x_ << ", " << y_ << ")" << std::endl;
  }

  std::unique_ptr<Object> clone() { return std::make_unique<Triangle>(*this); }

 private:
  int32_t x_;
  int32_t y_;
};

/*
int main() {
  Triangle triangle(10, 5);
  Rect rect(25, 10);

  std::vector<std::unique_ptr<Object>> objects;
  objects.push_back(triangle.clone());
  objects.push_back(triangle.clone());
  objects.push_back(rect.clone());

  drawObjects(objects);

  return 0;
}

void drawObjects(const std::vector<std::unique_ptr<Object>>& objects) {
  for (const auto& object : objects) object->draw();
}
*/