/**
 * composite is a structural design pattern.
 * 1. 모든 객체들이 구현해야하는 공통 인터페의스를 정의 (Component)
 * 2. Component 를 구현한 단일 객체 -> 더 이상 하위 객체를 가질 수 없음 (Leaf)
 * 3. Component 를 구현한 복합 객체 -> 하위 객체를 가질 수 있음 (Composite)
 * (맴버 변수로 하위 객체들을 list 로 관리)
 * 계층적(트리) 형식의 데이터를 다룰때 사용. -> 객체를 트리 구조로 구성하여,
 * 단일 객체와 복합 객체를 동일한 인터페이스를 사용하여 다룰 수 있음.
 */
#include <deque>
#include <iostream>
#include <memory>
#include <string>

// Component
class FileSystem {
 protected:
  FileSystem() = default;

 public:
  virtual ~FileSystem() = default;

  virtual void Print(const std::string& str) const = 0;
  virtual int32_t GetSize() const = 0;

  virtual void Add(std::shared_ptr<FileSystem> node) {}
};

// Composite
class Folder final : public FileSystem {
 public:
  Folder(const std::string& name) : name_(name) {}

  void Add(std::shared_ptr<FileSystem> node) { list_.push_back(node); }

  void Print(const std::string& str) const override {
    int32_t size = GetSize();

    std::cout << str << "ㄴ" << name_ << " (" << size << "kb)" << std::endl;

    for (const auto& node : list_) node->Print(str + "    ");
  }

  int32_t GetSize() const override {
    int32_t sum = 0;
    for (const auto& node : list_) sum += node->GetSize();

    return sum;
  }

 private:
  std::deque<std::shared_ptr<FileSystem>> list_;
  std::string name_;
};

// Leaf
class File : public FileSystem {
 public:
  File(const std::string& name, const int32_t size)
      : name_(name), size_(size) {}

  void Print(const std::string& str) const override {
    std::cout << str << "ㄴ" << name_ << " (" << size_ << "kb)" << std::endl;
  }

  int32_t GetSize() const override { return size_; }

 private:
  std::string name_;
  int32_t size_;
};

int main() {
  Folder root("root");
  std::shared_ptr<Folder> sub1 = std::make_shared<Folder>("sub1");
  std::shared_ptr<Folder> sub2 = std::make_shared<Folder>("sub2");

  root.Add(sub1);
  root.Add(sub2);

  root.Add(std::make_shared<File>("file1", 10));

  sub1->Add(std::make_shared<File>("file11", 10));
  sub1->Add(std::make_shared<File>("file12", 10));

  sub2->Add(std::make_shared<File>("file21", 10));

  root.Print("");

  return 0;
}