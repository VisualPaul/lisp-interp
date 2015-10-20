// -*-c++-*-
#ifndef GC_H
#define GC_H

#include <type_traits>
#include <vector>
#include <cstddef>
#include <string>
#include <cassert>
#include <cstdio>

class GC;

class GCObject {
public:
    GCObject();
    void gcMark();
    virtual std::string gcRepr() = 0;
protected:
    virtual void gcMarkChildren();
    virtual ~GCObject();
private:
    void free() {
        delete this;
    }
        
    friend class GC;
    bool _gcMark;
};

/* For internal usage only */
class GCObjectPtrBase {
protected:
    GCObject *_pointer;
public:
    GCObjectPtrBase(GCObject *obj);
    ~GCObjectPtrBase();
    friend class GC;
};

/* Stack allocation only. Don't even try to use this in heap */
template <typename T>
class GCObjectPtr : GCObjectPtrBase {
public:
    GCObjectPtr(T *x) : GCObjectPtrBase(x) {
        static_assert(std::is_base_of<GCObject, T>::value,
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
    operator void *() {
        return _pointer;
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
#ifdef GC_DEBUG
        collectGarbage();
#else
        if (_objects.size() > _targetObjectNumber)
            collectGarbage();
#endif
    }
private:
    GC();
    std::vector<GCObjectPtrBase *> _gcBases;
    std::vector<GCObject *> _objects;
    static GC *_object;
    std::size_t _targetObjectNumber;
    friend class GCObjectPtrBase;
    friend class GCObject;

    bool _gcForbidden; // hack
    friend class GCProtector;
};

class GCProtector {
    GC *gc;
    bool pstate;
public:
    GCProtector() : gc(GC::getGC()) {
        pstate = gc->_gcForbidden;
        gc->_gcForbidden = true;
    }
    ~GCProtector() {
        gc->_gcForbidden = pstate;
    }
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
