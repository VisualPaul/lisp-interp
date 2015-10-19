// -*-c++-*-
#ifndef GC_H
#define GC_H

#include <type_traits>
#include <vector>
#include <cstddef>

class Object;
class GC;

/* For internal usage only */
class GCObjectPtrBase {
protected:
    Object *_pointer;
public:
    GCObjectPtrBase(Object *obj);
    ~GCObjectPtrBase();
    friend class GC;
};

/* Stack allocation only. Don't even try to use this in heap */
template <typename T>
class GCObjectPtr : GCObjectPtrBase {

public:
    GCObjectPtr(T *x) : GCObjectPtrBase(x) {
        static_assert(std::is_base_of<Object, T>::value,
                      "GCObjectPtr can point to Object subclasses only");
    }
    T &operator *() const {
        return *getNormalPointer();
    }
    T *getNormalPointer() const {
        return static_cast<T *>(_pointer);
    }
    T *operator ->() const {
        return getNormalPointer();
    }
    GCObjectPtr<T> &operator = (T *pointer);
    
    void *operator new(size_t s) = delete;
    void operator delete(void *p) = delete;
};

class GC {
public:
    static inline GC *getGC();
    void collectGarbage();
    void maybeCollectGarbage() {
        collectGarbage();
    }
private:
    GC() {}
    std::vector<GCObjectPtrBase *> _gcBases;
    std::vector<Object *> _objects;
    static GC *_object;
    friend class GCObjectPtrBase;
    friend class Object;
};

GC *GC::getGC() {
    if (_object == nullptr)
        _object = new GC;
    return _object;
}

template <typename T>
GCObjectPtr<T> &GCObjectPtr<T>::operator = (T *pointer) {
    _pointer = pointer;
    GC::getGC()->maybeCollectGarbage();
    return *this;
}

#endif
