/**
 * bridge is a structural design pattern.
 * 1. 동작을 처리하는 구현부 (Implementor)
 * 2. 확장을 위한 추상부를 분리 (Abstraction)
 * 3. 추상부(Abstraction) 의 복합 객체(맴버 변수)로 동작 구현부(Implementor) 를
 * 선언하여 사용.
 * 클래스 계층 구조가 복잡하고 확장 가능성이 높을 때 사용. (기존
 * 코드를 수정하지 않고 동작 추가 + 확장 가능)
 */
#include <iostream>
#include <memory>

// Implementor : 동작 인터페이스 정의
class Draw {
 protected:
  Draw() = default;

 public:
  virtual ~Draw() {}
  virtual void DrawRect() const = 0;
  virtual void DrawCircle() const = 0;
};

// Concrete Implementor : 동작 인터페이스 구현
class WinDraw final : public Draw {
 public:
  void DrawRect() const override {
    std::cout << "Windows 에서 rect draw" << std::endl;
  }
  void DrawCircle() const override {
    std::cout << "Windows 에서 circle draw" << std::endl;
  }
};

// Concrete Implementor : 동작 인터페이스 구현
class MacDraw final : public Draw {
 public:
  void DrawRect() const override {
    std::cout << "Mac 에서 rect draw" << std::endl;
  }
  void DrawCircle() const override {
    std::cout << "Mac 에서 circle draw" << std::endl;
  }
};

// Abstraction : 객체 확장을 위한 추상부 정의
class Shape {
 protected:
  Shape(std::unique_ptr<Draw>&& draw) : draw_(std::move(draw)) {}

 public:
  virtual ~Shape() {}

  virtual void DrawShape() const = 0;

 protected:
  std::unique_ptr<Draw> draw_;
};

// Concrete Abstraction : 추상부 구현
class Rectangle final : public Shape {
 public:
  Rectangle(std::unique_ptr<Draw>&& draw) : Shape(std::move(draw)) {
    std::cout << "Rectangle 생성" << std::endl;
  }

  void DrawShape() const override { draw_->DrawRect(); }
};

// Concrete Abstraction : 추상부 구현
class Circle final : public Shape {
 public:
  Circle(std::unique_ptr<Draw>&& draw) : Shape(std::move(draw)) {
    std::cout << "Circle 생성" << std::endl;
  }

  void DrawShape() const override { draw_->DrawCircle(); }
};

/*
int main() {
  std::unique_ptr<Shape> rect_shape_mac =
      std::make_unique<Rectangle>(std::make_unique<MacDraw>());
  rect_shape_mac->DrawShape();

  std::unique_ptr<Shape> circle_shape_mac =
      std::make_unique<Circle>(std::make_unique<MacDraw>());
  circle_shape_mac->DrawShape();

  std::unique_ptr<Shape> rect_shape_win =
      std::make_unique<Rectangle>(std::make_unique<WinDraw>());
  rect_shape_win->DrawShape();

  std::unique_ptr<Shape> circle_shape_win =
      std::make_unique<Circle>(std::make_unique<WinDraw>());
  circle_shape_win->DrawShape();

  return 0;
}
*/