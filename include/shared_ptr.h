#ifndef SHARED_PTR
#define SHARED_PTR

template<typename T>
class SharedPtr {
private:
    T *_p;
    size_t *_count;
public:
    SharedPtr();

    explicit SharedPtr(T *p);

    SharedPtr(const SharedPtr &ptr);

    SharedPtr(SharedPtr &&ptr) noexcept;

    ~SharedPtr();

    SharedPtr &operator=(const SharedPtr &ptr);

    SharedPtr &operator=(SharedPtr &&ptr) noexcept;

    T *get() const;

    void reset();

    void reset(T *p);

    [[nodiscard]] size_t use_count() const;

    T &operator*() const;

    T *operator->() const;

    explicit operator bool() const;
};

template<typename T, typename... Args>
SharedPtr<T> make_shared(Args &&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}


template<typename T>
SharedPtr<T>::SharedPtr() : _p(nullptr), _count(nullptr) {
}

template<typename T>
SharedPtr<T>::SharedPtr(T *p) : _p(p), _count(new size_t(1)) {
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr &ptr) : _p(ptr._p), _count(ptr._count) {
    if (_count) {
        ++(*_count);
    }
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr &&ptr) noexcept : _p(ptr._p), _count(ptr._count) {
    ptr._p = nullptr;
    ptr._count = nullptr;
}

template<typename T>
SharedPtr<T>::~SharedPtr() {
    if (_count) {
        --(*_count);
        if (*_count == 0) {
            delete _p;
            delete _count;
            _p = nullptr;
            _count = nullptr;
        }
    }
}

template<typename T>
SharedPtr<T> &SharedPtr<T>::operator=(const SharedPtr &ptr) {
    if (this != &ptr) {
        if (_count) {
            --(*_count);
            if (*_count == 0) {
                delete _p;
                delete _count;
            }
        }
        _p = ptr._p;
        _count = ptr._count;
        if (_count) {
            ++(*_count);
        }
    }
    return *this;
}

template<typename T>
SharedPtr<T> &SharedPtr<T>::operator=(SharedPtr &&ptr) noexcept {
    if (this != &ptr) {
        if (_count) {
            --(*_count);
            if (*_count == 0) {
                delete _p;
                delete _count;
            }
        }
        _p = ptr._p;
        _count = ptr._count;
        ptr._p = nullptr;
        ptr._count = nullptr;
    }
    return *this;
}

template<typename T>
T *SharedPtr<T>::get() const {
    return _p;
}

template<typename T>
void SharedPtr<T>::reset() {
    if (_count) {
        --(*_count);
        if (*_count == 0) {
            delete _p;
            delete _count;
        }
    }
    _p = nullptr;
    _count = nullptr;
}

template<typename T>
void SharedPtr<T>::reset(T *p) {
    if (_count) {
        --(*_count);
        if (*_count == 0) {
            delete _p;
            delete _count;
        }
    }
    _p = p;
    _count = new size_t(1);
}

template<typename T>
size_t SharedPtr<T>::use_count() const {
    return _count ? *_count : 0;
}

template<typename T>
T *SharedPtr<T>::operator->() const {
    return _p;
}

template<typename T>
T &SharedPtr<T>::operator*() const {
    return *_p;
}

template<typename T>
SharedPtr<T>::operator bool() const {
    return _p != nullptr;
}


#endif //SHARED_PTR