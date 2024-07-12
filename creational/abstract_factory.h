/**
 * Abstract Factory is a creational design pattern.
 * 1. 객체 생성 역할을 factory 클래스로 몰아넣음 (의존성 제거).
 * 2. 확장성과 factory 클래스의 변동 억제(OCP 원칙)를 위해 서브클래스로 분리.
 * 3. 관련된 집합 객체의 생성을 한곳에 모아둠.
 * 호출 코드에서 생성 루틴에 대한 의존성 제거 + 확장성이 필요 + 관련 객체가 함께
 * 사용되어야 할때 사용됨.
 */

#include <iostream>
#include <memory>

// Abstract Product 1
class CheckBox {
 public:
  virtual void Check(bool enable) = 0;
};

// Abstract Product 2
class Button {
 public:
  virtual void Click(bool enable) = 0;
};

// Concrete Product 1
class WindowsCheckBox : public CheckBox {
 public:
  void Check(bool enable) override {}
};

class MacCheckBox : public CheckBox {
 public:
  void Check(bool enable) override {}
};

// Concrete Product 2
class WindowsButton : public Button {
 public:
  void Click(bool enable) override {}
};

class MacButton : public Button {
 public:
  void Click(bool enable) override {}
};

// Abstract Factory
class GUIFactory {
 public:
  virtual std::unique_ptr<CheckBox> CreateCheckBox() = 0;
  virtual std::unique_ptr<Button> CreateButton() = 0;
};

// Concrete Factory
class WindowsGUIFactory : public GUIFactory {
 public:
  std::unique_ptr<CheckBox> CreateCheckBox() override {
    return std::make_unique<WindowsCheckBox>();
  }
  std::unique_ptr<Button> CreateButton() override {
    return std::make_unique<WindowsButton>();
  }
};

class MacGUIFactory : public GUIFactory {
 public:
  std::unique_ptr<CheckBox> CreateCheckBox() override {
    return std::make_unique<MacCheckBox>();
  }
  std::unique_ptr<Button> CreateButton() override {
    return std::make_unique<MacButton>();
  }
};

int main() {
  std::unique_ptr<GUIFactory> factory = std::make_unique<WindowsGUIFactory>();
  auto button = factory->CreateButton();
  auto checkbox = factory->CreateCheckBox();

  factory = std::make_unique<MacGUIFactory>();
  button = factory->CreateButton();
  checkbox = factory->CreateCheckBox();
}