/**
 * singleton is a creational design pattern.
 * 1. 하나의 객체만 존재하도록 보장.
 * 2. 다른 코드에 대한 단일 액세스 지점을 제공.
 * 프로그램 내에서 유일한 인스턴스가 필요한 경우 사용.
 */
#include <iostream>
#include <string>

class Singleton {
 private:
  // 외부에서 new'/'delete' 연산자로 인한 '생성'/'제거' 를 방지하기 위해 항상
  // private 이어야함.
  Singleton(const std::string value) : value_(value) {}
  ~Singleton() {}

 public:
  // 유일한 인스턴스여야 하기에 '복사'/'할당'을 제거.
  Singleton(Singleton& other) = delete;
  void operator=(const Singleton&) = delete;

  static Singleton& GetInstance(const std::string& value) {
    static Singleton instance(value);
    return instance;
  };

  void doSomething() { std::cout << value_ << std::endl; }

  std::string value() const { return value_; }

 private:
  std::string value_;
};

/*
int main() {
  Singleton::GetInstance("hello").doSomething();
  Singleton::GetInstance("world").doSomething(); // 처음 만든 hello 출력
  return 0;
}
*/