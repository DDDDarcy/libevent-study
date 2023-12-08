#pragma once



#include <cstddef>
#include <memory>
#include <mutex>
template <class Derived>
class Singleton
{
public:
    //static methods
    template <typename... Args>
    static inline Derived &Getinstance(Args && ...args)
    {
        return *GetInstancePointer(std::forward<Args>(args)...);
    }

    template <typename... Args>
    static inline std::shared_ptr<Derived> GetInstancePointer(Args &&...args)
    {
        static std::shared_ptr<Derived> instancePointer = CreateInstance(std::forward<Args>(args)...);
        return instancePointer;
    }
protected:
    Singleton(void) = default;

    Singleton(Singleton const & ) = default;

    Singleton(Singleton && ) = default;

    Singleton &operator=(Singleton const &) = default;
    
    Singleton &operator=(Singleton &&) = default;

    virtual ~Singleton(void) = default;

private:

    static std::shared_ptr<Derived> InstancePointer;

    static std::mutex Lock;

    template <typename... Args>
    static inline std::shared_ptr<Derived> CreateInstance(Args &&... args)
    {
        if(Singleton:: InstancePointer == nullptr)
        {
            std::lock_guard<std::mutex> lock(Lock);
            if(Singleton::InstancePointer == nullptr)
            {
                Singleton::InstancePointer = std::make_shared<Derived>(std::forward<Args>(args)...);
            }
        }
        return Singleton::InstancePointer;
    }
    
};

template <class Derived>
std::shared_ptr<Derived> InstancePointer = nullptr;

template <class Derived>
std::mutex Singleton<Derived>::Lock;