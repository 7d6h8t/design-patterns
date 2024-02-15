/**
 * fryweight is a structural design pattern.
 */
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

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
      cache_[key] = new TreeSharedState(key, "mesh", "texture");
      return *cache_[key];
    }
  }

 private:
  static std::unordered_map<std::string, TreeSharedState*> cache_;
};

std::unordered_map<std::string, TreeSharedState*> Factory::cache_;

void CreateTree(const std::string& type, const double x, const double y) {
  TreeSharedState tree_state = Factory::GetInstance().GetSharedState(type);
  Tree tree(tree_state, x, y);

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
