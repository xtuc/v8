// Copyright 2017 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/builtins/builtins-utils-gen.h"
#include "src/builtins/builtins.h"
#include "src/code-stub-assembler.h"

namespace v8 {
namespace internal {

class BigIntBuiltinsAssembler : public CodeStubAssembler {
 public:
  explicit BigIntBuiltinsAssembler(compiler::CodeAssemblerState* state)
      : CodeStubAssembler(state) {}
};

// https://tc39.github.io/proposal-bigint/#sec-to-big-int64
TF_BUILTIN(ToBigInt64, BigIntBuiltinsAssembler) {
  TVARIABLE(UintPtrT, var_low);
  TVARIABLE(UintPtrT, var_high);

  // input
  Node* value = Parameter(Descriptor::kArgument);
  Node* context = Parameter(Descriptor::kContext);

  TNode<BigInt>bigint = ToBigInt(CAST(context), CAST(value));

  // 2. Let int64bit be n modulo 2^64.
  BigIntToRawBytes(bigint, &var_low, &var_high);

  if (Is64()) {
    // 3. If int64bit ≥ 263, return int64bit - 264; otherwise return int64bit.
    ReturnRaw(Signed(var_low.value()));
  } else {
    /* Node* base = IntPtrConstant(0); */
    /* Node* pointerSize = IntPtrConstant(kPointerSize); */

    /* StoreNoWriteBarrier(MachineRepresentation::kWord32, base,
     *                     var_low.value()); */
    /* StoreNoWriteBarrier( */
    /*     MachineRepresentation::kWord32, IntPtrAdd(base, pointerSize), */
    /*     var_high.value()); */

    /* ReturnRaw(base); */
    UNIMPLEMENTED();
  }
}

// https://tc39.github.io/proposal-bigint/#sec-bigint-constructor-number-value
// TODO(sven): only implemented for int64, trap otherwise
TF_BUILTIN(NewBigInt, BigIntBuiltinsAssembler) {
  // FIXME(sven): test value trap otherwise
  TVARIABLE(BigInt, value_bigint);

  // input
  TNode<IntPtrT> ptr = UncheckedCast<IntPtrT>(Parameter(Descriptor::kArgument));

  Label is_int64(this);

  Goto(&is_int64);

  BIND(&is_int64);
  {
    if (Is64()) {
      value_bigint = BigIntFromInt64(ptr);
    } else {
#if defined(V8_TARGET_BIG_ENDIAN)
      TNode<IntPtrT> high = UncheckedCast<IntPtrT>(
          Load(MachineType::UintPtr(), ptr));
      TNode<IntPtrT> low = UncheckedCast<IntPtrT>(
          Load(MachineType::UintPtr(), ptr, Int32Constant(kPointerSize)));
#else
      TNode<IntPtrT> low = UncheckedCast<IntPtrT>(
          Load(MachineType::UintPtr(), ptr));
      TNode<IntPtrT> high = UncheckedCast<IntPtrT>(
          Load(MachineType::UintPtr(), ptr, Int32Constant(kPointerSize)));
#endif

      value_bigint = BigIntFromInt32Pair(low, high);
    }
  }

  DCHECK(IsBigInt(value_bigint.value()));

  Return(value_bigint.value());
}

}  // namespace internal
}  // namespace v8
