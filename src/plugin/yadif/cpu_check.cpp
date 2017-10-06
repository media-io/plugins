/*
cpu_check.cpp: Copyright (C) 2016  Oka Motofumi

Author: Oka Motofumi (chikuzen.mo at gmail dot com)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with the author; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#if defined(_M_IX86) || defined(_M_AMD64) || defined(__i686) || defined(__x86_64)

#include <cstdint>
#if defined(__GNUC__)
    #include <cpuid.h>
#else
    #include <intrin.h>
#endif


enum {
    CPU_NO_X86_SIMD          = 0x000000,
    CPU_SSE2_SUPPORT         = 0x000001,
    CPU_SSE3_SUPPORT         = 0x000002,
    CPU_SSSE3_SUPPORT        = 0x000004,
    CPU_SSE4_1_SUPPORT       = 0x000008,
    CPU_SSE4_2_SUPPORT       = 0x000010,
    CPU_SSE4_A_SUPPORT       = 0x000020,
    CPU_FMA4_SUPPORT         = 0x000040,
    CPU_FMA3_SUPPORT         = 0x000080,
    CPU_AVX_SUPPORT          = 0x000100,
    CPU_AVX2_SUPPORT         = 0x000200,
    CPU_AVX512F_SUPPORT      = 0x000400,
    CPU_AVX512DQ_SUPPORT     = 0x000800,
    CPU_AVX512IFMA52_SUPPORT = 0x001000,
    CPU_AVX512PF_SUPPORT     = 0x002000,
    CPU_AVX512ER_SUPPORT     = 0x004000,
    CPU_AVX512CD_SUPPORT     = 0x008000,
    CPU_AVX512BW_SUPPORT     = 0x010000,
    CPU_AVX512VL_SUPPORT     = 0x020000,
    CPU_AVX512VBMI_SUPPORT   = 0x040000,
};


static inline void get_cpuid(int *array, int info_type)
{
#if defined(__GNUC__)
    __cpuid(info_type, array[0], array[1], array[2], array[3]);
#else
    __cpuid(array, info_type);
#endif
}


static inline void get_cpuid2(int *array, int info_type, int ecx)
{
#if defined(__GNUC__)
    __cpuid_count(info_type, ecx, array[0], array[1], array[2], array[3]);
#else
    __cpuidex(array, info_type, ecx);
#endif
}

static uint32_t get_simd_support_info(void)
{
    uint32_t ret = 0;
    int regs[4] = {0};
    
    auto is_bit_set = [](int bitfield, int bit) {
        return (bitfield & (1 << bit)) != 0;
    };

    get_cpuid(regs, 0x00000001);
    if (is_bit_set(regs[3], 26)) {
        ret |= CPU_SSE2_SUPPORT;
    }
    if (is_bit_set(regs[2], 0)) {
        ret |= CPU_SSE3_SUPPORT;
    }
    if (is_bit_set(regs[2], 9)) {
        ret |= CPU_SSSE3_SUPPORT;
    }
    if (is_bit_set(regs[2], 19)) {
        ret |= CPU_SSE4_1_SUPPORT;
    }
    if (is_bit_set(regs[2], 26)) {
        ret |= CPU_SSE4_2_SUPPORT;
    }
    if (is_bit_set(regs[2], 27)) {
        if (is_bit_set(regs[2], 28)) {
            ret |= CPU_AVX_SUPPORT;
        }
        if (is_bit_set(regs[2], 12)) {
            ret |= CPU_FMA3_SUPPORT;
        }
    }

    regs[3] = 0;
    get_cpuid(regs, 0x80000001);
    if (is_bit_set(regs[3], 6)) {
        ret |= CPU_SSE4_A_SUPPORT;
    }
    if (is_bit_set(regs[3], 16)) {
        ret |= CPU_FMA4_SUPPORT;
    }

    get_cpuid(regs, 0x00000000);
    if (regs[0] < 7) {
        return ret;
    }

    get_cpuid2(regs, 0x00000007, 0);
    if (is_bit_set(regs[1], 5)) {
        ret |= CPU_AVX2_SUPPORT;
    }
    if (!is_bit_set(regs[1], 16)) {
        return ret;
    }

    ret |= CPU_AVX512F_SUPPORT;
    if (is_bit_set(regs[1], 17)) {
        ret |= CPU_AVX512DQ_SUPPORT;
    }
    if (is_bit_set(regs[1], 21)) {
        ret |= CPU_AVX512IFMA52_SUPPORT;
    }
    if (is_bit_set(regs[1], 26)) {
        ret |= CPU_AVX512PF_SUPPORT;
    }
    if (is_bit_set(regs[1], 27)) {
        ret |= CPU_AVX512ER_SUPPORT;
    }
    if (is_bit_set(regs[1], 28)) {
        ret |= CPU_AVX512CD_SUPPORT;
    }
    if (is_bit_set(regs[1], 30)) {
        ret |= CPU_AVX512BW_SUPPORT;
    }
    if (is_bit_set(regs[1], 31)) {
        ret |= CPU_AVX512VL_SUPPORT;
    }
    if (is_bit_set(regs[2], 1)) {
        ret |= CPU_AVX512VBMI_SUPPORT;
    }

    return ret;
}

bool has_sse2(void)
{
    return (get_simd_support_info() & CPU_SSE2_SUPPORT) != 0;
}

bool has_ssse3(void)
{
    return (get_simd_support_info() & CPU_SSSE3_SUPPORT) != 0;
}

bool has_sse41(void)
{
    return (get_simd_support_info() & CPU_SSE4_1_SUPPORT) != 0;
}

bool has_avx(void)
{
    return (get_simd_support_info() & CPU_AVX_SUPPORT) != 0;
}

bool has_avx2(void)
{
    return (get_simd_support_info() & CPU_AVX2_SUPPORT) != 0;
}

#endif
