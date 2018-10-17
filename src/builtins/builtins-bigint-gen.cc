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
    // TODO(sven): return a int32 pair. It seems that wasm don't support
    // 32bits yet?
    UNIMPLEMENTED();
  }
}

}  // namespace internal
}  // namespace v8
