/**
 * Command is a behavioral design pattern.
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
class Light {
 public:
  void TurnOn() { std::cout << "turn on" << std::endl; }
  void TurnOff() { std::cout << "turn off" << std::endl; }
};

// ConcreteCommand
class TurnOnCommand : public Command {
 public:
  TurnOnCommand(Light* Light) : light_(Light) {}
  void Execute() override { light_->TurnOn(); }

 private:
  Light* light_;
};

// ConcreteCommand
class TurnOffCommand : public Command {
 public:
  TurnOffCommand(Light* Light) : light_(Light) {}
  void Execute() override { light_->TurnOff(); }

 private:
  Light* light_;
};

// invoker
class RemoteControl {
 public:
  void SetCommand(Command* command) { command_ = command; }
  void PressButton() { command_->Execute(); }

 private:
  Command* command_;
};

int main() {
  Light light_;
  TurnOnCommand turn_on(&light_);
  TurnOffCommand turn_off(&light_);

  RemoteControl remote;
  remote.SetCommand(&turn_on);

  remote.PressButton();

  remote.SetCommand(&turn_off);
  remote.PressButton();

  return 0;
}