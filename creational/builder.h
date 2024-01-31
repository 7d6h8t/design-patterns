/**
 * Builder is a creational design pattern.
 * It makes sense to use the builder pattern only when youer products are quite
 * complex and require extensive configuration.
 * (GOF) 객체의 생성 알고리즘 + 조립 방법을 분리 -> 공정화를 구축
 */
#include <iostream>
#include <memory>

// Product : Director 가 Builder로 만들어낸 결과물.
class Data {
 public:
  Data(const std::string& name, const int32_t age) : name_(name), age_(age) {}

  std::string getName() { return name_; }
  int32_t getAge() { return age_; }

 private:
  std::string name_;
  int32_t age_;
};

// Builder : 빌더 추상 클래스
class Builder {
 public:
  Builder(const Data& data) : data_(data) {}

  // Data 객체의 데이터들을 원하는 형태의 문자열 포맷을 해주는 메서드들 (머리 -
  // 중간 - 끝 형식)
  virtual std::string head() = 0;
  virtual std::string body() = 0;
  virtual std::string foot() = 0;

  // 상속한 자식 클래스에서 사용하도록 protected 접근제어자 지정
 protected:
  Data data_;
};

// ConcreteBuilder : Builder의 구현체. Product 생성 담당.
// Data 데이터들을 평범한 물자열로 변환해주는 빌더
class PlainTextBuilder : public Builder {
 public:
  PlainTextBuilder(const Data& data) : data_(data) {}

  std::string head() override { return ""; }
  std::string body() override {
    std::string str;

    str.append("Name: ");
    str.append(data_.getName());
    str.append(", Age: ");
    str.append(std::to_string(data_.getAge()));

    return str;
  }
  std::string foot() override { return ""; }
};

// ConcreteBuilder : Builder의 구현체. Product 생성 담당.
// Data 데이터들을 JSON 형태의 문자열로 변환해주는 빌더
class JsonBuilder : public Builder {
 public:
  JsonBuilder(const Data& data) : data_(data) {}

  std::string head() override { return "{\n"; }
  std::string body() override {
    std::string str;

    str.append("\t\"Name\" : ");
    str.append("\"" + data_.getName() + "\", \n");
    str.append("\t\"Age\" : ");
    str.append(std::to_string(data_.getAge()));

    return str;
  }
  std::string foot() override { return "\n}"; }
};

// ConcreteBuilder : Builder의 구현체. Product 생성 담당.
// Data 데이터들을 XML 형태의 문자열로 변환해주는 빌더
class XMLBuilder : public Builder {
 public:
  XMLBuilder(const Data& data) : data_(data) {}

  std::string head() override {
    std::string str;

    str.append("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
    str.append("<DATA>\n");

    return str;
  }
  std::string body() override {
    std::string str;

    str.append("\t<NAME>");
    str.append(data_.getName());
    str.append("</NAME>");
    str.append("\n\t<AGE>");
    str.append(std::to_string(data_.getAge()));
    str.append("</AGE>");

    return str;
  }

  std::string foot() override { return "\n</DATA>"; }
};

// Director : Builder에서 제공하는 메서드들을 사용해 정해진 순서대로 Product를
// 생성하는 프로세스 정의. 각 문자열 포맷 빌드 과정을 템플릿화 시킨 디렉터
class Director {
 public:
  Director(std::unique_ptr<Builder>&& pBuilder)
      : pBuilder_(std::move(pBuilder)) {}

  // 일종의 빌드 템플릿 메서드
  std::string build() {
    std::string str;

    // 빌더 구현체에서 저으이한 생성 알고리즘이 실행됨
    str.append(pBuilder_->head());
    str.append(pBuilder_->body());
    str.append(pBuilder_->foot());

    return str;
  }

 private:
  std::unique_ptr<Builder> pBuilder_;
};

/*
int main() {
  // 1. 포맷할 데이터 생성
  Data data("홍길동", 44);

  // 2. 일반 텍스트로 포맷하여 출력
  Director text_director(std::make_unique<PlainTextBuilder>(data));
  std::string text = text_director.build();
  std::cout << text << std::endl;

  // 3. JSON 형식으로 포맷하여 출력
  Director json_director(std::make_unique<JsonBuilder>(data));
  std::string json = json_director.build();
  std::cout << json << std::endl;

  // 4. XML 형식으로 포맷하여 출력
  Director xml_director(std::make_unique<XMLBuilder>(data));
  std::string xml = xml_director.build();
  std::cout << xml << std::endl;

  return 0;
}
*/