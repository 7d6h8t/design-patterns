/**
 * Memento is a behavioral design pattern.
 * 객체의 상태를 저장 + 나중에 이 상태를 복원(롤백) 할때 사용함.
 * 상태 스냅샷이 필요한 경우.
 */

#include <deque>
#include <iostream>
#include <string>

// Memento: 상태를 저장하는 클래스
class EditorMemento {
 public:
  explicit EditorMemento(const std::string& content) : content_(content) {}

  const std::string& GetContent() const { return content_; }

 private:
  std::string content_;
};

// Originator: 상태를 저장하고 복원하는 클래스
class TextEditor {
 public:
  void SetContent(const std::string& content) { content_ = content; }

  const std::string& GetContent() const { return content_; }

  EditorMemento CreateMemento() const { return EditorMemento(content_); }

  void RestoreMemento(const EditorMemento& memento) {
    content_ = memento.GetContent();
  }

 private:
  std::string content_;
};

// Caretaker: 상태를 저장하고 복원하는 클래스를 관리하는 클래스
class UndoRedoManager {
 public:
  void SaveState(const EditorMemento& memento) {
    undo_stack_.push_back(memento);
    current_idx = undo_stack_.size() - 1;
  }

  void Undo(TextEditor& editor) {
    if (CanUndo()) {
      --current_idx;
      editor.RestoreMemento(undo_stack_[current_idx]);
    }
  }

  void Redo(TextEditor& editor) {
    if (CanRedo()) {
      ++current_idx;
      editor.RestoreMemento(undo_stack_[current_idx]);
    }
  }

 private:
  bool CanUndo() const { return current_idx > 0; }
  bool CanRedo() const { return current_idx < undo_stack_.size() - 1; }

  std::deque<EditorMemento> undo_stack_;
  size_t current_idx = 0;
};

int main() {
  TextEditor editor;
  UndoRedoManager undo_redo_manager;

  // 초기 상태 저장
  undo_redo_manager.SaveState(editor.CreateMemento());

  // 사용자의 동작
  editor.SetContent("Hello, ");
  std::cout << "Current Content: " << editor.GetContent() << std::endl;

  // 상태 저장 및 undo 가능
  undo_redo_manager.SaveState(editor.CreateMemento());
  undo_redo_manager.Undo(editor);
  std::cout << "After Undo: " << editor.GetContent() << std::endl;

  // redo 가능
  undo_redo_manager.Redo(editor);
  std::cout << "After Redo: " << editor.GetContent() << std::endl;

  return 0;
}