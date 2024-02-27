/**
 * State Method is a behavioral design pattern.
 * 1. 상태 인터페이스 정의 State
 * 2. 각 상태를 구체화하여 구현 ConcreteState
 * 3. 알고리즘 인터페이스를 맴버변수로 사용하여 실행하는 주체 구현 Context
 * 내부 상태가 변할때 -> 행동 변할 수 있도록 하는 패턴.
 */
#include <iostream>
#include <memory>

// State
class MotionState {
 public:
  virtual ~MotionState() {}
  virtual void Action() = 0;
};

// ConcreteState
class Run : public MotionState {
 public:
  void Action() override { std::cout << "running.." << std::endl; }
};

// ConcreteState
class Jump : public MotionState {
 public:
  void Action() override { std::cout << "jumping.." << std::endl; }
};

// ConcreteState
class Duck : public MotionState {
 public:
  void Action() override { std::cout << "ducking.." << std::endl; }
};

// Context
class Mario {
 public:
  void SetState(std::unique_ptr<MotionState>&& state) {
    state_ = std::move(state);
  }
  void Move() {
    if (state_) state_->Action();
  }

 private:
  std::unique_ptr<MotionState> state_ = nullptr;
};

/*
int main() {
  Mario mario;

  mario.SetState(std::make_unique<Run>());
  mario.Move();

  mario.SetState(std::make_unique<Jump>());
  mario.Move();

  mario.SetState(std::make_unique<Duck>());
  mario.Move();
}
*/