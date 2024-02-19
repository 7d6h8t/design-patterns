/**
 * Command is a behavioral design pattern.
 */

#include <iostream>
#include <string>

class Command {
 public:
  virtual ~Command() {}
  virtual void Execute() const = 0;
};

class SimpleCommand : public Command {
 public:
  explicit SimpleCommand(const std::string& pay_load) : pay_load_(pay_load) {}
  void Execute() const override {
    std::cout << "SimpleCommand: See, I can do simple things like printing ("
              << pay_load_ << ")\n";
  }

 private:
  std::string pay_load_;
};

class Receiver {
 public:
  void DoSomething(const std::string& a) {
    std::cout << "Receiver: Working on (" << a << ".)\n";
  }
  void DoSomethingElse(const std::string& b) {
    std::cout << "Receiver: Also working on (" << b << ".)\n";
  }
};

class ComplexCommand : public Command {
 public:
  ComplexCommand(Receiver* receiver, const std::string& a, const std::string& b)
      : receiver_(receiver), a_(a), b_(b) {}

  void Execute() const override {
    std::cout << "ComplexCommand: Complex stuff should be done by a receiver "
                 "object.\n";
    receiver_->DoSomething(a_);
    receiver_->DoSomethingElse(b_);
  }

 private:
  Receiver* receiver_;
  std::string a_;
  std::string b_;
};

class Invoker {
 public:
  ~Invoker() {
    delete on_start_;
    delete on_finish_;
  }

  void SetOnStart(Command* command) { on_start_ = command; }
  void SetOnFinish(Command* command) { on_finish_ = command; }
  void DoSomethingImportant() {
    std::cout << "Invoker: Does anybody want something done before I begin?\n";
    if (on_start_) on_start_->Execute();

    std::cout << "Invoker: ...doing something really important...\n";
    std::cout << "Invoker: Does anybody want something done after I finish?\n";
    if (on_finish_) on_finish_->Execute();
  }

 private:
  Command* on_start_;
  Command* on_finish_;
};

int main() {
  Invoker* invoker = new Invoker;
  invoker->SetOnStart(new SimpleCommand("Say Hi!"));
  Receiver* receiver = new Receiver;
  invoker->SetOnFinish(
      new ComplexCommand(receiver, "Send email", "Save report"));
  invoker->DoSomethingImportant();

  delete invoker;
  delete receiver;

  return 0;
}