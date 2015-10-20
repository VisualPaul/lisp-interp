#include "gc.h"
#include "object.h"
#include <cstdio>
#include <cassert>

GCObject::GCObject() : _gcMark(false) {
    GC *gc = GC::getGC();
    gc->_objects.push_back(this);
}

GCObject::~GCObject() {}

void GCObject::gcMark() {
    if (_gcMark)
        return;
    _gcMark = true;
    gcMarkChildren();
}

void GCObject::gcMarkChildren() {
}

GCObjectPtrBase::GCObjectPtrBase(GCObject *obj) {
    _pointer = obj;
    GC *gc = GC::getGC();
    gc->_gcBases.push_back(this);
    gc->maybeCollectGarbage();
}

GCObjectPtrBase::~GCObjectPtrBase() {
    GC *gc = GC::getGC();
    gc->_gcBases.pop_back();
}

GC::GC() :_gcForbidden(false) {}

void GC::collectGarbage() {
    if (_gcForbidden)
        return;
    static int gcGen = 0;
    
#ifdef GC_DEBUG
    fprintf(stderr, "gc: garbage collection started, generation %d\n", gcGen);
#endif
    ++gcGen;
    for (auto x : _gcBases)
        if (x->_pointer)
            x->_pointer->gcMark();
    Symbol::gcMarkSymbols();
    auto mit = std::partition(_objects.begin(), _objects.end(), [] (GCObject *x) {
            return x->_gcMark;
        });
#ifdef GC_DEBUG
    fprintf(stderr, "%zd objects are scheduled for deletion out of %zd\n",
            std::distance(mit, _objects.end()), _objects.size());
    if (mit != _objects.end()) {
        for (auto it = mit; it != _objects.end(); ++it) {
            fprintf(stderr, "Dead %p: %s\n", *it, (**it).gcRepr().c_str());
        }
    }
#endif
    for (auto it = mit; it != _objects.end(); ++it)
        (**it).free();
    _objects.erase(mit, _objects.end());
    for (GCObject *x : _objects) {
        x->_gcMark = false;
    }
}

GC *GC::_object;
