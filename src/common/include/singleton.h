#include <mutex>

template<typename T>
class Singleton {
public:
    static T* getInstance() {
        if (instance_ == nullptr) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (instance_ == nullptr) {
                instance_ = new T();
            }
        }
        return instance_;
    }
private:
    Singleton() {}
    static T* instance_;
    static std::mutex mutex_;
};

template<typename T>
T* Singleton<T>::instance_ = nullptr;

template<typename T>
std::mutex Singleton<T>::mutex_;
