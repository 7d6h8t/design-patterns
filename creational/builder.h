/**
 * Builder is a creational design pattern.
 * It makes sense to use the builder pattern only when youer products are quite
 * complex and require extensive configuration.
 */
#include <iostream>
#include <vector>

// Product 클래스: 빌더 패턴으로 생성할 객체의 대상 클래스
class Shape {
 public:
  void addPoint(int x, int y) { points_.emplace_back(std::make_pair(x, y)); }

  void draw() const {
    std::cout << "Drawing Shape with Points: ";
    for (const auto& point : points_)
      std::cout << "(" << point.first << ", " << point.second << ") ";

    std::cout << std::endl;
  }

 private:
  std::vector<std::pair<int, int>> points_;
};

// Builder 인터페이스: 제품을 생성하는 단계별 메서드를 정의
class ShapeBuilder {
 public:
  virtual void buildPoints() = 0;
  virtual Shape getResult() = 0;
};

class LineBuilder : public ShapeBuilder {
 public:
  void buildPoints() override {
    shape_.addPoint(0, 0);
    shape_.addPoint(0, 10);
  }

  Shape getResult() override { return shape_; }

 private:
  Shape shape_;
};

class RectBuilder : public ShapeBuilder {
 public:
  void buildPoints() override {
    shape_.addPoint(0, 0);
    shape_.addPoint(10, 0);
    shape_.addPoint(10, 10);
    shape_.addPoint(0, 10);
  }

  Shape getResult() override { return shape_; }

 private:
  Shape shape_;
};

class PolygonBuilder : public ShapeBuilder {
 public:
  void buildPoints() override {
    shape_.addPoint(0, 0);
    shape_.addPoint(10, 0);
    shape_.addPoint(5, 10);
  }

  Shape getResult() override { return shape_; }

 private:
  Shape shape_;
};

class PolylineBuilder : public ShapeBuilder {
 public:
  void buildPoints() override {
    shape_.addPoint(0, 0);
    shape_.addPoint(5, 5);
    shape_.addPoint(10, 0);
  }

  Shape getResult() override { return shape_; }

 private:
  Shape shape_;
};

// Director 클래스: 빌더 클래스를 사용하여 객체를 생성하는 클래스
class DrawingTool final {
 private:
  DrawingTool() = delete;
  ~DrawingTool() = delete;

 public:
  static Shape createShape(ShapeBuilder& builder) {
    builder.buildPoints();
    return builder.getResult();
  }
};

/*
int main() {
  // 클라이언트 코드
  LineBuilder lineBuilder;
  Shape lineShape = drawingTool::createShape(lineBuilder);
  lineShape.draw();

  RectBuilder rectBuilder;
  Shape rectShape = drawingTool::createShape(rectBuilder);
  rectShape.draw();

  PolygonBuilder polygonBuilder;
  Shape polygonShape = drawingTool::createShape(polygonBuilder);
  polygonShape.draw();

  PolylineBuilder polylineBuilder;
  Shape polylineShape = drawingTool::createShape(polylineBuilder);
  polylineShape.draw();

  return 0;
}
*/