/**
 * singleton is a creational design pattern,
 * which ensures that only one object of its kind exists
 * and provides a single point of access to it for any other code.
 */

#include <mutex>
#include <string>

class Singleton {
  /**
   * The Singleton's constructor/destructor should always be private to
   * prevent direct construction/desctruction calls with the `new`/`delete`
   * operator.
   */
 protected:
  Singleton(const std::string value) : value_(value) {}
  ~Singleton() {}

 public:
  // Singletons should not be cloneable.
  Singleton(Singleton& other) = delete;

  // Singletons should not be assignable.
  void operator=(const Singleton&) = delete;

  /**
   * This is the static method that controls the access to the singleton
   * instance. On the first run, it creates a singleton object and places it
   * into the static field. On subsequent runs, it returns the client existing
   * object stored in the static field.
   */
  static Singleton* GetInstance(const std::string& value) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr) pinstance_ = new Singleton(value);

    return pinstance_;
  };

  void SomeBusinessLogic() {
    // ...
  }

  std::string value() const { return value_; }

 protected:
  std::string value_;

 private:
  inline static Singleton* pinstance_ = nullptr;
  static std::mutex mutex_;
};