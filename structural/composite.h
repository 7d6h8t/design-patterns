/**
 * composite is a structural design pattern.
 * 계층적(트리) 형식의 데이터를 다룰때 사용.
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

  virtual void print(const std::string& str) const = 0;
  virtual int32_t GetSize() const = 0;

  virtual void Add(std::unique_ptr<FileSystem>&& component) {}
};

// Composite
class Folder final : public FileSystem {
 public:
  Folder(const std::string& name) : name_(name) {}

  void Add(std::unique_ptr<FileSystem>&& node) {
    list_.push_back(std::move(node));
  }

  void print(const std::string& str) const override {
    int32_t size = GetSize();

    std::cout << str << "ㄴ" << name_ << " (" << size << "kb)" << std::endl;

    for (const auto& node : list_) node->print(str + "    ");
  }

  int32_t GetSize() const override {
    int32_t sum = 0;
    for (const auto& node : list_) sum += node->GetSize();

    return sum;
  }

 private:
  std::deque<std::unique_ptr<FileSystem>> list_;
  std::string name_;
};

// Leaf
class File : public FileSystem {
 public:
  File(const std::string& name, const int32_t size)
      : name_(name), size_(size) {}

  void print(const std::string& str) const override {
    std::cout << str << "ㄴ" << name_ << " (" << size_ << "kb)" << std::endl;
  }

  int32_t GetSize() const override { return size_; }

 private:
  std::string name_;
  int32_t size_;
};

int main() {
  Folder root("root");

  root.Add(std::make_unique<File>("file1", 10));

  std::unique_ptr<Folder> sub1 = std::make_unique<Folder>("sub1");
  sub1->Add(std::make_unique<File>("file11", 10));
  sub1->Add(std::make_unique<File>("file12", 10));

  std::unique_ptr<Folder> sub2 = std::make_unique<Folder>("sub2");
  sub2->Add(std::make_unique<File>("file21", 10));

  root.Add(std::move(sub1));
  root.Add(std::move(sub2));

  root.print("");

  return 0;
}