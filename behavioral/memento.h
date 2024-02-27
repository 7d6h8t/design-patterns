/**
 * Memento is a behavioral design pattern.
 * 1. Originator의 특정 상태를 저장하기 위한 클래스 정의 Memento
 * 2. Memento 를 생성하고, 현재 상태를 저장하는 클래스 정의 Originator
 * 3. Memento 의 상태를 관리하고, Originator 의 상태를 복원하는 클래스 Caretaker
 * 객체의 상태를 저장 + 나중에 이 상태를 복원(롤백) 할때 사용함.
 * 상태 스냅샷이 필요한 경우.
 */

#include <deque>
#include <iostream>
#include <string>

// Memento: Originator 의 특정 상태를 저장하는 클래스
class Memento {
 public:
  explicit Memento(const std::string& state) : state_(state) {}
  std::string GetState() const { return state_; }

 private:
  std::string state_;
};

// Originator: 상태를 저장하고 복원하는 클래스
class TextEditor {
 public:
  void Typing(const std::string& text) { text_ += text; }
  void Print() { std::cout << text_ << std::endl; }

  // Create Memento
  Memento Save() const { return Memento(text_); }

  // Set Memento
  void Restore(const Memento& memento) { text_ = memento.GetState(); }

 private:
  std::string text_;
};

// Caretaker: Originator를 관리하는 클래스
class UndoManager {
 public:
  void SaveState(const Memento& memento) { undo_stack_.push_back(memento); }
  Memento GetState(const uint32_t i) { return undo_stack_[i]; }

 private:
  std::deque<Memento> undo_stack_;
};

/*
int main() {
  TextEditor editor;
  UndoManager undo_manager;

  editor.Typing("Hello, ");
  undo_manager.SaveState(editor.Save());

  editor.Typing("world!");
  undo_manager.SaveState(editor.Save());

  editor.Typing(" Goodbye!");
  editor.Print();

  editor.Restore(undo_manager.GetState(1));
  editor.Print();

  return 0;
}
*/