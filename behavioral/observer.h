/**
 * Observer is a behavioral design pattern.
 * 한 객체의 상태 변경 -> 의존하는 다른 객체 모두에 notify & update
 */
#include <algorithm>
#include <deque>
#include <iostream>
#include <random>

class Observer;
class WeatherAPI;

// Subject
class Subject {
 public:
  virtual ~Subject() {}

  virtual void RegisterObserver(Observer* o) = 0;
  virtual void RemoveObserver(Observer* o) = 0;
  virtual void Notify() = 0;
};

// Observer
class Observer {
 public:
  virtual void Display(WeatherAPI* api) = 0;
};

// ConcreteSubject
class WeatherAPI : public Subject {
 public:
  void measurementsChanged() {
    temp_ = rand();
    humidity_ = rand();
    pressure_ = rand();

    Notify();
  }

  void RegisterObserver(Observer* o) override { subscribers_.push_back(o); }
  void RemoveObserver(Observer* o) override {
    auto itr = std::find(subscribers_.begin(), subscribers_.end(), o);
    if (itr != subscribers_.end()) subscribers_.erase(itr);

    subscribers_.push_back(o);
  }

  void Notify() override {
    for (Observer* o : subscribers_) o->Display(this);
  }

 private:
  float temp_;
  float humidity_;
  float pressure_;
  std::deque<Observer*> subscribers_;
};

class KoreanUser : public Observer {
 public:
  KoreanUser(const std::string& name) : name_(name) {}
  void Display(WeatherAPI* api) { std::cout << std::endl; }

 private:
  std::string name_;
}