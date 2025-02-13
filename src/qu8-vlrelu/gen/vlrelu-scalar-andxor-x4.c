// Auto-generated file. Do not edit!
//   Template: src/qs8-vlrelu/scalar-andxor.c.in
//   Generator: tools/xngen
//
// Copyright 2022 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <xnnpack/math.h>
#include <xnnpack/vlrelu.h>


void xnn_qu8_vlrelu_ukernel__scalar_andxor_x4(
    size_t n,
    const uint8_t* x,
    uint8_t* y,
    const union xnn_qu8_lrelu_params params[restrict XNN_MIN_ELEMENTS(1)])
{
  const int32_t vinput_zero_point = params->scalar_andxor.input_zero_point;
  const int32_t vmultiplier_diff = params->scalar_andxor.multiplier_diff;
  const int32_t vmultiplier_base = params->scalar_andxor.multiplier_base;
  const int32_t vbias = params->scalar_andxor.bias;
  for (; n >= 4 * sizeof(uint8_t); n -= 4 * sizeof(uint8_t)) {
    int32_t vacc0 = (int32_t) x[0];
    int32_t vacc1 = (int32_t) x[1];
    int32_t vacc2 = (int32_t) x[2];
    int32_t vacc3 = (int32_t) x[3];
    x += 4;

    vacc0 -= vinput_zero_point;
    vacc1 -= vinput_zero_point;
    vacc2 -= vinput_zero_point;
    vacc3 -= vinput_zero_point;

    int32_t vmultiplier0 = asr_s32(vacc0, 31);
    int32_t vmultiplier1 = asr_s32(vacc1, 31);
    int32_t vmultiplier2 = asr_s32(vacc2, 31);
    int32_t vmultiplier3 = asr_s32(vacc3, 31);

    vmultiplier0 &= vmultiplier_diff;
    vmultiplier1 &= vmultiplier_diff;
    vmultiplier2 &= vmultiplier_diff;
    vmultiplier3 &= vmultiplier_diff;

    vmultiplier0 ^= vmultiplier_base;
    vmultiplier1 ^= vmultiplier_base;
    vmultiplier2 ^= vmultiplier_base;
    vmultiplier3 ^= vmultiplier_base;

    vacc0 = vbias + vacc0 * vmultiplier0;
    vacc1 = vbias + vacc1 * vmultiplier1;
    vacc2 = vbias + vacc2 * vmultiplier2;
    vacc3 = vbias + vacc3 * vmultiplier3;

    int32_t vout0 = asr_s32(vacc0, 8);
    int32_t vout1 = asr_s32(vacc1, 8);
    int32_t vout2 = asr_s32(vacc2, 8);
    int32_t vout3 = asr_s32(vacc3, 8);

    vout0 = math_max_s32(vout0, 0);
    vout1 = math_max_s32(vout1, 0);
    vout2 = math_max_s32(vout2, 0);
    vout3 = math_max_s32(vout3, 0);

    vout0 = math_min_s32(vout0, 255);
    vout1 = math_min_s32(vout1, 255);
    vout2 = math_min_s32(vout2, 255);
    vout3 = math_min_s32(vout3, 255);

    y[0] = (uint8_t) vout0;
    y[1] = (uint8_t) vout1;
    y[2] = (uint8_t) vout2;
    y[3] = (uint8_t) vout3;
    y += 4;
  }
  if XNN_UNLIKELY(n != 0) {
    do {
      int32_t vacc = (int32_t) *x++ - vinput_zero_point;
      const int32_t vmultiplier = vmultiplier_base ^ (vmultiplier_diff & asr_s32(vacc, 31));
      vacc = vbias + vacc * vmultiplier;

      int32_t vout = asr_s32(vacc, 8);
      vout = math_max_s32(vout, 0);
      vout = math_min_s32(vout, 255);
      *y++ = (uint8_t) vout;

      n -= sizeof(uint8_t);
    } while (n != 0);
  }
}
