/**
 * Command is a behavioral design pattern.
 * 1. 기능을 묶는 인터페이스 클래스 정의 Command.
 * 2. 원하는 기능들을 각각 클래스로 만들어 캡슐화 ConcreateCommand
 * 객체의 행위(메소드) 를 클래스로 -> 캡슐화 목적.
 * 호출자와 호출 하려는 객체의 의존성을 제거할때 사용.
 * 기능을 추가하거나 수정할때, 기존 코드는 그대로두고 ConcreteCommand 를
 * 추가하면 되기에 확장성이 높음
 */

#include <iostream>
#include <string>

// Command
class Command {
 public:
  virtual ~Command() {}
  virtual void Execute() = 0;
};

// Recevicer
class Lamp {
 public:
  void TurnOn() { std::cout << "lamp turn on" << std::endl; }
  void TurnOff() { std::cout << "lamp turn off" << std::endl; }
};

// Recevicer
class Heater {
 public:
  void TurnOn() { std::cout << "heater turn on" << std::endl; }
  void TurnOff() { std::cout << "heater turn off" << std::endl; }
};

// ConcreteCommand
class LampOnCommand : public Command {
 public:
  LampOnCommand(Lamp* lamp) : lamp_(lamp) {}
  void Execute() override { lamp_->TurnOn(); }

 private:
  Lamp* lamp_;
};

// ConcreteCommand
class LampOffCommand : public Command {
 public:
  LampOffCommand(Lamp* lamp) : lamp_(lamp) {}
  void Execute() override { lamp_->TurnOff(); }

 private:
  Lamp* lamp_;
};

// ConcreteCommand
class HeaterOnCommand : public Command {
 public:
  HeaterOnCommand(Heater* heater) : heater_(heater) {}
  void Execute() override { heater_->TurnOn(); }

 private:
  Heater* heater_;
};

// ConcreteCommand
class HeaterOffCommand : public Command {
 public:
  HeaterOffCommand(Heater* heater) : heater_(heater) {}
  void Execute() override { heater_->TurnOff(); }

 private:
  Heater* heater_;
};

// invoker
class OKGoogle {
 public:
  void SetCommand(Command* command) { command_ = command; }
  void talk() { command_->Execute(); }

 private:
  Command* command_;
};

int main() {
  Heater heater;
  Lamp lamp;

  Command* heater_on_command = new HeaterOnCommand(&heater);
  Command* lamp_on_command = new LampOnCommand(&lamp);

  OKGoogle ok_google;
  ok_google.SetCommand(heater_on_command);
  ok_google.talk();

  ok_google.SetCommand(lamp_on_command);
  ok_google.talk();

  return 0;
}