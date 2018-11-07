// Copyright 2018 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_OBJECTS_SLOTS_INL_H_
#define V8_OBJECTS_SLOTS_INL_H_

#include "src/objects/slots.h"

#include "src/base/atomic-utils.h"
#include "src/objects.h"
#include "src/objects/maybe-object.h"

namespace v8 {
namespace internal {

void ObjectSlot::store(Object* value) {
  *reinterpret_cast<Address*>(address()) = value->ptr();
}

Object* ObjectSlot::Relaxed_Load() const {
  Address object_ptr = base::AsAtomicWord::Relaxed_Load(location());
  return reinterpret_cast<Object*>(object_ptr);
}

Object* ObjectSlot::Relaxed_Load(int offset) const {
  Address object_ptr = base::AsAtomicWord::Relaxed_Load(
      reinterpret_cast<Address*>(address() + offset * kPointerSize));
  return reinterpret_cast<Object*>(object_ptr);
}

void ObjectSlot::Relaxed_Store(int offset, Object* value) const {
  Address* addr = reinterpret_cast<Address*>(address() + offset * kPointerSize);
  base::AsAtomicWord::Relaxed_Store(addr, value->ptr());
}

Object* ObjectSlot::Release_CompareAndSwap(Object* old, Object* target) const {
  Address result = base::AsAtomicWord::Release_CompareAndSwap(
      location(), old->ptr(), target->ptr());
  return reinterpret_cast<Object*>(result);
}

MaybeObject MaybeObjectSlot::operator*() {
  return MaybeObject(*reinterpret_cast<Address*>(address()));
}

void MaybeObjectSlot::store(MaybeObject value) {
  *reinterpret_cast<Address*>(address()) = value.ptr();
}

MaybeObject MaybeObjectSlot::Relaxed_Load() const {
  Address object_ptr = base::AsAtomicWord::Relaxed_Load(location());
  return MaybeObject(object_ptr);
}

void MaybeObjectSlot::Release_CompareAndSwap(MaybeObject old,
                                             MaybeObject target) const {
  base::AsAtomicWord::Release_CompareAndSwap(location(), old.ptr(),
                                             target.ptr());
}

HeapObjectReference HeapObjectSlot::operator*() {
  return HeapObjectReference(*reinterpret_cast<Address*>(address()));
}
void HeapObjectSlot::store(HeapObjectReference value) {
  *reinterpret_cast<Address*>(address()) = value.ptr();
}

}  // namespace internal
}  // namespace v8

#endif  // V8_OBJECTS_SLOTS_INL_H_
