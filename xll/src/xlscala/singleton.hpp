#ifndef AETHER_HPP_SINGLETON
#define AETHER_HPP_SINGLETON


namespace xlscala {

    template <typename T>
    class Singleton {
    public:
        static T& instance();

    protected:
       Singleton() {}
       void init() {}

    private:
       Singleton(const Singleton&) {}
    };

    template <typename T>
    T& Singleton<T>::instance() {
        static std::shared_ptr<T> instance_;
        if (!instance_) {
            instance_ = std::shared_ptr<T>(new T);
            instance_->init();
        }
        return *instance_;
    }

}

#endif
