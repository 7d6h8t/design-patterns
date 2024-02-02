/**
 * adaper is a structural design pattern.
 * 1. Adapter 클래스로 -> [기존 코드] + [새로운 코드] 묶어서 연결시킴.
 * 2. 묶어서 연결시키는 방법은 "복합 객체" 와 "다중 상속" 두가지 존재 ("복합
 * 객체" 방법이 권장됨 -> 더 모듈화 + 유연, 확장가능 하기 때문에).
 * 기존 코드를 수정하지 않고 + 다른 클래스와 호환할때 사용됨.
 */
#include <iostream>
#include <memory>

// Adaptee : 재사용 하고싶은 기존 코드
class Fahrenheit {
 public:
  Fahrenheit(const double temp) : temp_(temp) {}

  double GetTemp() const { return temp_; }

 private:
  double temp_;
};

// Target : 기존 코드를 이용해서 사용하려는 코드
class Celsius {
 public:
  virtual ~Celsius() = default;

  virtual double GetTemp() const = 0;
};

// Adapter (*복합 객체) : Adaptee -> Target 연결해주는 코드
class FahrenheitToCelsiusAdapter1 : public Celsius {
 public:
  FahrenheitToCelsiusAdapter1(const Fahrenheit& fahrenheit)
      : fahrenheit_(fahrenheit) {}

  double GetTemp() const override {
    double temp = (fahrenheit_.GetTemp() - 32) * 5.0 / 9.0;
    return temp;
  }

 private:
  Fahrenheit fahrenheit_;
};

// Adapter (다중 상속) : Adaptee -> Target 연결해주는 코드
class FahrenheitToCelsiusAdapter2 : public Celsius, public Fahrenheit {
 public:
  FahrenheitToCelsiusAdapter2(const double temp) : Fahrenheit(temp) {}
  double GetTemp() const override {
    double temp = (Fahrenheit::GetTemp() - 32) * 5.0 / 9.0;
    return temp;
  }
};

/*
int main() {
  Fahrenheit fahrenheit(32.0);

  std::cout << "temp : " << fahrenheit.GetTemp() << "° F" << std::endl;
  std::unique_ptr<Celsius> celsius1 =
      std::make_unique<FahrenheitToCelsiusAdapter1>(fahrenheit);

  std::cout << "temp : " << celsius1->GetTemp() << "° C" << std::endl;

  std::unique_ptr<FahrenheitToCelsiusAdapter2> celsius2 =
      std::make_unique<FahrenheitToCelsiusAdapter2>(62.0);

  std::cout << "temp : " << celsius2->GetTemp() << "° C" << std::endl;

  return 0;
}
*/