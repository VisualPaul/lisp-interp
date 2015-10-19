#include "gc.h"
#include "object.h"
#include <cstdio>

GCObjectPtrBase::GCObjectPtrBase(Object *obj) {
    _pointer = obj;
    GC *gc = GC::getGC();
    gc->_gcBases.push_back(this);
    gc->maybeCollectGarbage();
}

GCObjectPtrBase::~GCObjectPtrBase() {
    GC *gc = GC::getGC();
    gc->_gcBases.pop_back();
    gc->maybeCollectGarbage();
}

void GC::collectGarbage() {
#ifdef GC_DEBUG
    fputs("gc: garbage collection started\n", stderr);
#endif

    for (auto x : _gcBases)
        if (x->_pointer)
            x->_pointer->gcMark();
    Symbol::gcMarkSymbols();
    auto mit = std::partition(_objects.begin(), _objects.end(), [] (Object *x) {
            return x->_gcMark;
        });
#ifdef GC_DEBUG
    fprintf(stderr, "%zd objects are scheduled for deletion out of %zd\n", std::distance(mit, _objects.end()), _objects.size());
#endif
    for (auto it = mit; it != _objects.end(); ++it)
        (**it).free();
    _objects.erase(mit, _objects.end());
    for (Object *x : _objects)
        x->_gcMark = false;
}

GC *GC::_object;
