/**
 * Iterator is a behavioral design pattern.
 * 1. 데이터 요소에 접근 및 순회 역할의 인터페이스 정의 Iterator
 * 2. Iterator 를 구현하여 실제 순회 방법 구현 ConcreteIterator
 * 3. 데이터 집합 인터페이스 정의 Aggregate (Iterator 반환 함수 필요)
 * 4. Aggregate 를 구현하여 실제 데이터 집합 정의 ConcreteAggregate
 * 데이터 집합에 대해 일관된 방식의 순차적 접근이 필요할때 사용.
 */
#include <iostream>

// Iterator
template <typename T>
class Iterator {
 public:
  virtual bool HasNext() = 0;
  virtual T Next() = 0;
};

// ConcreteIterator
template <typename T>
class ArrayIterator : public Iterator<T> {
 public:
  ArrayIterator(T* arr) : arr_(arr) {}
  bool HasNext() override { return next_index_ < 5 /*arr_.size*/; }
  T Next() override { return arr_[next_index_++]; }

 private:
  T* arr_;
  int32_t next_index_ = 0;
};

template <typename T>
class Aggregate {
  virtual Iterator<T>* Iterator() = 0;
};

template <typename T>
class ConcreateAggregate : public Aggregate<T> {
 public:
  ConcreateAggregate(const int32_t size) { arr_ = new T[size]; }
  void Add(T elem) { arr_[index_++] = elem; }
  Iterator<T>* Iterator() override { return new ArrayIterator<T>(arr_); }

 private:
  T* arr_;
  int32_t index_ = 0;
};

int main() {
  ConcreateAggregate<int32_t> aggregate(5);
  aggregate.Add(1);
  aggregate.Add(2);
  aggregate.Add(3);
  aggregate.Add(4);
  aggregate.Add(5);

  Iterator<int32_t>* iter = aggregate.Iterator();

  while (iter->HasNext()) {
    std::cout << iter->Next() << " -> ";
  }
}