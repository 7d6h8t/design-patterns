/**
 * Chain of Responsibility is a behavioral design pattern.
 */
#include <memory>
#include <string>

class Handler {
 public:
  virtual Handler* SetNext(Handler* handler) {
    next_handler_ = handler;
    return handler;
  }

  virtual std::string Handle(const std::string& request) = 0;
  void run(const std::string& url) {
    Handle(url);

    if (next_handler_ != nullptr) next_handler_->run(url);
  }

 protected:
  Handler* next_handler_;
};
