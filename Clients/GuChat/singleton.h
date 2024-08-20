#ifndef SINGLETON_H
#define SINGLETON_H
#include "global.h"
template<typename T>
class Singleton {
protected:
    Singleton() = default;
    Singleton(const Singleton<T>&) = delete;            // 拷贝构造
    Singleton& operator=(const Singleton<T>&) = delete; // 拷贝赋值
    // 类的static变量 一定要被初始化
    static std::shared_ptr<T> _instance;                // 智能指针 （自动回收）

public:
    static std::shared_ptr<T> GetInstance() {
        static std::once_flag s_flag;
        std::call_once(s_flag, [&]() {
            // 继承该模板类的子类的构造函数会设置成private make_shared无法访问私有的构造函数 所以使用new + 友元
            _instance = std::shared_ptr<T>(new T);
        });
        return _instance;
    }

    void PrintAddress() {
        std::cout << _instance.get() << std::endl;
    }

    ~Singleton() {
        std::cout << "this is singleton destruct" << std::endl;
    }
};

// 初始化
template<typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

#endif // SINGLETON_H
