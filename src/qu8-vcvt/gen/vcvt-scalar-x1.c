// Auto-generated file. Do not edit!
//   Template: src/qs8-vcvt/scalar.c.in
//   Generator: tools/xngen
//
// Copyright 2022 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <xnnpack/math.h>
#include <xnnpack/vcvt.h>


void xnn_qu8_vcvt_ukernel__scalar_x1(
    size_t n,
    const uint8_t* x,
    uint8_t* y,
    const union xnn_qu8_cvt_params params[restrict XNN_MIN_ELEMENTS(1)])
{
  const int32_t vbias = params->scalar.bias;
  const int32_t vmultiplier = params->scalar.multiplier;
  do {
    int32_t vacc = *x++;
    vacc = vbias + vacc * vmultiplier;

    int32_t vout = asr_s32(vacc, 8);
    vout = math_max_s32(vout, 0);
    vout = math_min_s32(vout, 255);
    *y++ = (uint8_t) vout;

    n -= sizeof(uint8_t);
  } while (n != 0);
}
