// Copyright 2018 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_OBJECTS_HEAP_OBJECT_INL_H_
#define V8_OBJECTS_HEAP_OBJECT_INL_H_

#include "src/objects/heap-object.h"

#include "src/heap/heap-write-barrier-inl.h"

// Has to be the last include (doesn't have include guards):
#include "src/objects/object-macros.h"

namespace v8 {
namespace internal {

#define TYPE_CHECK_FORWARDER(Type)                       \
  bool ObjectPtr::Is##Type() const {                     \
    return reinterpret_cast<Object*>(ptr())->Is##Type(); \
  }
OBJECT_TYPE_LIST(TYPE_CHECK_FORWARDER)
HEAP_OBJECT_TYPE_LIST(TYPE_CHECK_FORWARDER)
#undef TYPE_CHECK_FORWARDER

#define TYPE_CHECK_FORWARDER(NAME, Name, name)           \
  bool ObjectPtr::Is##Name() const {                     \
    return reinterpret_cast<Object*>(ptr())->Is##Name(); \
  }
STRUCT_LIST(TYPE_CHECK_FORWARDER)
#undef TYPE_CHECK_FORWARDER

#define TYPE_CHECK_FORWARDER(Type, Value)                       \
  bool ObjectPtr::Is##Type(Isolate* isolate) const {            \
    return reinterpret_cast<Object*>(ptr())->Is##Type(isolate); \
  }                                                             \
  bool ObjectPtr::Is##Type(ReadOnlyRoots roots) const {         \
    return reinterpret_cast<Object*>(ptr())->Is##Type(roots);   \
  }                                                             \
  bool ObjectPtr::Is##Type() const {                            \
    return reinterpret_cast<Object*>(ptr())->Is##Type();        \
  }
ODDBALL_LIST(TYPE_CHECK_FORWARDER)
#undef TYPE_CHECK_FORWARDER

double ObjectPtr::Number() const {
  return reinterpret_cast<Object*>(ptr())->Number();
}

bool ObjectPtr::ToInt32(int32_t* value) const {
  return reinterpret_cast<Object*>(ptr())->ToInt32(value);
}

bool ObjectPtr::ToUint32(uint32_t* value) const {
  return reinterpret_cast<Object*>(ptr())->ToUint32(value);
}

void ObjectPtr::ShortPrint(FILE* out) {
  return reinterpret_cast<Object*>(ptr())->ShortPrint(out);
}

OBJECT_CONSTRUCTORS_IMPL(HeapObjectPtr, ObjectPtr)

#define TYPE_CHECK_FORWARDER(Type)                           \
  bool HeapObjectPtr::Is##Type() const {                     \
    return reinterpret_cast<HeapObject*>(ptr())->Is##Type(); \
  }
HEAP_OBJECT_TYPE_LIST(TYPE_CHECK_FORWARDER)
#undef TYPE_CHECK_FORWARDER

Map* HeapObjectPtr::map() const {
  return Map::cast(READ_FIELD(this, kMapOffset));
}

ObjectSlot HeapObjectPtr::map_slot() {
  return ObjectSlot(FIELD_ADDR(this, kMapOffset));
}

WriteBarrierMode HeapObjectPtr::GetWriteBarrierMode(
    const DisallowHeapAllocation& promise) {
  Heap* heap = Heap::FromWritableHeapObject(this);
  if (heap->incremental_marking()->IsMarking()) return UPDATE_WRITE_BARRIER;
  if (Heap::InNewSpace(*this)) return SKIP_WRITE_BARRIER;
  return UPDATE_WRITE_BARRIER;
}

ObjectSlot HeapObjectPtr::RawField(int byte_offset) const {
  return ObjectSlot(FIELD_ADDR(this, byte_offset));
}

}  // namespace internal
}  // namespace v8

#include "src/objects/object-macros-undef.h"

#endif  // V8_OBJECTS_HEAP_OBJECT_INL_H_
