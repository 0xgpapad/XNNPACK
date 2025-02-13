// Auto-generated file. Do not edit!
//   Template: src/f32-qs8-vcvt/avx.c.in
//   Generator: tools/xngen
//
// Copyright 2021 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <immintrin.h>

#include <xnnpack/common.h>
#include <xnnpack/intrinsics-polyfill.h>
#include <xnnpack/vcvt.h>


void xnn_f32_qu8_vcvt_ukernel__avx_x32(
    size_t n,
    const float* x,
    uint8_t* y,
    const union xnn_f32_qu8_cvt_params params[restrict XNN_MIN_ELEMENTS(1)])
{
  assert(n != 0);
  assert(n % sizeof(float) == 0);
  assert(x != NULL);
  assert(y != NULL);

  const __m256 vscale = _mm256_load_ps(params->avx.scale);
  const __m256 voutput_max_less_zero_point = _mm256_load_ps(params->avx.output_max_less_zero_point);
  const __m128i voutput_zero_point = _mm_load_si128((const __m128i*) params->avx.output_zero_point);
  const __m128i voutput_min = _mm_load_si128((const __m128i*) params->avx.output_min);

  for (; n >= 32 * sizeof(float); n -= 32 * sizeof(float)) {
    __m256 vx01234567 = _mm256_loadu_ps(x);
    __m256 vx89ABCDEF = _mm256_loadu_ps(x + 8);
    __m256 vxGHIJKLMN = _mm256_loadu_ps(x + 16);
    __m256 vxOPQRSTUV = _mm256_loadu_ps(x + 24);
    x += 32;

    vx01234567 = _mm256_mul_ps(vx01234567, vscale);
    vx89ABCDEF = _mm256_mul_ps(vx89ABCDEF, vscale);
    vxGHIJKLMN = _mm256_mul_ps(vxGHIJKLMN, vscale);
    vxOPQRSTUV = _mm256_mul_ps(vxOPQRSTUV, vscale);

    vx01234567 = _mm256_min_ps(vx01234567, voutput_max_less_zero_point);
    vx89ABCDEF = _mm256_min_ps(vx89ABCDEF, voutput_max_less_zero_point);
    vxGHIJKLMN = _mm256_min_ps(vxGHIJKLMN, voutput_max_less_zero_point);
    vxOPQRSTUV = _mm256_min_ps(vxOPQRSTUV, voutput_max_less_zero_point);

    const __m256i vacc01234567 = _mm256_cvtps_epi32(vx01234567);
    const __m256i vacc89ABCDEF = _mm256_cvtps_epi32(vx89ABCDEF);
    const __m256i vaccGHIJKLMN = _mm256_cvtps_epi32(vxGHIJKLMN);
    const __m256i vaccOPQRSTUV = _mm256_cvtps_epi32(vxOPQRSTUV);

    __m128i vy01234567 = _mm_packs_epi32(_mm256_castsi256_si128(vacc01234567), _mm256_extractf128_si256(vacc01234567, 1));
    __m128i vy89ABCDEF = _mm_packs_epi32(_mm256_castsi256_si128(vacc89ABCDEF), _mm256_extractf128_si256(vacc89ABCDEF, 1));
    __m128i vyGHIJKLMN = _mm_packs_epi32(_mm256_castsi256_si128(vaccGHIJKLMN), _mm256_extractf128_si256(vaccGHIJKLMN, 1));
    __m128i vyOPQRSTUV = _mm_packs_epi32(_mm256_castsi256_si128(vaccOPQRSTUV), _mm256_extractf128_si256(vaccOPQRSTUV, 1));

    vy01234567 = _mm_adds_epi16(vy01234567, voutput_zero_point);
    vy89ABCDEF = _mm_adds_epi16(vy89ABCDEF, voutput_zero_point);
    vyGHIJKLMN = _mm_adds_epi16(vyGHIJKLMN, voutput_zero_point);
    vyOPQRSTUV = _mm_adds_epi16(vyOPQRSTUV, voutput_zero_point);

    __m128i vy0123456789ABCDEF = _mm_packus_epi16(vy01234567, vy89ABCDEF);
    __m128i vyGHIJKLMNOPQRSTUV = _mm_packus_epi16(vyGHIJKLMN, vyOPQRSTUV);

    vy0123456789ABCDEF = _mm_max_epu8(vy0123456789ABCDEF, voutput_min);
    vyGHIJKLMNOPQRSTUV = _mm_max_epu8(vyGHIJKLMNOPQRSTUV, voutput_min);

    _mm_storeu_si128((__m128i*) y, vy0123456789ABCDEF);
    _mm_storeu_si128((__m128i*) (y + 16), vyGHIJKLMNOPQRSTUV);
    y += 32;
  }
  for (; n >= 8 * sizeof(float); n -= 8 * sizeof(float)) {
    __m256 vx = _mm256_loadu_ps(x);
    vx = _mm256_mul_ps(vx, vscale);
    vx = _mm256_min_ps(vx, voutput_max_less_zero_point);
    x += 8;

    const __m256i vacc = _mm256_cvtps_epi32(vx);

    __m128i vy = _mm_packs_epi32(_mm256_castsi256_si128(vacc), _mm256_extractf128_si256(vacc, 1));
    vy = _mm_adds_epi16(vy, voutput_zero_point);
    vy = _mm_packus_epi16(vy, vy);
    vy = _mm_max_epu8(vy, voutput_min);

    _mm_storel_epi64((__m128i*) y, vy);
    y += 8;
  }
  if XNN_UNLIKELY(n != 0) {
    assert(n >= 1 * sizeof(float));
    assert(n <= 7 * sizeof(float));
    const __m256i vmask = _mm256_loadu_si256((const __m256i*) ((uintptr_t) &params->avx.mask_table[7] - n));

    __m256 vx = _mm256_maskload_ps(x, vmask);
    vx = _mm256_mul_ps(vx, vscale);
    vx = _mm256_min_ps(vx, voutput_max_less_zero_point);

    const __m256i vacc = _mm256_cvtps_epi32(vx);

    __m128i vy = _mm_packs_epi32(_mm256_castsi256_si128(vacc), _mm256_extractf128_si256(vacc, 1));
    vy = _mm_adds_epi16(vy, voutput_zero_point);
    vy = _mm_packus_epi16(vy, vy);
    vy = _mm_max_epu8(vy, voutput_min);

    if (n & (4 * sizeof(float))) {
      _mm_storeu_si32(y, vy);
      y += 4;
      vy = _mm_srli_epi64(vy, 32);
    }
    if (n & (2 * sizeof(float))) {
      _mm_storeu_si16(y, vy);
      y += 2;
      vy = _mm_srli_epi32(vy, 16);
    }
    if (n & (1 * sizeof(float))) {
      *y = (uint8_t) _mm_extract_epi8(vy, 0);
    }
  }
}
