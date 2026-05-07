#ifndef SINGLETON_H
#define SINGLETON_H

#include "global.h"

template <typename T>
class Singleton {
protected:
    Singleton() = default;
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator=(const Singleton<T>&) = delete;

    static std::shared_ptr<T> instance_;
public:
    static std::shared_ptr<T> GetInstance() {
        // 只会调用一次
        static std::once_flag s_flag;
        std::call_once(s_flag, [&](){
            // CRTP继承模版单例类的子类会设置为 private，make_shared无法访问私有构造函数
            instance_ = std::shared_ptr<T>(new T);
        });

        return instance_;
    }

    void PrintAddress() {
        std::cout << instance_.get() << std::endl;
    }

    ~Singleton() {
        std::cout << "This is singleton destruct" << std::endl;
    }
};

// 模板类static实例要放在.h中
template<typename T>
std::shared_ptr<T> Singleton<T>::instance_ = nullptr;

#endif // SINGLETON_H
