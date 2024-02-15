/**
 * fryweight is a structural design pattern.
 * 1. 공유 가능한 (변하지 않는) 데이터 ConcreteFlyweight.
 * 2. 공유 불가능한 데이터 ConcreteFlyweight(unshared) 로 분리하여
 * 3. ConcreteFlyweight 를 관리 (생성, 캐시) 하는 팩토리 FlyweightFactory 구현.
 * FlyweightFactory 로 데이터 경량화 (공유 데이터 캐시)를 관리함.
 * 메모리 최적화를 위해 상태가 변하지 않는 데이터를 재사용(공유) 할때 사용함.
 * (RAM 이 부족한 경우가 아니라면 거의 사용하지 않는 패턴임.)
 */
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

// ConcreteFlyweight
class TreeSharedState {
 public:
  TreeSharedState(const std::string& type, const std::string& mesh,
                  const std::string& texture)
      : type_(type), mesh_(mesh), texture_(texture) {}

  std::string GetType() const { return type_; }
  std::string GetMesh() const { return mesh_; }
  std::string GetTexture() const { return texture_; }

 private:
  std::string type_;
  std::string mesh_;
  std::string texture_;
};

// ConcreteFlyweight (unshared)
class Tree {
 public:
  Tree(const TreeSharedState& shared_state, const double x, const double y)
      : shared_state_(shared_state), x_(x), y_(y) {}

  double GetX() { return x_; }
  double GetY() { return y_; }

 private:
  TreeSharedState shared_state_;
  double x_;
  double y_;
};

// FlyweightFactory
class Factory final {
 public:
  static Factory& GetInstance() {
    static Factory instance;
    return instance;
  }

  TreeSharedState GetSharedState(const std::string& key) {
    if (cache_.find(key) != cache_.end())
      return *cache_.at(key);
    else {
      cache_[key] = std::make_unique<TreeSharedState>(key, "mesh", "texture");
      return *cache_[key];
    }
  }

 private:
  static std::unordered_map<std::string, std::unique_ptr<TreeSharedState>>
      cache_;
};

std::unordered_map<std::string, std::unique_ptr<TreeSharedState>>
    Factory::cache_;

/*
void CreateTree(const std::string& type, const double x, const double y) {
  Tree tree(Factory::GetInstance().GetSharedState(type), x, y);

  std::cout << "x:" << tree.GetX() << " y:" << tree.GetY() << " 위치에 " << type
            << " 나무 생성 완료" << std::endl;
}

int main() {
  CreateTree("Oak", 10, 20);
  CreateTree("Oak", 20, 30);
  CreateTree("Oak", 25, 40);
  CreateTree("Oak", 30, 20);

  CreateTree("Acacia", 1, 29);
  CreateTree("Acacia", 49, 2);

  return 0;
}
*/