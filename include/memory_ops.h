//
//  memory_ops.h
//  PROJECT memory_ops
//
//  Copied from DPDK
//

#pragma once
#include <immintrin.h>
#include <xmmintrin.h>
#include <cstdint>
#include <cstdio>
#include <cstring>

/**
 * Macro to align a value to a given power-of-two. The resultant value
 * will be of the same type as the first parameter, and will be no
 * bigger than the first parameter. Second parameter must be a
 * power-of-two value.
 */
#define RTE_PTR_ALIGN(ptr, align) \
  ((typeof(ptr))RTE_ALIGN_FLOOR((uintptr_t)ptr, align))

#define RTE_ALIGN_FLOOR(val, align) \
  (typeof(val))((val) & (~((typeof(val))((align)-1))))

// copy 16 bytes from one location to another
// address should not overlap
inline void rte_mov16(uint8_t *dst, const uint8_t *src) {
  __m128i xmm0;
  xmm0 = _mm_loadu_si128((const __m128i *)src);
  _mm_storeu_si128((__m128i *)dst, xmm0);
}

// copy 32 bytes like below
inline void rte_mov32(uint8_t *dst, const uint8_t *src) {
  __m256i ymm0;
  ymm0 = _mm256_loadu_si256((const __m256i *)src);
  _mm256_storeu_si256((__m256i *)dst, ymm0);
}

// copy 64 bytes like below
inline void rte_mov64(uint8_t *dst, const uint8_t *src) {
  __m512i zmm0;
  zmm0 = _mm512_loadu_si512((const void *)src);
  _mm512_storeu_si512((void *)dst, zmm0);
}

// copy 128 bytes like below
inline void rte_mov128(uint8_t *dst, const uint8_t *src) {
  rte_mov64(dst, src);
  rte_mov64(dst + 1 * 64, src + 1 * 64);
}

// copy 256 bytes like below
inline void rte_mov256(uint8_t *dst, const uint8_t *src) {
  rte_mov64(dst, src);
  rte_mov64(dst + 1 * 64, src + 1 * 64);
  rte_mov64(dst + 2 * 64, src + 2 * 64);
  rte_mov64(dst + 3 * 64, src + 3 * 64);
}

// copy the first 128 blocks in size n
inline void rte_mov128blocks(uint8_t *dst, const uint8_t *src, size_t n) {
  __m512i zmm0, zmm1;

  while (n >= 128) {
    zmm0 = _mm512_loadu_si512((const void *)(src + 0 * 64));
    n -= 128;
    zmm1 = _mm512_loadu_si512((const void *)(src + 1 * 64));
    src = src + 128;
    _mm512_storeu_si512((void *)(dst + 0 * 64), zmm0);
    _mm512_storeu_si512((void *)(dst + 1 * 64), zmm1);
    dst = dst + 128;
  }
}

// copy the first 512 blocks in size n
inline void rte_mov512blocks(uint8_t *dst, const uint8_t *src, size_t n) {
  __m512i zmm0, zmm1, zmm2, zmm3, zmm4, zmm5, zmm6, zmm7;

  while (n >= 512) {
    zmm0 = _mm512_loadu_si512((const void *)(src + 0 * 64));
    n -= 512;
    zmm1 = _mm512_loadu_si512((const void *)(src + 1 * 64));
    zmm2 = _mm512_loadu_si512((const void *)(src + 2 * 64));
    zmm3 = _mm512_loadu_si512((const void *)(src + 3 * 64));
    zmm4 = _mm512_loadu_si512((const void *)(src + 4 * 64));
    zmm5 = _mm512_loadu_si512((const void *)(src + 5 * 64));
    zmm6 = _mm512_loadu_si512((const void *)(src + 6 * 64));
    zmm7 = _mm512_loadu_si512((const void *)(src + 7 * 64));
    src = src + 512;
    _mm512_storeu_si512((void *)(dst + 0 * 64), zmm0);
    _mm512_storeu_si512((void *)(dst + 1 * 64), zmm1);
    _mm512_storeu_si512((void *)(dst + 2 * 64), zmm2);
    _mm512_storeu_si512((void *)(dst + 3 * 64), zmm3);
    _mm512_storeu_si512((void *)(dst + 4 * 64), zmm4);
    _mm512_storeu_si512((void *)(dst + 5 * 64), zmm5);
    _mm512_storeu_si512((void *)(dst + 6 * 64), zmm6);
    _mm512_storeu_si512((void *)(dst + 7 * 64), zmm7);
    dst = dst + 512;
  }
}

inline void *rte_memcpy_generic(void *dst, const void *src, size_t n) {
  uintptr_t dstu = (uintptr_t)dst;
  uintptr_t srcu = (uintptr_t)src;
  void *ret = dst;
  size_t dstofss;
  size_t bits;

  /**
   * Copy less than 16 bytes
   */
  if (n < 16) {
    if (n & 0x01) {
      *(uint8_t *)dstu = *(const uint8_t *)srcu;
      srcu = (uintptr_t)((const uint8_t *)srcu + 1);
      dstu = (uintptr_t)((uint8_t *)dstu + 1);
    }
    if (n & 0x02) {
      *(uint16_t *)dstu = *(const uint16_t *)srcu;
      srcu = (uintptr_t)((const uint16_t *)srcu + 1);
      dstu = (uintptr_t)((uint16_t *)dstu + 1);
    }
    if (n & 0x04) {
      *(uint32_t *)dstu = *(const uint32_t *)srcu;
      srcu = (uintptr_t)((const uint32_t *)srcu + 1);
      dstu = (uintptr_t)((uint32_t *)dstu + 1);
    }
    if (n & 0x08) *(uint64_t *)dstu = *(const uint64_t *)srcu;
    return ret;
  }

  /**
   * Fast way when copy size doesn't exceed 512 bytes
   */
  if (n <= 32) {
    rte_mov16((uint8_t *)dst, (const uint8_t *)src);
    rte_mov16((uint8_t *)dst - 16 + n, (const uint8_t *)src - 16 + n);
    return ret;
  }
  if (n <= 64) {
    rte_mov32((uint8_t *)dst, (const uint8_t *)src);
    rte_mov32((uint8_t *)dst - 32 + n, (const uint8_t *)src - 32 + n);
    return ret;
  }
  if (n <= 512) {
    if (n >= 256) {
      n -= 256;
      rte_mov256((uint8_t *)dst, (const uint8_t *)src);
      src = (const uint8_t *)src + 256;
      dst = (uint8_t *)dst + 256;
    }
    if (n >= 128) {
      n -= 128;
      rte_mov128((uint8_t *)dst, (const uint8_t *)src);
      src = (const uint8_t *)src + 128;
      dst = (uint8_t *)dst + 128;
    }
  COPY_BLOCK_128_BACK63:
    if (n > 64) {
      rte_mov64((uint8_t *)dst, (const uint8_t *)src);
      rte_mov64((uint8_t *)dst - 64 + n, (const uint8_t *)src - 64 + n);
      return ret;
    }
    if (n > 0)
      rte_mov64((uint8_t *)dst - 64 + n, (const uint8_t *)src - 64 + n);
    return ret;
  }

  /**
   * Make store aligned when copy size exceeds 512 bytes
   */
  dstofss = ((uintptr_t)dst & 0x3F);
  if (dstofss > 0) {
    dstofss = 64 - dstofss;
    n -= dstofss;
    rte_mov64((uint8_t *)dst, (const uint8_t *)src);
    src = (const uint8_t *)src + dstofss;
    dst = (uint8_t *)dst + dstofss;
  }

  /**
   * Copy 512-byte blocks.
   * Use copy block function for better instruction order control,
   * which is important when load is unaligned.
   */
  rte_mov512blocks((uint8_t *)dst, (const uint8_t *)src, n);
  bits = n;
  n = n & 511;
  bits -= n;
  src = (const uint8_t *)src + bits;
  dst = (uint8_t *)dst + bits;

  /**
   * Copy 128-byte blocks.
   * Use copy block function for better instruction order control,
   * which is important when load is unaligned.
   */
  if (n >= 128) {
    rte_mov128blocks((uint8_t *)dst, (const uint8_t *)src, n);
    bits = n;
    n = n & 127;
    bits -= n;
    src = (const uint8_t *)src + bits;
    dst = (uint8_t *)dst + bits;
  }

  /**
   * Copy whatever left
   */
  goto COPY_BLOCK_128_BACK63;
}
