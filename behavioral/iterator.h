/**
 * Iterator is a behavioral design pattern.
 * 데이터 집합에 대해 순차적인 접근이 필요할때 사용.
 */
#include <iostream>

template <typename T>
class Iterator {
 public:
  virtual bool HasNext() = 0;
  virtual T Next() = 0;
};

template <typename T>
class ConcreteIterator : public Iterator<T> {
 public:
  ConcreteIterator(T* arr) : arr_(arr) {}
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
  Iterator<T>* Iterator() override { return new ConcreteIterator<T>(arr_); }

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