/*
* Simd Library (http://ermig1979.github.io/Simd).
*
* Copyright (c) 2011-2020 Yermalayeu Ihar.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef __SimdEnable_h__
#define __SimdEnable_h__

#include "Simd/SimdCpu.h"

#if defined(_MSC_VER)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <intrin.h>
#endif

#if defined(__GNUC__)
#if defined(SIMD_PPC_ENABLE) || defined(SIMD_PPC64_ENABLE) || defined(SIMD_ARM_ENABLE) || defined(SIMD_ARM64_ENABLE)
#include <fcntl.h>
#include <sys/auxv.h>
#if defined(SIMD_ARM_ENABLE) || defined(SIMD_ARM64_ENABLE)
#include <asm/hwcap.h>
#endif
#endif
#endif

namespace Simd
{
#ifdef SIMD_SSE_ENABLE
    namespace Sse
    {
        SIMD_INLINE bool SupportedByCPU()
        {
            return Base::CheckBit(Cpuid::Ordinary, Cpuid::Edx, Cpuid::SSE);
        }

        SIMD_INLINE bool SupportedByOS()
        {
#if defined(_MSC_VER)
            __try
            {
                __m128 value = _mm_set1_ps(1.0f);// try to execute of SSE instructions;
                return true;
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
                return false;
            }
#else
            return true;
#endif
        }

        const bool Enable = SupportedByCPU() && SupportedByOS();
    }
#endif

#ifdef SIMD_SSE2_ENABLE
    namespace Sse2
    {
        SIMD_INLINE bool SupportedByCPU()
        {
            return Base::CheckBit(Cpuid::Ordinary, Cpuid::Edx, Cpuid::SSE2);
        }

        SIMD_INLINE bool SupportedByOS()
        {
#if defined(_MSC_VER)
            __try
            {
                __m128d value = _mm_set1_pd(1.0);// try to execute of SSE2 instructions;
                return true;
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
                return false;
            }
#else
            return true;
#endif
        }

        const bool Enable = SupportedByCPU() && SupportedByOS();
    }
#endif

#ifdef SIMD_SSE3_ENABLE
    namespace Sse3
    {
        SIMD_INLINE bool SupportedByCPU()
        {
            return Base::CheckBit(Cpuid::Ordinary, Cpuid::Ecx, Cpuid::SSE3);
        }

        SIMD_INLINE bool SupportedByOS()
        {
#if defined(_MSC_VER)
            __try
            {
                __m128 value = _mm_hadd_ps(_mm_set1_ps(1.0f), _mm_set1_ps(2.0f)); //try to execute of SSE3 instructions;
                return true;
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
                return false;
            }
#else
            return true;
#endif
        }

        const bool Enable = SupportedByCPU() && SupportedByOS();
    }
#endif

#ifdef SIMD_SSSE3_ENABLE
    namespace Ssse3
    {
        SIMD_INLINE bool SupportedByCPU()
        {
            return Base::CheckBit(Cpuid::Ordinary, Cpuid::Ecx, Cpuid::SSSE3);
        }

        SIMD_INLINE bool SupportedByOS()
        {
#if defined(_MSC_VER)
            __try
            {
                __m128i value = _mm_abs_epi8(_mm_set1_epi8(-1)); //try to execute of SSSE3 instructions;
                return true;
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
                return false;
            }
#else
            return true;
#endif
        }

        const bool Enable = SupportedByCPU() && SupportedByOS();
    }
#endif

#ifdef SIMD_SSE41_ENABLE
    namespace Sse41
    {
        SIMD_INLINE bool SupportedByCPU()
        {
            return Base::CheckBit(Cpuid::Ordinary, Cpuid::Ecx, Cpuid::SSE41);
        }

        SIMD_INLINE bool SupportedByOS()
        {
#if defined(_MSC_VER)
            __try
            {
                int value = _mm_testz_si128(_mm_set1_epi8(0), _mm_set1_epi8(-1)); // try to execute of SSE41 instructions;
                return true;
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
                return false;
            }
#else
            return true;
#endif
        }

        const bool Enable = SupportedByCPU() && SupportedByOS();
    }
#endif

#ifdef SIMD_SSE42_ENABLE
    namespace Sse42
    {
        SIMD_INLINE bool SupportedByCPU()
        {
            return Base::CheckBit(Cpuid::Ordinary, Cpuid::Ecx, Cpuid::SSE42);
        }

        SIMD_INLINE bool SupportedByOS()
        {
#if defined(_MSC_VER)
            __try
            {
                uint32_t value = _mm_crc32_u8(0, 1); // try to execute of SSE42 instructions;
                return true;
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
                return false;
            }
#else
            return true;
#endif
        }

        const bool Enable = SupportedByCPU() && SupportedByOS();
    }
#endif

#ifdef SIMD_AVX_ENABLE
    namespace Avx
    {
        SIMD_INLINE bool SupportedByCPU()
        {
            return
                Base::CheckBit(Cpuid::Ordinary, Cpuid::Ecx, Cpuid::OSXSAVE) &&
                Base::CheckBit(Cpuid::Ordinary, Cpuid::Ecx, Cpuid::AVX);
        }

        SIMD_INLINE bool SupportedByOS()
        {
#if defined(_MSC_VER)
            __try
            {
                __m256d value = _mm256_set1_pd(1.0);// try to execute of AVX instructions;
                return true;
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
                return false;
            }
#else
            return true;
#endif
        }

        const bool Enable = SupportedByCPU() && SupportedByOS();
    }
#endif

#ifdef SIMD_AVX2_ENABLE
    namespace Avx2
    {
        SIMD_INLINE bool SupportedByCPU()
        {
            return
                Base::CheckBit(Cpuid::Ordinary, Cpuid::Ecx, Cpuid::OSXSAVE) &&
                Base::CheckBit(Cpuid::Extended, Cpuid::Ebx, Cpuid::AVX2) &&
                Base::CheckBit(Cpuid::Ordinary, Cpuid::Ecx, Cpuid::FMA) &&
                Base::CheckBit(Cpuid::Ordinary, Cpuid::Ecx, Cpuid::F16C);
        }

        SIMD_INLINE bool SupportedByOS()
        {
#if defined(_MSC_VER)
            __try
            {
                __m256i value = _mm256_abs_epi8(_mm256_set1_epi8(1));// try to execute of AVX2 instructions;
                return true;
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
                return false;
            }
#else
            return true;
#endif
        }

        const bool Enable = SupportedByCPU() && SupportedByOS();
    }
#endif

#ifdef SIMD_AVX512F_ENABLE
    namespace Avx512f
    {
        SIMD_INLINE bool SupportedByCPU()
        {
            return
                Base::CheckBit(Cpuid::Extended, Cpuid::Ebx, Cpuid::AVX512F) &&
                Base::CheckBit(Cpuid::Extended, Cpuid::Ebx, Cpuid::AVX512CD);
        }

        SIMD_INLINE bool SupportedByOS()
        {
#if defined(_MSC_VER)
            __try
            {
                __m512d value = _mm512_set1_pd(1.0);// try to execute of AVX-512F instructions;
                return true;
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
                return false;
            }
#else
            return true;
#endif
        }

        const bool Enable = SupportedByCPU() && SupportedByOS();
    }
#endif

#ifdef SIMD_AVX512BW_ENABLE
    namespace Avx512bw
    {
        SIMD_INLINE bool SupportedByCPU()
        {
            return
                Base::CheckBit(Cpuid::Extended, Cpuid::Ebx, Cpuid::AVX512F) &&
                Base::CheckBit(Cpuid::Extended, Cpuid::Ebx, Cpuid::AVX512CD) &&
                Base::CheckBit(Cpuid::Extended, Cpuid::Ebx, Cpuid::AVX512DQ) &&
                Base::CheckBit(Cpuid::Extended, Cpuid::Ebx, Cpuid::AVX512BW) &&
                Base::CheckBit(Cpuid::Extended, Cpuid::Ebx, Cpuid::AVX512VL);
        }

        SIMD_INLINE bool SupportedByOS()
        {
#if defined(_MSC_VER)
            __try
            {
                __m512i value = _mm512_abs_epi8(_mm512_set1_epi8(1));// try to execute of AVX-512BW instructions;
                return true;
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
                return false;
            }
#else
            return true;
#endif
        }

        const bool Enable = SupportedByCPU() && SupportedByOS();
    }
#endif

#ifdef SIMD_AVX512VNNI_ENABLE
    namespace Avx512vnni
    {
        SIMD_INLINE bool SupportedByCPU()
        {
            return
                Base::CheckBit(Cpuid::Extended, Cpuid::Ecx, Cpuid::AVX512VNNI);
        }

        SIMD_INLINE bool SupportedByOS()
        {
#if defined(_MSC_VER)
            __try
            {
                __m512i value = _mm512_dpbusd_epi32(_mm512_setzero_si512(), _mm512_set1_epi8(1), _mm512_set1_epi8(1));// try to execute of AVX-512VNNI instructions;
                return true;
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
                return false;
            }
#else
            return true;
#endif
        }

        const bool Enable = SupportedByCPU() && SupportedByOS();
    }
#endif

#ifdef SIMD_VMX_ENABLE
    namespace Vmx
    {
        SIMD_INLINE bool SupportedByCPU()
        {
            return Base::CheckBit(AT_HWCAP, PPC_FEATURE_HAS_ALTIVEC);
        }

        SIMD_INLINE bool SupportedByOS()
        {
            return true;
        }

        const bool Enable = SupportedByCPU() && SupportedByOS();
    }
#endif

#ifdef SIMD_VSX_ENABLE
    namespace Vsx
    {
        SIMD_INLINE bool SupportedByCPU()
        {
            return Base::CheckBit(AT_HWCAP, PPC_FEATURE_HAS_VSX);
        }

        SIMD_INLINE bool SupportedByOS()
        {
            return true;
        }

        const bool Enable = SupportedByCPU() && SupportedByOS();
    }
#endif

#ifdef SIMD_NEON_ENABLE
    namespace Neon
    {
        SIMD_INLINE bool SupportedByCPU()
        {
#if defined(_MSC_VER)
            return true;
#elif defined(__GNUC__)
#if defined(SIMD_ARM64_ENABLE)
            return true;
#else
            return Base::CheckBit(AT_HWCAP, HWCAP_NEON);
#endif
#else
#error Do not know how to detect NEON support!
#endif
        }

        SIMD_INLINE bool SupportedByOS()
        {
            return true;
        }

        const bool Enable = SupportedByCPU() && SupportedByOS();
    }
#endif

#ifdef SIMD_MSA_ENABLE
    namespace Msa
    {
        SIMD_INLINE bool SupportedByCPU()
        {
            return true;
        }

        SIMD_INLINE bool SupportedByOS()
        {
            return true;
        }

        const bool Enable = SupportedByCPU() && SupportedByOS();
    }
#endif

    SIMD_INLINE size_t Alignment()
    {
#ifdef SIMD_AVX512VNNI_ENABLE
        if (Avx512vnni::Enable)
            return sizeof(__m512i);
        else
#endif
#ifdef SIMD_AVX512BW_ENABLE
        if (Avx512bw::Enable)
            return sizeof(__m512i);
        else
#endif
#ifdef SIMD_AVX512F_ENABLE
        if (Avx512f::Enable)
            return sizeof(__m512);
        else
#endif
#ifdef SIMD_AVX2_ENABLE
        if (Avx2::Enable)
            return sizeof(__m256i);
        else
#endif
#ifdef SIMD_AVX_ENABLE
        if (Avx::Enable)
            return sizeof(__m256);
        else
#endif
#ifdef SIMD_SSE41_ENABLE
        if (Sse41::Enable)
            return sizeof(__m128i);
        else
#endif
#ifdef SIMD_SSSE3_ENABLE
        if (Ssse3::Enable)
            return sizeof(__m128i);
        else
#endif
#ifdef SIMD_SSE2_ENABLE
        if (Sse2::Enable)
            return sizeof(__m128i);
        else
#endif
#ifdef SIMD_SSE_ENABLE
        if (Sse::Enable)
            return sizeof(__m128);
        else
#endif
#ifdef SIMD_VSX_ENABLE
        if (Vsx::Enable)
            return sizeof(__vector uint8_t);
        else
#endif
#ifdef SIMD_VMX_ENABLE
        if (Vmx::Enable)
            return sizeof(__vector uint8_t);
        else
#endif
#ifdef SIMD_NEON_ENABLE
        if (Neon::Enable)
            return sizeof(uint8x16_t);
        else
#endif
#ifdef SIMD_MSA_ENABLE
        if (Msa::Enable)
            return sizeof(v16u8);
        else
#endif
            return sizeof(void *);
    }

    const size_t ALIGNMENT = Alignment();
}

#define SIMD_BASE_FUNC(func) Simd::Base::func

#ifdef SIMD_SSE_ENABLE
#define SIMD_SSE_FUNC(func) Simd::Sse::Enable ? Simd::Sse::func : 
#else
#define SIMD_SSE_FUNC(func) 
#endif

#ifdef SIMD_SSE2_ENABLE
#define SIMD_SSE2_FUNC(func) Simd::Sse2::Enable ? Simd::Sse2::func : 
#else
#define SIMD_SSE2_FUNC(func) 
#endif

#ifdef SIMD_SSE3_ENABLE
#define SIMD_SSE3_FUNC(func) Simd::Sse3::Enable ? Simd::Sse3::func : 
#else
#define SIMD_SSE3_FUNC(func) 
#endif

#ifdef SIMD_SSSE3_ENABLE
#define SIMD_SSSE3_FUNC(func) Simd::Ssse3::Enable ? Simd::Ssse3::func : 
#else
#define SIMD_SSSE3_FUNC(func) 
#endif

#ifdef SIMD_SSE41_ENABLE
#define SIMD_SSE41_FUNC(func) Simd::Sse41::Enable ? Simd::Sse41::func : 
#else
#define SIMD_SSE41_FUNC(func) 
#endif

#ifdef SIMD_SSE42_ENABLE
#define SIMD_SSE42_FUNC(func) Simd::Sse42::Enable ? Simd::Sse42::func : 
#else
#define SIMD_SSE42_FUNC(func) 
#endif

#ifdef SIMD_AVX_ENABLE
#define SIMD_AVX_FUNC(func) Simd::Avx::Enable ? Simd::Avx::func : 
#else
#define SIMD_AVX_FUNC(func)
#endif

#ifdef SIMD_AVX2_ENABLE
#define SIMD_AVX2_FUNC(func) Simd::Avx2::Enable ? Simd::Avx2::func : 
#else
#define SIMD_AVX2_FUNC(func)
#endif

#ifdef SIMD_AVX512F_ENABLE
#define SIMD_AVX512F_FUNC(func) Simd::Avx512f::Enable ? Simd::Avx512f::func : 
#else
#define SIMD_AVX512F_FUNC(func)
#endif

#ifdef SIMD_AVX512BW_ENABLE
#define SIMD_AVX512BW_FUNC(func) Simd::Avx512bw::Enable ? Simd::Avx512bw::func : 
#else
#define SIMD_AVX512BW_FUNC(func)
#endif

#ifdef SIMD_AVX512VNNI_ENABLE
#define SIMD_AVX512VNNI_FUNC(func) Simd::Avx512vnni::Enable ? Simd::Avx512vnni::func : 
#else
#define SIMD_AVX512VNNI_FUNC(func)
#endif

#ifdef SIMD_VMX_ENABLE
#define SIMD_VMX_FUNC(func) Simd::Vmx::Enable ? Simd::Vmx::func : 
#else
#define SIMD_VMX_FUNC(func)
#endif

#ifdef SIMD_VSX_ENABLE
#define SIMD_VSX_FUNC(func) Simd::Vsx::Enable ? Simd::Vsx::func : 
#else
#define SIMD_VSX_FUNC(func)
#endif

#ifdef SIMD_NEON_ENABLE
#define SIMD_NEON_FUNC(func) Simd::Neon::Enable ? Simd::Neon::func : 
#else
#define SIMD_NEON_FUNC(func)
#endif

#define SIMD_FUNC0(func) SIMD_BASE_FUNC(func)
#define SIMD_FUNC1(func, EXT1) EXT1(func) SIMD_BASE_FUNC(func)
#define SIMD_FUNC2(func, EXT1, EXT2) EXT1(func) EXT2(func) SIMD_BASE_FUNC(func)
#define SIMD_FUNC3(func, EXT1, EXT2, EXT3) EXT1(func) EXT2(func) EXT3(func) SIMD_BASE_FUNC(func)
#define SIMD_FUNC4(func, EXT1, EXT2, EXT3, EXT4) EXT1(func) EXT2(func) EXT3(func) EXT4(func) SIMD_BASE_FUNC(func)
#define SIMD_FUNC5(func, EXT1, EXT2, EXT3, EXT4, EXT5) EXT1(func) EXT2(func) EXT3(func) EXT4(func) EXT5(func) SIMD_BASE_FUNC(func)
#define SIMD_FUNC6(func, EXT1, EXT2, EXT3, EXT4, EXT5, EXT6) EXT1(func) EXT2(func) EXT3(func) EXT4(func) EXT5(func) EXT6(func) SIMD_BASE_FUNC(func)

#endif//__SimdEnable_h__
