/*  This file is part of the Vc library. {{{
Copyright © 2009-2014 Matthias Kretz <kretz@kde.org>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the names of contributing organizations nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

}}}*/

#ifndef AVX_VECTORHELPER_H
#define AVX_VECTORHELPER_H

#include <limits>
#include "types.h"
#include "intrinsics.h"
#include "casts.h"
#include "../common/loadstoreflags.h"
#include "macros.h"

namespace Vc_VERSIONED_NAMESPACE
{
namespace AVX
{

#define OP0(name, code) static Vc_ALWAYS_INLINE Vc_CONST VectorType name() { return code; }
#define OP1(name, code) static Vc_ALWAYS_INLINE Vc_CONST VectorType name(VTArg a) { return code; }
#define OP2(name, code) static Vc_ALWAYS_INLINE Vc_CONST VectorType name(VTArg a, VTArg b) { return code; }
#define OP3(name, code) static Vc_ALWAYS_INLINE Vc_CONST VectorType name(VTArg a, VTArg b, VTArg c) { return code; }

        template<> struct VectorHelper<__m128>
        {
            typedef __m128 VectorType;
#ifdef VC_PASSING_VECTOR_BY_VALUE_IS_BROKEN
            typedef const VectorType & VTArg;
#else
            typedef const VectorType VTArg;
#endif
        };

        template<> struct VectorHelper<__m256>
        {
            typedef __m256 VectorType;
#ifdef VC_PASSING_VECTOR_BY_VALUE_IS_BROKEN
            typedef const VectorType & VTArg;
#else
            typedef const VectorType VTArg;
#endif

            template<typename Flags> static Vc_ALWAYS_INLINE void store(float *mem, VTArg x, typename Flags::EnableIfAligned               = nullptr) { _mm256_store_ps(mem, x); }
            template<typename Flags> static Vc_ALWAYS_INLINE void store(float *mem, VTArg x, typename Flags::EnableIfUnalignedNotStreaming = nullptr) { _mm256_storeu_ps(mem, x); }
            template<typename Flags> static Vc_ALWAYS_INLINE void store(float *mem, VTArg x, typename Flags::EnableIfStreaming             = nullptr) { _mm256_stream_ps(mem, x); }
            template<typename Flags> static Vc_ALWAYS_INLINE void store(float *mem, VTArg x, typename Flags::EnableIfUnalignedAndStreaming = nullptr) { AvxIntrinsics::stream_store(mem, x, setallone_ps()); }

            template<typename Flags> static Vc_ALWAYS_INLINE void store(float *mem, VTArg x, VTArg m, typename std::enable_if<!Flags::IsStreaming, void *>::type = nullptr) { _mm256_maskstore(mem, m, x); }
            template<typename Flags> static Vc_ALWAYS_INLINE void store(float *mem, VTArg x, VTArg m, typename std::enable_if< Flags::IsStreaming, void *>::type = nullptr) { AvxIntrinsics::stream_store(mem, x, m); }

            static Vc_ALWAYS_INLINE Vc_CONST VectorType cdab(VTArg x) { return _mm256_permute_ps(x, _MM_SHUFFLE(2, 3, 0, 1)); }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType badc(VTArg x) { return _mm256_permute_ps(x, _MM_SHUFFLE(1, 0, 3, 2)); }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType aaaa(VTArg x) { return _mm256_permute_ps(x, _MM_SHUFFLE(0, 0, 0, 0)); }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType bbbb(VTArg x) { return _mm256_permute_ps(x, _MM_SHUFFLE(1, 1, 1, 1)); }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType cccc(VTArg x) { return _mm256_permute_ps(x, _MM_SHUFFLE(2, 2, 2, 2)); }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType dddd(VTArg x) { return _mm256_permute_ps(x, _MM_SHUFFLE(3, 3, 3, 3)); }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType dacb(VTArg x) { return _mm256_permute_ps(x, _MM_SHUFFLE(3, 0, 2, 1)); }

            OP0(allone, setallone_ps())
            OP0(zero, _mm256_setzero_ps())
            OP2(or_, _mm256_or_ps(a, b))
            OP2(xor_, _mm256_xor_ps(a, b))
            OP2(and_, _mm256_and_ps(a, b))
            OP2(andnot_, _mm256_andnot_ps(a, b))
            OP3(blend, _mm256_blendv_ps(a, b, c))
        };

        template<> struct VectorHelper<__m256d>
        {
            typedef __m256d VectorType;
#ifdef VC_PASSING_VECTOR_BY_VALUE_IS_BROKEN
            typedef const VectorType & VTArg;
#else
            typedef const VectorType VTArg;
#endif

            template<typename Flags> static Vc_ALWAYS_INLINE void store(double *mem, VTArg x, typename Flags::EnableIfAligned               = nullptr) { _mm256_store_pd(mem, x); }
            template<typename Flags> static Vc_ALWAYS_INLINE void store(double *mem, VTArg x, typename Flags::EnableIfUnalignedNotStreaming = nullptr) { _mm256_storeu_pd(mem, x); }
            template<typename Flags> static Vc_ALWAYS_INLINE void store(double *mem, VTArg x, typename Flags::EnableIfStreaming             = nullptr) { _mm256_stream_pd(mem, x); }
            template<typename Flags> static Vc_ALWAYS_INLINE void store(double *mem, VTArg x, typename Flags::EnableIfUnalignedAndStreaming = nullptr) { AvxIntrinsics::stream_store(mem, x, setallone_pd()); }

            template<typename Flags> static Vc_ALWAYS_INLINE void store(double *mem, VTArg x, VTArg m, typename std::enable_if<!Flags::IsStreaming, void *>::type = nullptr) { _mm256_maskstore(mem, m, x); }
            template<typename Flags> static Vc_ALWAYS_INLINE void store(double *mem, VTArg x, VTArg m, typename std::enable_if< Flags::IsStreaming, void *>::type = nullptr) { AvxIntrinsics::stream_store(mem, x, m); }

            static VectorType cdab(VTArg x) { return _mm256_permute_pd(x, 5); }
            static VectorType badc(VTArg x) { return _mm256_permute2f128_pd(x, x, 1); }
            // aaaa bbbb cccc dddd specialized in vector.tcc
            static VectorType dacb(VTArg x) {
                const __m128d cb = avx_cast<__m128d>(_mm_alignr_epi8(avx_cast<__m128i>(lo128(x)),
                            avx_cast<__m128i>(hi128(x)), sizeof(double))); // XXX: lo and hi swapped?
                const __m128d da = _mm_blend_pd(lo128(x), hi128(x), 0 + 2); // XXX: lo and hi swapped?
                return concat(cb, da);
            }

            OP0(allone, setallone_pd())
            OP0(zero, _mm256_setzero_pd())
            OP2(or_, _mm256_or_pd(a, b))
            OP2(xor_, _mm256_xor_pd(a, b))
            OP2(and_, _mm256_and_pd(a, b))
            OP2(andnot_, _mm256_andnot_pd(a, b))
            OP3(blend, _mm256_blendv_pd(a, b, c))
        };

        template<> struct VectorHelper<__m256i>
        {
            typedef __m256i VectorType;
#ifdef VC_PASSING_VECTOR_BY_VALUE_IS_BROKEN
            typedef const VectorType & VTArg;
#else
            typedef const VectorType VTArg;
#endif

            template<typename Flags, typename T> static Vc_ALWAYS_INLINE void store(T *mem, VTArg x, typename Flags::EnableIfAligned               = nullptr) { _mm256_store_si256(reinterpret_cast<__m256i *>(mem), x); }
            template<typename Flags, typename T> static Vc_ALWAYS_INLINE void store(T *mem, VTArg x, typename Flags::EnableIfUnalignedNotStreaming = nullptr) { _mm256_storeu_si256(reinterpret_cast<__m256i *>(mem), x); }
            template<typename Flags, typename T> static Vc_ALWAYS_INLINE void store(T *mem, VTArg x, typename Flags::EnableIfStreaming             = nullptr) { _mm256_stream_si256(reinterpret_cast<__m256i *>(mem), x); }
            template<typename Flags, typename T> static Vc_ALWAYS_INLINE void store(T *mem, VTArg x, typename Flags::EnableIfUnalignedAndStreaming = nullptr) { AvxIntrinsics::stream_store(mem, x, setallone_si256()); }

            template<typename Flags, typename T> static Vc_ALWAYS_INLINE void store(T *mem, VTArg x, VTArg m, typename std::enable_if<!Flags::IsStreaming, void *>::type = nullptr) { _mm256_maskstore(mem, m, x); }
            template<typename Flags, typename T> static Vc_ALWAYS_INLINE void store(T *mem, VTArg x, VTArg m, typename std::enable_if< Flags::IsStreaming, void *>::type = nullptr) { AvxIntrinsics::stream_store(mem, x, m); }

            static VectorType cdab(VTArg x) { return avx_cast<VectorType>(_mm256_permute_ps(avx_cast<__m256>(x), _MM_SHUFFLE(2, 3, 0, 1))); }
            static VectorType badc(VTArg x) { return avx_cast<VectorType>(_mm256_permute_ps(avx_cast<__m256>(x), _MM_SHUFFLE(1, 0, 3, 2))); }
            static VectorType aaaa(VTArg x) { return avx_cast<VectorType>(_mm256_permute_ps(avx_cast<__m256>(x), _MM_SHUFFLE(0, 0, 0, 0))); }
            static VectorType bbbb(VTArg x) { return avx_cast<VectorType>(_mm256_permute_ps(avx_cast<__m256>(x), _MM_SHUFFLE(1, 1, 1, 1))); }
            static VectorType cccc(VTArg x) { return avx_cast<VectorType>(_mm256_permute_ps(avx_cast<__m256>(x), _MM_SHUFFLE(2, 2, 2, 2))); }
            static VectorType dddd(VTArg x) { return avx_cast<VectorType>(_mm256_permute_ps(avx_cast<__m256>(x), _MM_SHUFFLE(3, 3, 3, 3))); }
            static VectorType dacb(VTArg x) { return avx_cast<VectorType>(_mm256_permute_ps(avx_cast<__m256>(x), _MM_SHUFFLE(3, 0, 2, 1))); }

            OP0(allone, setallone_si256())
            OP0(zero, _mm256_setzero_si256())
            OP2(or_, or_si256(a, b))
            OP2(xor_, xor_si256(a, b))
            OP2(and_, and_si256(a, b))
            OP2(andnot_, andnot_si256(a, b))
            OP3(blend, blendv_epi8(a, b, c))
        };

        template<> struct VectorHelper<__m128i>
        {
            typedef __m128i VectorType;
#ifdef VC_PASSING_VECTOR_BY_VALUE_IS_BROKEN
            typedef const VectorType & VTArg;
#else
            typedef const VectorType VTArg;
#endif

            template<typename Flags, typename T> static Vc_ALWAYS_INLINE void store(T *mem, VTArg x, typename Flags::EnableIfAligned               = nullptr) { _mm_store_si128(reinterpret_cast<__m128i *>(mem), x); }
            template<typename Flags, typename T> static Vc_ALWAYS_INLINE void store(T *mem, VTArg x, typename Flags::EnableIfUnalignedNotStreaming = nullptr) { _mm_storeu_si128(reinterpret_cast<__m128i *>(mem), x); }
            template<typename Flags, typename T> static Vc_ALWAYS_INLINE void store(T *mem, VTArg x, typename Flags::EnableIfStreaming             = nullptr) { _mm_stream_si128(reinterpret_cast<__m128i *>(mem), x); }
            template<typename Flags, typename T> static Vc_ALWAYS_INLINE void store(T *mem, VTArg x, typename Flags::EnableIfUnalignedAndStreaming = nullptr) { _mm_maskmoveu_si128(x, _mm_setallone_si128(), reinterpret_cast<char *>(mem)); }

            // _mm_maskstore_* only works for 64bit and 32bit types. For shorts we have to resort to _mm_maskmoveu_si128
            template<typename Flags, typename T> static Vc_ALWAYS_INLINE void store(T *mem, VTArg x, VTArg m) { _mm_maskmoveu_si128(x, m, reinterpret_cast<char *>(mem)); }

            static VectorType cdab(VTArg x) { const __m128i tmp = _mm_shufflelo_epi16(x, _MM_SHUFFLE(2, 3, 0, 1)); return _mm_shufflehi_epi16(tmp, _MM_SHUFFLE(2, 3, 0, 1)); }
            static VectorType badc(VTArg x) { const __m128i tmp = _mm_shufflelo_epi16(x, _MM_SHUFFLE(1, 0, 3, 2)); return _mm_shufflehi_epi16(tmp, _MM_SHUFFLE(1, 0, 3, 2)); }
            static VectorType aaaa(VTArg x) { const __m128i tmp = _mm_shufflelo_epi16(x, _MM_SHUFFLE(0, 0, 0, 0)); return _mm_shufflehi_epi16(tmp, _MM_SHUFFLE(0, 0, 0, 0)); }
            static VectorType bbbb(VTArg x) { const __m128i tmp = _mm_shufflelo_epi16(x, _MM_SHUFFLE(1, 1, 1, 1)); return _mm_shufflehi_epi16(tmp, _MM_SHUFFLE(1, 1, 1, 1)); }
            static VectorType cccc(VTArg x) { const __m128i tmp = _mm_shufflelo_epi16(x, _MM_SHUFFLE(2, 2, 2, 2)); return _mm_shufflehi_epi16(tmp, _MM_SHUFFLE(2, 2, 2, 2)); }
            static VectorType dddd(VTArg x) { const __m128i tmp = _mm_shufflelo_epi16(x, _MM_SHUFFLE(3, 3, 3, 3)); return _mm_shufflehi_epi16(tmp, _MM_SHUFFLE(3, 3, 3, 3)); }
            static VectorType dacb(VTArg x) { const __m128i tmp = _mm_shufflelo_epi16(x, _MM_SHUFFLE(3, 0, 2, 1)); return _mm_shufflehi_epi16(tmp, _MM_SHUFFLE(3, 0, 2, 1)); }

            OP0(allone, _mm_setallone_si128())
            OP0(zero, _mm_setzero_si128())
            OP2(or_, _mm_or_si128(a, b))
            OP2(xor_, _mm_xor_si128(a, b))
            OP2(and_, _mm_and_si128(a, b))
            OP2(andnot_, _mm_andnot_si128(a, b))
            OP3(blend, _mm_blendv_epi8(a, b, c))
        };
#undef OP1
#undef OP2
#undef OP3

#define OP1(op) \
        static Vc_INTRINSIC VectorType Vc_CONST op(VTArg a) { return CAT(_mm256_##op##_, SUFFIX)(a); }
#define OP(op) \
        static Vc_INTRINSIC VectorType Vc_CONST op(VTArg a, VTArg b) { return CAT(op##_ , SUFFIX)(a, b); }
#define OP_(op) \
        static Vc_INTRINSIC VectorType Vc_CONST op(VTArg a, VTArg b) { return CAT(_mm256_##op    , SUFFIX)(a, b); }
#define OPx(op, op2) \
        static Vc_INTRINSIC VectorType Vc_CONST op(VTArg a, VTArg b) { return CAT(_mm256_##op2##_, SUFFIX)(a, b); }
#define OPcmp(op) \
        static Vc_INTRINSIC VectorType Vc_CONST cmp##op(VTArg a, VTArg b) { return CAT(cmp##op##_, SUFFIX)(a, b); }
#define OP_CAST_(op) \
        static Vc_INTRINSIC VectorType Vc_CONST op(VTArg a, VTArg b) { return CAT(_mm256_castps_, SUFFIX)( \
            _mm256_##op##ps(CAT(CAT(_mm256_cast, SUFFIX), _ps)(a), \
              CAT(CAT(_mm256_cast, SUFFIX), _ps)(b))); \
        }
#define MINMAX \
        static Vc_INTRINSIC VectorType Vc_CONST min(VTArg a, VTArg b) { return CAT(min_, SUFFIX)(a, b); } \
        static Vc_INTRINSIC VectorType Vc_CONST max(VTArg a, VTArg b) { return CAT(max_, SUFFIX)(a, b); }

        template<> struct VectorHelper<double> {
            typedef __m256d VectorType;
#ifdef VC_PASSING_VECTOR_BY_VALUE_IS_BROKEN
            typedef const VectorType & VTArg;
#else
            typedef const VectorType VTArg;
#endif
            typedef double EntryType;
#define SUFFIX pd

            static Vc_ALWAYS_INLINE VectorType notMaskedToZero(VTArg a, __m256 mask) { return CAT(_mm256_and_, SUFFIX)(_mm256_castps_pd(mask), a); }
            static Vc_ALWAYS_INLINE VectorType set(const double a) { return CAT(_mm256_set1_, SUFFIX)(a); }
            static Vc_ALWAYS_INLINE VectorType set(const double a, const double b, const double c, const double d) {
                return CAT(_mm256_set_, SUFFIX)(a, b, c, d);
            }
            static Vc_ALWAYS_INLINE VectorType zero() { return CAT(_mm256_setzero_, SUFFIX)(); }
            static Vc_ALWAYS_INLINE VectorType one()  { return CAT(setone_, SUFFIX)(); }// set(1.); }

            static inline void fma(VectorType &v1, VTArg v2, VTArg v3) {
#ifdef VC_IMPL_FMA4
                v1 = _mm256_macc_pd(v1, v2, v3);
#else
                VectorType h1 = _mm256_and_pd(v1, _mm256_broadcast_sd(reinterpret_cast<const double *>(&c_general::highMaskDouble)));
                VectorType h2 = _mm256_and_pd(v2, _mm256_broadcast_sd(reinterpret_cast<const double *>(&c_general::highMaskDouble)));
#if defined(VC_GCC) && VC_GCC < 0x40703
                // GCC before 4.7.3 uses an incorrect optimization where it replaces the subtraction with an andnot
                // http://gcc.gnu.org/bugzilla/show_bug.cgi?id=54703
                asm("":"+x"(h1), "+x"(h2));
#endif
                const VectorType l1 = _mm256_sub_pd(v1, h1);
                const VectorType l2 = _mm256_sub_pd(v2, h2);
                const VectorType ll = mul(l1, l2);
                const VectorType lh = add(mul(l1, h2), mul(h1, l2));
                const VectorType hh = mul(h1, h2);
                // ll < lh < hh for all entries is certain
                const VectorType lh_lt_v3 = cmplt(abs(lh), abs(v3)); // |lh| < |v3|
                const VectorType b = _mm256_blendv_pd(v3, lh, lh_lt_v3);
                const VectorType c = _mm256_blendv_pd(lh, v3, lh_lt_v3);
                v1 = add(add(ll, b), add(c, hh));
#endif
            }

            static Vc_INTRINSIC VectorType Vc_CONST add(VTArg a, VTArg b) { return _mm256_add_pd(a,b); }
            static Vc_INTRINSIC VectorType Vc_CONST sub(VTArg a, VTArg b) { return _mm256_sub_pd(a,b); }
            static Vc_INTRINSIC VectorType Vc_CONST mul(VTArg a, VTArg b) { return _mm256_mul_pd(a,b); }
            OPcmp(eq) OPcmp(neq)
            OPcmp(lt) OPcmp(nlt)
            OPcmp(le) OPcmp(nle)

            OP1(sqrt)
            static Vc_ALWAYS_INLINE Vc_CONST VectorType rsqrt(VTArg x) {
                return _mm256_div_pd(one(), sqrt(x));
            }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType reciprocal(VTArg x) {
                return _mm256_div_pd(one(), x);
            }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType isNaN(VTArg x) {
                return cmpunord_pd(x, x);
            }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType isFinite(VTArg x) {
                return cmpord_pd(x, _mm256_mul_pd(zero(), x));
            }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType isInfinite(VectorType x) {
                return _mm256_castsi256_pd(cmpeq_epi64(_mm256_castpd_si256(abs(x)), _mm256_castpd_si256(set1_pd(c_log<double>::d(1)))));
            }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType abs(VTArg a) {
                return CAT(_mm256_and_, SUFFIX)(a, setabsmask_pd());
            }

            static Vc_INTRINSIC VectorType Vc_CONST min(VTArg a, VTArg b) { return _mm256_min_pd(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST max(VTArg a, VTArg b) { return _mm256_max_pd(a, b); }
            static Vc_ALWAYS_INLINE Vc_CONST EntryType min(VTArg a) {
                __m128d b = _mm_min_pd(avx_cast<__m128d>(a), _mm256_extractf128_pd(a, 1));
                b = _mm_min_sd(b, _mm_unpackhi_pd(b, b));
                return _mm_cvtsd_f64(b);
            }
            static Vc_ALWAYS_INLINE Vc_CONST EntryType max(VTArg a) {
                __m128d b = _mm_max_pd(avx_cast<__m128d>(a), _mm256_extractf128_pd(a, 1));
                b = _mm_max_sd(b, _mm_unpackhi_pd(b, b));
                return _mm_cvtsd_f64(b);
            }
            static Vc_ALWAYS_INLINE Vc_CONST EntryType mul(VTArg a) {
                __m128d b = _mm_mul_pd(avx_cast<__m128d>(a), _mm256_extractf128_pd(a, 1));
                b = _mm_mul_sd(b, _mm_shuffle_pd(b, b, _MM_SHUFFLE2(0, 1)));
                return _mm_cvtsd_f64(b);
            }
            static Vc_ALWAYS_INLINE Vc_CONST EntryType add(VTArg a) {
                __m128d b = _mm_add_pd(avx_cast<__m128d>(a), _mm256_extractf128_pd(a, 1));
                b = _mm_hadd_pd(b, b); // or: b = _mm_add_sd(b, _mm256_shuffle_pd(b, b, _MM_SHUFFLE2(0, 1)));
                return _mm_cvtsd_f64(b);
            }
#undef SUFFIX
            static Vc_ALWAYS_INLINE Vc_CONST VectorType round(VTArg a) {
                return _mm256_round_pd(a, _MM_FROUND_NINT);
            }
        };

        template<> struct VectorHelper<float> {
            typedef float EntryType;
            typedef __m256 VectorType;
#ifdef VC_PASSING_VECTOR_BY_VALUE_IS_BROKEN
            typedef const VectorType & VTArg;
#else
            typedef const VectorType VTArg;
#endif
#define SUFFIX ps

            static Vc_ALWAYS_INLINE Vc_CONST VectorType notMaskedToZero(VTArg a, __m256 mask) { return CAT(_mm256_and_, SUFFIX)(mask, a); }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType set(const float a) { return CAT(_mm256_set1_, SUFFIX)(a); }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType set(const float a, const float b, const float c, const float d,
                    const float e, const float f, const float g, const float h) {
                return CAT(_mm256_set_, SUFFIX)(a, b, c, d, e, f, g, h); }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType zero() { return CAT(_mm256_setzero_, SUFFIX)(); }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType one()  { return CAT(setone_, SUFFIX)(); }// set(1.f); }
            static Vc_ALWAYS_INLINE Vc_CONST __m256 concat(__m256d a, __m256d b) { return _mm256_insertf128_ps(avx_cast<__m256>(_mm256_cvtpd_ps(a)), _mm256_cvtpd_ps(b), 1); }

            static inline void fma(VectorType &v1, VTArg v2, VTArg v3) {
#ifdef VC_IMPL_FMA4
                v1 = _mm256_macc_ps(v1, v2, v3);
#else
                __m256d v1_0 = _mm256_cvtps_pd(lo128(v1));
                __m256d v1_1 = _mm256_cvtps_pd(hi128(v1));
                __m256d v2_0 = _mm256_cvtps_pd(lo128(v2));
                __m256d v2_1 = _mm256_cvtps_pd(hi128(v2));
                __m256d v3_0 = _mm256_cvtps_pd(lo128(v3));
                __m256d v3_1 = _mm256_cvtps_pd(hi128(v3));
                v1 = AVX::concat(
                        _mm256_cvtpd_ps(_mm256_add_pd(_mm256_mul_pd(v1_0, v2_0), v3_0)),
                        _mm256_cvtpd_ps(_mm256_add_pd(_mm256_mul_pd(v1_1, v2_1), v3_1)));
#endif
            }

            static Vc_INTRINSIC VectorType Vc_CONST add(VTArg a, VTArg b) { return _mm256_add_ps(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST sub(VTArg a, VTArg b) { return _mm256_sub_ps(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST mul(VTArg a, VTArg b) { return _mm256_mul_ps(a, b); }
            OPcmp(eq) OPcmp(neq)
            OPcmp(lt) OPcmp(nlt)
            OPcmp(le) OPcmp(nle)

            OP1(sqrt) OP1(rsqrt)
            static Vc_ALWAYS_INLINE Vc_CONST VectorType isNaN(VTArg x) {
                return cmpunord_ps(x, x);
            }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType isFinite(VTArg x) {
                return cmpord_ps(x, _mm256_mul_ps(zero(), x));
            }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType isInfinite(VectorType x) {
                return _mm256_castsi256_ps(cmpeq_epi32(_mm256_castps_si256(abs(x)), _mm256_castps_si256(set1_ps(c_log<float>::d(1)))));
            }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType reciprocal(VTArg x) {
                return _mm256_rcp_ps(x);
            }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType abs(VTArg a) {
                return CAT(_mm256_and_, SUFFIX)(a, setabsmask_ps());
            }

            static Vc_INTRINSIC VectorType Vc_CONST min(VTArg a, VTArg b) { return _mm256_min_ps(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST max(VTArg a, VTArg b) { return _mm256_max_ps(a, b); }
            static Vc_ALWAYS_INLINE Vc_CONST EntryType min(VTArg a) {
                __m128 b = _mm_min_ps(lo128(a), hi128(a));
                b = _mm_min_ps(b, _mm_movehl_ps(b, b));   // b = min(a0, a2), min(a1, a3), min(a2, a2), min(a3, a3)
                b = _mm_min_ss(b, _mm_shuffle_ps(b, b, _MM_SHUFFLE(1, 1, 1, 1))); // b = min(a0, a1), a1, a2, a3
                return _mm_cvtss_f32(b);
            }
            static Vc_ALWAYS_INLINE Vc_CONST EntryType max(VTArg a) {
                __m128 b = _mm_max_ps(avx_cast<__m128>(a), _mm256_extractf128_ps(a, 1));
                b = _mm_max_ps(b, _mm_movehl_ps(b, b));   // b = max(a0, a2), max(a1, a3), max(a2, a2), max(a3, a3)
                b = _mm_max_ss(b, _mm_shuffle_ps(b, b, _MM_SHUFFLE(1, 1, 1, 1))); // b = max(a0, a1), a1, a2, a3
                return _mm_cvtss_f32(b);
            }
            static Vc_ALWAYS_INLINE Vc_CONST EntryType mul(VTArg a) {
                __m128 b = _mm_mul_ps(avx_cast<__m128>(a), _mm256_extractf128_ps(a, 1));
                b = _mm_mul_ps(b, _mm_shuffle_ps(b, b, _MM_SHUFFLE(0, 1, 2, 3)));
                b = _mm_mul_ss(b, _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 2, 0, 1)));
                return _mm_cvtss_f32(b);
            }
            static Vc_ALWAYS_INLINE Vc_CONST EntryType add(VTArg a) {
                __m128 b = _mm_add_ps(avx_cast<__m128>(a), _mm256_extractf128_ps(a, 1));
                b = _mm_add_ps(b, _mm_shuffle_ps(b, b, _MM_SHUFFLE(0, 1, 2, 3)));
                b = _mm_add_ss(b, _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 2, 0, 1)));
                return _mm_cvtss_f32(b);
            }
#undef SUFFIX
            static Vc_ALWAYS_INLINE Vc_CONST VectorType round(VTArg a) {
                return _mm256_round_ps(a, _MM_FROUND_NINT);
            }
        };

        template<> struct VectorHelper<int> {
            typedef int EntryType;
            typedef __m128i VectorType;
#ifdef VC_PASSING_VECTOR_BY_VALUE_IS_BROKEN
            typedef const VectorType & VTArg;
#else
            typedef const VectorType VTArg;
#endif
#define SUFFIX si128

            static Vc_INTRINSIC Vc_CONST VectorType or_(VTArg a, VTArg b) { return _mm_or_si128(a, b); }
            static Vc_INTRINSIC Vc_CONST VectorType and_(VTArg a, VTArg b) { return _mm_and_si128(a, b); }
            static Vc_INTRINSIC Vc_CONST VectorType xor_(VTArg a, VTArg b) { return _mm_xor_si128(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST zero() { return CAT(_mm_setzero_, SUFFIX)(); }
            static Vc_INTRINSIC VectorType Vc_CONST notMaskedToZero(VTArg a, __m128 mask) { return CAT(_mm_and_, SUFFIX)(_mm_castps_si128(mask), a); }
#undef SUFFIX
#define SUFFIX epi32
            static Vc_INTRINSIC VectorType Vc_CONST one() { return CAT(_mm_setone_, SUFFIX)(); }

            static Vc_INTRINSIC VectorType Vc_CONST set(const int a) { return CAT(_mm_set1_, SUFFIX)(a); }
            static Vc_INTRINSIC VectorType Vc_CONST set(const int a, const int b, const int c, const int d)
            {
                return CAT(_mm_set_, SUFFIX)(a, b, c, d);
            }

            static Vc_INTRINSIC void fma(VectorType &v1, VTArg v2, VTArg v3) { v1 = add(mul(v1, v2), v3); }

            template <int shift>
            static Vc_ALWAYS_INLINE Vc_CONST VectorType shiftLeft(VTArg a)
            {
                return slli_epi32<shift>(a);
            }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType shiftLeft(VTArg a, int shift)
            {
                return sll_epi32(a, _mm_cvtsi32_si128(shift));
            }
            template <int shift>
            static Vc_ALWAYS_INLINE Vc_CONST VectorType shiftRight(VTArg a)
            {
                return srai_epi32<shift>(a);
            }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType shiftRight(VTArg a, int shift)
            {
                return sra_epi32(a, _mm_cvtsi32_si128(shift));
            }
            static Vc_INTRINSIC Vc_CONST VectorType abs(VTArg a) { return _mm_abs_epi32(a); }

            static Vc_INTRINSIC VectorType Vc_CONST min(VTArg a, VTArg b) { return CAT(_mm_min_, SUFFIX)(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST max(VTArg a, VTArg b) { return CAT(_mm_max_, SUFFIX)(a, b); }
            static Vc_INTRINSIC EntryType Vc_CONST min(VTArg a) {
                auto b = _mm_min_epi32(a, _mm_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                b = _mm_min_epi32(b, _mm_shufflelo_epi16(b, _MM_SHUFFLE(1, 0, 3, 2))); // using lo_epi16 for speed here
                return _mm_cvtsi128_si32(b);
            }
            static Vc_INTRINSIC EntryType Vc_CONST max(VTArg a) {
                auto b = _mm_max_epi32(a, _mm_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                b = _mm_max_epi32(b, _mm_shufflelo_epi16(b, _MM_SHUFFLE(1, 0, 3, 2))); // using lo_epi16 for speed here
                return _mm_cvtsi128_si32(b);
            }
            static Vc_INTRINSIC EntryType Vc_CONST add(VTArg a) {
                auto b = _mm_add_epi32(a, _mm_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                b = _mm_add_epi32(b, _mm_shufflelo_epi16(b, _MM_SHUFFLE(1, 0, 3, 2)));
                return _mm_cvtsi128_si32(b);
            }
            static Vc_INTRINSIC EntryType Vc_CONST mul(VTArg a) {
                auto b = _mm_mullo_epi32(a, _mm_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                b = _mm_mullo_epi32(b, _mm_shufflelo_epi16(b, _MM_SHUFFLE(1, 0, 3, 2)));
                return _mm_cvtsi128_si32(b);
            }

            static Vc_INTRINSIC VectorType Vc_CONST mul(VTArg a, VTArg b) { return _mm_mullo_epi32(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST add(VTArg a, VTArg b) { return _mm_add_epi32(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST sub(VTArg a, VTArg b) { return _mm_sub_epi32(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST cmpeq(VTArg a, VTArg b) { return _mm_cmpeq_epi32(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST cmpneq(VTArg a, VTArg b) { return _mm_andnot_si128(cmpeq(a, b), _mm_setallone_si128()); }
            static Vc_INTRINSIC VectorType Vc_CONST cmplt(VTArg a, VTArg b) { return _mm_cmplt_epi32(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST cmpnlt(VTArg a, VTArg b) { return _mm_andnot_si128(cmplt(a, b), _mm_setallone_si128()); }
            static Vc_INTRINSIC VectorType Vc_CONST cmpgt(VTArg a, VTArg b) { return _mm_cmpgt_epi32(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST cmple (VTArg a, VTArg b) { return _mm_andnot_si128(cmpgt(a, b), _mm_setallone_si128()); }
            static Vc_INTRINSIC VectorType Vc_CONST cmpnle(VTArg a, VTArg b) { return cmpgt(a, b); }
#undef SUFFIX
            static Vc_INTRINSIC VectorType Vc_CONST round(VTArg a) { return a; }
        };

        template<> struct VectorHelper<unsigned int> {
            typedef unsigned int EntryType;
            typedef __m128i VectorType;
#ifdef VC_PASSING_VECTOR_BY_VALUE_IS_BROKEN
            typedef const VectorType & VTArg;
#else
            typedef const VectorType VTArg;
#endif
#define SUFFIX si128
            static Vc_INTRINSIC Vc_CONST VectorType or_(VTArg a, VTArg b) { return _mm_or_si128(a, b); }
            static Vc_INTRINSIC Vc_CONST VectorType and_(VTArg a, VTArg b) { return _mm_and_si128(a, b); }
            static Vc_INTRINSIC Vc_CONST VectorType xor_(VTArg a, VTArg b) { return _mm_xor_si128(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST zero() { return CAT(_mm_setzero_, SUFFIX)(); }
            static Vc_INTRINSIC VectorType Vc_CONST notMaskedToZero(VTArg a, __m128 mask) { return CAT(_mm_and_, SUFFIX)(_mm_castps_si128(mask), a); }

#undef SUFFIX
#define SUFFIX epu32
            static Vc_INTRINSIC VectorType Vc_CONST one() { return CAT(_mm_setone_, SUFFIX)(); }

            static Vc_INTRINSIC Vc_CONST VectorType min(VTArg a, VTArg b) { return _mm_min_epu32(a, b); }
            static Vc_INTRINSIC Vc_CONST VectorType max(VTArg a, VTArg b) { return _mm_max_epu32(a, b); }
            static Vc_INTRINSIC EntryType Vc_CONST min(VTArg a) {
                auto b = _mm_min_epu32(a, _mm_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                b = _mm_min_epu32(b, _mm_shufflelo_epi16(b, _MM_SHUFFLE(1, 0, 3, 2))); // using lo_epi16 for speed here
                return _mm_cvtsi128_si32(b);
            }
            static Vc_INTRINSIC EntryType Vc_CONST max(VTArg a) {
                auto b = _mm_max_epu32(a, _mm_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                b = _mm_max_epu32(b, _mm_shufflelo_epi16(b, _MM_SHUFFLE(1, 0, 3, 2))); // using lo_epi16 for speed here
                return _mm_cvtsi128_si32(b);
            }
            static Vc_INTRINSIC EntryType Vc_CONST add(VTArg a) {
                auto b = _mm_add_epi32(a, _mm_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                b = _mm_add_epi32(b, _mm_shufflelo_epi16(b, _MM_SHUFFLE(1, 0, 3, 2)));
                return _mm_cvtsi128_si32(b);
            }
            static Vc_INTRINSIC EntryType Vc_CONST mul(VTArg a) {
                auto b = _mm_mullo_epi32(a, _mm_shuffle_epi32(a, _MM_SHUFFLE(1, 0, 3, 2)));
                b = _mm_mullo_epi32(b, _mm_shufflelo_epi16(b, _MM_SHUFFLE(1, 0, 3, 2)));
                return _mm_cvtsi128_si32(b);
            }

            static Vc_INTRINSIC VectorType Vc_CONST abs(VTArg a) { return a; }
            static Vc_INTRINSIC VectorType Vc_CONST mul(VTArg a, VTArg b) { return _mm_mullo_epi32(a, b); }
            static Vc_INTRINSIC void fma(VectorType &v1, VTArg v2, VTArg v3) { v1 = add(mul(v1, v2), v3); }

#undef SUFFIX
#define SUFFIX epi32
            template <int shift>
            static Vc_ALWAYS_INLINE Vc_CONST VectorType shiftLeft(VTArg a)
            {
                return _mm_slli_epi32(a, shift);
            }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType shiftLeft(VTArg a, int shift)
            {
                return _mm_sll_epi32(a, _mm_cvtsi32_si128(shift));
            }
            template <int shift>
            static Vc_ALWAYS_INLINE Vc_CONST VectorType shiftRight(VTArg a)
            {
                return _mm_srli_epi32(a, shift);
            }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType shiftRight(VTArg a, int shift)
            {
                return _mm_srl_epi32(a, _mm_cvtsi32_si128(shift));
            }
            static Vc_INTRINSIC VectorType Vc_CONST set(const unsigned int a) { return CAT(_mm_set1_, SUFFIX)(a); }
            static Vc_INTRINSIC VectorType Vc_CONST set(const unsigned int a, const unsigned int b, const unsigned int c, const unsigned int d) {
                return CAT(_mm_set_, SUFFIX)(a, b, c, d);
            }

            static Vc_INTRINSIC Vc_CONST VectorType add(VTArg a, VTArg b) { return _mm_add_epi32(a, b); }
            static Vc_INTRINSIC Vc_CONST VectorType sub(VTArg a, VTArg b) { return _mm_sub_epi32(a, b); }
            static Vc_INTRINSIC Vc_CONST VectorType cmpeq(VTArg a, VTArg b) { return _mm_cmpeq_epi32(a, b); }
            static Vc_INTRINSIC Vc_CONST VectorType cmpneq(VTArg a, VTArg b) { return _mm_andnot_si128(cmpeq(a, b), _mm_setallone_si128()); }

#ifndef USE_INCORRECT_UNSIGNED_COMPARE
            static Vc_INTRINSIC VectorType Vc_CONST cmplt(VTArg a, VTArg b) {
                return SseIntrinsics::_mm_cmplt_epu32(a, b);
            }
            static Vc_INTRINSIC VectorType Vc_CONST cmpgt(VTArg a, VTArg b) {
                return SseIntrinsics::_mm_cmpgt_epu32(a, b);
            }
#else
            static Vc_INTRINSIC Vc_CONST VectorType cmplt(VTArg a, VTArg b) { return _mm_cmplt_epi32(a, b); }
            static Vc_INTRINSIC Vc_CONST VectorType cmpgt(VTArg a, VTArg b) { return _mm_cmpgt_epi32(a, b); }
#endif
            static Vc_INTRINSIC VectorType Vc_CONST cmpnlt(VTArg a, VTArg b) { return _mm_andnot_si128(cmplt(a, b), _mm_setallone_si128()); }
            static Vc_INTRINSIC VectorType Vc_CONST cmple (VTArg a, VTArg b) { return _mm_andnot_si128(cmpgt(a, b), _mm_setallone_si128()); }
            static Vc_INTRINSIC VectorType Vc_CONST cmpnle(VTArg a, VTArg b) { return cmpgt(a, b); }

#undef SUFFIX
            static Vc_INTRINSIC VectorType Vc_CONST round(VTArg a) { return a; }
        };

#ifndef VC_IMPL_AVX2
#undef OP1
#undef OP
#undef OP_
#undef OPx
#undef OPcmp
#undef OP_CAST_
#undef MINMAX
#else
        template<> struct VectorHelper<signed short> {
            typedef VectorTypeHelper<signed short>::Type VectorType;
#ifdef VC_PASSING_VECTOR_BY_VALUE_IS_BROKEN
            typedef const VectorType & VTArg;
#else
            typedef const VectorType VTArg;
#endif
            typedef signed short EntryType;

            static Vc_INTRINSIC VectorType Vc_CONST or_(VTArg a, VTArg b) { return _mm_or_si128(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST and_(VTArg a, VTArg b) { return _mm_and_si128(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST xor_(VTArg a, VTArg b) { return _mm_xor_si128(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST zero() { return _mm_setzero_si128(); }
            static Vc_INTRINSIC VectorType Vc_CONST notMaskedToZero(VTArg a, __m128 mask) { return _mm_and_si128(_mm_castps_si128(mask), a); }

#define SUFFIX epi16
            static Vc_INTRINSIC VectorType Vc_CONST one() { return CAT(_mm_setone_, SUFFIX)(); }

            template <int shift>
            static Vc_ALWAYS_INLINE Vc_CONST VectorType shiftLeft(VTArg a)
            {
                return CAT(_mm_slli_, SUFFIX)(a, shift);
            }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType shiftLeft(VTArg a, int shift)
            {
                return sll_epi16(a, _mm_cvtsi32_si128(shift));
            }
            template <int shift>
            static Vc_ALWAYS_INLINE Vc_CONST VectorType shiftRight(VTArg a)
            {
                return CAT(_mm_srai_, SUFFIX)(a, shift);
            }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType shiftRight(VTArg a, int shift)
            {
                return sra_epi16(a, _mm_cvtsi32_si128(shift));
            }
            static Vc_INTRINSIC VectorType Vc_CONST set(const EntryType a) { return CAT(_mm_set1_, SUFFIX)(a); }
            static Vc_INTRINSIC VectorType Vc_CONST set(const EntryType a, const EntryType b, const EntryType c, const EntryType d,
                    const EntryType e, const EntryType f, const EntryType g, const EntryType h) {
                return CAT(_mm_set_, SUFFIX)(a, b, c, d, e, f, g, h);
            }

            static Vc_INTRINSIC void fma(VectorType &v1, VTArg v2, VTArg v3) {
                v1 = add(mul(v1, v2), v3);
            }

            static Vc_INTRINSIC VectorType Vc_CONST abs(VTArg a) { return _mm_abs_epi16(a); }
            static Vc_INTRINSIC VectorType Vc_CONST mul(VTArg a, VTArg b) { return _mm_mullo_epi16(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST min(VTArg a, VTArg b) { return _mm_min_epi16(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST max(VTArg a, VTArg b) { return _mm_max_epi16(a, b); }

            static Vc_INTRINSIC EntryType Vc_CONST min(VTArg _a) {
                // reminder: _MM_SHUFFLE(3, 2, 1, 0) means "no change"
                VectorType a = min(_a, _mm_shuffle_epi32(_a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = min(a, _mm_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = min(a, _mm_shufflelo_epi16(a, _MM_SHUFFLE(1, 1, 1, 1)));
                return _mm_cvtsi128_si32(a); // & 0xffff is implicit
            }
            static Vc_INTRINSIC EntryType Vc_CONST max(VTArg _a) {
                // reminder: _MM_SHUFFLE(3, 2, 1, 0) means "no change"
                VectorType a = max(_a, _mm_shuffle_epi32(_a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = max(a, _mm_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = max(a, _mm_shufflelo_epi16(a, _MM_SHUFFLE(1, 1, 1, 1)));
                return _mm_cvtsi128_si32(a); // & 0xffff is implicit
            }
            static Vc_INTRINSIC EntryType Vc_CONST mul(VTArg _a) {
                VectorType a = mul(_a, _mm_shuffle_epi32(_a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = mul(a, _mm_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = mul(a, _mm_shufflelo_epi16(a, _MM_SHUFFLE(1, 1, 1, 1)));
                return _mm_cvtsi128_si32(a); // & 0xffff is implicit
            }
            static Vc_INTRINSIC EntryType Vc_CONST add(VTArg _a) {
                VectorType a = add(_a, _mm_shuffle_epi32(_a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = add(a, _mm_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = add(a, _mm_shufflelo_epi16(a, _MM_SHUFFLE(1, 1, 1, 1)));
                return _mm_cvtsi128_si32(a); // & 0xffff is implicit
            }

            static Vc_INTRINSIC VectorType Vc_CONST add(VTArg a, VTArg b) { return _mm_add_epi16(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST sub(VTArg a, VTArg b) { return _mm_sub_epi16(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST cmpeq(VTArg a, VTArg b) { return _mm_cmpeq_epi16(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST cmplt(VTArg a, VTArg b) { return _mm_cmplt_epi16(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST cmpgt(VTArg a, VTArg b) { return _mm_cmpgt_epi16(a, b); }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType cmpneq(VTArg a, VTArg b) { __m128i x = cmpeq(a, b); return _mm_andnot_si128(x, _mm_setallone_si128()); }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType cmpnlt(VTArg a, VTArg b) { __m128i x = cmplt(a, b); return _mm_andnot_si128(x, _mm_setallone_si128()); }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType cmple (VTArg a, VTArg b) { __m128i x = cmpgt(a, b); return _mm_andnot_si128(x, _mm_setallone_si128()); }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType cmpnle(VTArg a, VTArg b) { return cmpgt(a, b); }
#undef SUFFIX
            static Vc_ALWAYS_INLINE Vc_CONST VectorType round(VTArg a) { return a; }
        };

        template<> struct VectorHelper<unsigned short> {
            typedef VectorTypeHelper<unsigned short>::Type VectorType;
#ifdef VC_PASSING_VECTOR_BY_VALUE_IS_BROKEN
            typedef const VectorType & VTArg;
#else
            typedef const VectorType VTArg;
#endif
            typedef unsigned short EntryType;

            static Vc_INTRINSIC VectorType Vc_CONST or_(VTArg a, VTArg b) { return _mm_or_si128(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST and_(VTArg a, VTArg b) { return _mm_and_si128(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST xor_(VTArg a, VTArg b) { return _mm_xor_si128(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST zero() { return _mm_setzero_si128(); }
            static Vc_INTRINSIC VectorType Vc_CONST notMaskedToZero(VTArg a, __m128 mask) { return _mm_and_si128(_mm_castps_si128(mask), a); }
            static Vc_INTRINSIC VectorType Vc_CONST one() { return _mm_setone_epu16(); }

            static Vc_INTRINSIC VectorType Vc_CONST mul(VTArg a, VTArg b) { return _mm_mullo_epi16(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST min(VTArg a, VTArg b) { return _mm_min_epu16(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST max(VTArg a, VTArg b) { return _mm_max_epu16(a, b); }

#define SUFFIX epi16
            template <int shift>
            static Vc_ALWAYS_INLINE Vc_CONST VectorType shiftLeft(VTArg a)
            {
                return CAT(_mm_slli_, SUFFIX)(a, shift);
            }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType shiftLeft(VTArg a, int shift)
            {
                return sll_epi16(a, _mm_cvtsi32_si128(shift));
            }
            template <int shift>
            static Vc_ALWAYS_INLINE Vc_CONST VectorType shiftRight(VTArg a)
            {
                return CAT(_mm_srli_, SUFFIX)(a, shift);
            }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType shiftRight(VTArg a, int shift)
            {
                return srl_epi16(a, _mm_cvtsi32_si128(shift));
            }
            static Vc_INTRINSIC EntryType Vc_CONST min(VTArg _a) {
                // reminder: _MM_SHUFFLE(3, 2, 1, 0) means "no change"
                VectorType a = min(_a, _mm_shuffle_epi32(_a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = min(a, _mm_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = min(a, _mm_shufflelo_epi16(a, _MM_SHUFFLE(1, 1, 1, 1)));
                return _mm_cvtsi128_si32(a); // & 0xffff is implicit
            }
            static Vc_INTRINSIC EntryType Vc_CONST max(VTArg _a) {
                // reminder: _MM_SHUFFLE(3, 2, 1, 0) means "no change"
                VectorType a = max(_a, _mm_shuffle_epi32(_a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = max(a, _mm_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = max(a, _mm_shufflelo_epi16(a, _MM_SHUFFLE(1, 1, 1, 1)));
                return _mm_cvtsi128_si32(a); // & 0xffff is implicit
            }
            static Vc_INTRINSIC EntryType Vc_CONST mul(VTArg _a) {
                // reminder: _MM_SHUFFLE(3, 2, 1, 0) means "no change"
                VectorType a = mul(_a, _mm_shuffle_epi32(_a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = mul(a, _mm_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = mul(a, _mm_shufflelo_epi16(a, _MM_SHUFFLE(1, 1, 1, 1)));
                return _mm_cvtsi128_si32(a); // & 0xffff is implicit
            }
            static Vc_INTRINSIC EntryType Vc_CONST add(VTArg _a) {
                // reminder: _MM_SHUFFLE(3, 2, 1, 0) means "no change"
                VectorType a = add(_a, _mm_shuffle_epi32(_a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = add(a, _mm_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2)));
                a = add(a, _mm_shufflelo_epi16(a, _MM_SHUFFLE(1, 1, 1, 1)));
                return _mm_cvtsi128_si32(a); // & 0xffff is implicit
            }
            static Vc_INTRINSIC VectorType Vc_CONST set(const EntryType a) { return CAT(_mm_set1_, SUFFIX)(a); }
            static Vc_INTRINSIC VectorType Vc_CONST set(const EntryType a, const EntryType b, const EntryType c,
                    const EntryType d, const EntryType e, const EntryType f,
                    const EntryType g, const EntryType h) {
                return CAT(_mm_set_, SUFFIX)(a, b, c, d, e, f, g, h);
            }
            static Vc_INTRINSIC void fma(VectorType &v1, VTArg v2, VTArg v3) { v1 = add(mul(v1, v2), v3); }

            static Vc_INTRINSIC VectorType Vc_CONST abs(VTArg a) { return a; }
            static Vc_INTRINSIC VectorType Vc_CONST add(VTArg a, VTArg b) { return _mm_add_epi16(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST sub(VTArg a, VTArg b) { return _mm_sub_epi16(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST cmpeq(VTArg a, VTArg b) { return _mm_cmpeq_epi16(a, b); }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType cmpneq(VTArg a, VTArg b) { return _mm_andnot_si128(cmpeq(a, b), _mm_setallone_si128()); }

#ifndef USE_INCORRECT_UNSIGNED_COMPARE
            static Vc_INTRINSIC VectorType Vc_CONST cmplt(VTArg a, VTArg b) { return cmplt_epu16(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST cmpgt(VTArg a, VTArg b) { return cmpgt_epu16(a, b); }
#else
            static Vc_INTRINSIC VectorType Vc_CONST cmplt(VTArg a, VTArg b) { return _mm_cmplt_epi16(a, b); }
            static Vc_INTRINSIC VectorType Vc_CONST cmpgt(VTArg a, VTArg b) { return _mm_cmpgt_epi16(a, b); }
#endif
            static Vc_ALWAYS_INLINE Vc_CONST VectorType cmpnlt(VTArg a, VTArg b) { return _mm_andnot_si128(cmplt(a, b), _mm_setallone_si128()); }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType cmple (VTArg a, VTArg b) { return _mm_andnot_si128(cmpgt(a, b), _mm_setallone_si128()); }
            static Vc_ALWAYS_INLINE Vc_CONST VectorType cmpnle(VTArg a, VTArg b) { return cmpgt(a, b); }
#undef SUFFIX
            static Vc_ALWAYS_INLINE Vc_CONST VectorType round(VTArg a) { return a; }
        };
#undef OP1
#undef OP
#undef OP_
#undef OPx
#undef OPcmp
#undef OP_CAST_
#undef MINMAX

template<> struct VectorHelper<char>
{
    typedef VectorTypeHelper<char>::Type VectorType;
#ifdef VC_PASSING_VECTOR_BY_VALUE_IS_BROKEN
    typedef const VectorType & VTArg;
#else
    typedef const VectorType VTArg;
#endif
    typedef char EntryType;
};

template<> struct VectorHelper<signed char>
{
    typedef VectorTypeHelper<signed char>::Type VectorType;
#ifdef VC_PASSING_VECTOR_BY_VALUE_IS_BROKEN
    typedef const VectorType & VTArg;
#else
    typedef const VectorType VTArg;
#endif
    typedef signed char EntryType;
};

template<> struct VectorHelper<unsigned char>
{
    typedef VectorTypeHelper<unsigned char>::Type VectorType;
#ifdef VC_PASSING_VECTOR_BY_VALUE_IS_BROKEN
    typedef const VectorType & VTArg;
#else
    typedef const VectorType VTArg;
#endif
    typedef unsigned char EntryType;
};
#endif  // AVX2

}  // namespace AVX(2)
}  // namespace Vc

#include "undomacros.h"

#endif // AVX_VECTORHELPER_H
