/**
 * Factory Medthod is a creational design pattern.
 * 1. 객체 생성 역할을 factory 클래스로 몰아넣음 (의존성 제거).
 * 2. 확장성과 factory 클래스의 변동 억제(OCP 원칙)를 위해 서브클래스로 분리.
 * 호출 코드에서 생성 루틴에 대한 의존성 제거 + 확장성이 필요한 경우 사용됨.
 */
#include <iostream>
#include <memory>
#include <string>

// Product
class Button {
  // do somthing
};

// Concrete Product
class WindowsButton : public Button {
  // do somthing
};

class MacButton : public Button {
  // do somthing
};

// Creator
class Dialog {
 public:
  void Open() {
    std::unique_ptr<Button> default_button = CreateButton();
    // do somthing
    // default_button->SetColor();
    // default_button->SetPosition();
    // etc..
  }

 protected:
  // Factory Method
  virtual std::unique_ptr<Button> CreateButton() = 0;
};

// Concrete Creator
class WindowsDialog : public Dialog {
 private:
  std::unique_ptr<Button> CreateButton() override {
    return std::make_unique<WindowsButton>();
  }
};

class MacDialog : public Dialog {
 private:
  std::unique_ptr<Button> CreateButton() override {
    return std::make_unique<MacButton>();
  }
};

int main() {
  std::unique_ptr<Dialog> dlg = std::make_unique<MacDialog>();
  dlg->Open();

  dlg = std::make_unique<WindowsDialog>();
  dlg->Open();
}