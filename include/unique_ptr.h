#ifndef UNIQUE_PTR
#define UNIQUE_PTR

template<typename T>
class UniquePtr {

private:
    T *_p;

public:
    UniquePtr();

    explicit UniquePtr(T *p);

    UniquePtr(const UniquePtr &ptr) = delete;

    UniquePtr(UniquePtr &&ptr) noexcept;

    ~UniquePtr();

    UniquePtr &operator=(const UniquePtr &ptr) = delete;

    UniquePtr &operator=(UniquePtr &&ptr) noexcept;

    T *get() const;

    T *release();

    void reset();

    void reset(T *p);

    T &operator*() const;

    T *operator->() const;

    explicit operator bool() const;

};


template<typename T, typename... Args>
UniquePtr<T> make_unique(Args &&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}


template<typename T>
UniquePtr<T>::UniquePtr() : _p(nullptr) {
}

template<typename T>
UniquePtr<T>::UniquePtr(T *p) : _p(p) {
}

template<typename T>
UniquePtr<T>::UniquePtr(UniquePtr &&ptr) noexcept : _p(ptr._p) {
    ptr._p = nullptr;
}

template<typename T>
T *UniquePtr<T>::get() const {
    return _p;
}

template<typename T>
T *UniquePtr<T>::release() {
    T *temp = _p;
    _p = nullptr;
    return temp;
}

template<typename T>
void UniquePtr<T>::reset() {
    delete _p;
    _p = nullptr;
}

template<typename T>
void UniquePtr<T>::reset(T *p) {
    delete _p;
    _p = p;
}

template<typename T>
T &UniquePtr<T>::operator*() const {
    return *_p;
}

template<typename T>
T *UniquePtr<T>::operator->() const {
    return _p;
}

template<typename T>
UniquePtr<T>::operator bool() const {
    return _p != nullptr;
}

template<typename T>
UniquePtr<T> &UniquePtr<T>::operator=(UniquePtr &&ptr) noexcept {
    if (this != &ptr) {
        delete _p;
        _p = ptr._p;
        ptr._p = nullptr;
    }
    return *this;
}

template<typename T>
UniquePtr<T>::~UniquePtr() {
    delete _p;
}


#endif //UNIQUE_PTR