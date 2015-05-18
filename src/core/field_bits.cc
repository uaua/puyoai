#include "core/field_bits.h"

using namespace std;

const __m128i FieldBits::FIELD_MASK = _mm_set_epi16(0, 0x1FFE, 0x1FFE, 0x1FFE, 0x1FFE, 0x1FFE, 0x1FFE, 0);

const __m128i FieldBits::s_table_[128] = {
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00000001),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00000002),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00000004),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00000008),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00000010),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00000020),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00000040),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00000080),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00000100),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00000200),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00000400),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00000800),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00001000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00002000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00004000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00008000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00010000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00020000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00040000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00080000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00100000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00200000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00400000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00800000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x01000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x02000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x04000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x08000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x10000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x20000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x40000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x80000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000001, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000002, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000004, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000008, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000010, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000020, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000040, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000080, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000100, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000200, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000400, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00000800, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00001000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00002000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00004000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00008000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00010000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00020000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00040000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00080000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00100000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00200000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00400000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x00800000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x01000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x02000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x04000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x08000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x10000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x20000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x40000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000000, 0x80000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000001, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000002, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000004, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000008, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000010, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000020, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000040, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000080, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000100, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000200, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000400, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00000800, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00001000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00002000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00004000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00008000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00010000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00020000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00040000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00080000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00100000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00200000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00400000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x00800000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x01000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x02000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x04000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x08000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x10000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x20000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x40000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000000, 0x80000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000001, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000002, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000004, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000008, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000010, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000020, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000040, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000080, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000100, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000200, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000400, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00000800, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00001000, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00002000, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00004000, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00008000, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00010000, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00020000, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00040000, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00080000, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00100000, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00200000, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00400000, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x00800000, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x01000000, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x02000000, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x04000000, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x08000000, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x10000000, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x20000000, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x40000000, 0x00000000, 0x00000000, 0x00000000),
    _mm_set_epi32(0x80000000, 0x00000000, 0x00000000, 0x00000000)
};

FieldBits FieldBits::vanishingSeed() const
{
    //  x
    // xox              -- o is 3-connected
    //
    // xoox  ox   x oo
    //      xo  xoo oo  -- o is 2-connected.
    //
    // So, one 3-connected piece or two 2-connected pieces are necessary and sufficient.

    __m128i u = _mm_and_si128(_mm_slli_epi16(m_, 1), m_);
    __m128i d = _mm_and_si128(_mm_srli_epi16(m_, 1), m_);
    __m128i l = _mm_and_si128(_mm_slli_si128(m_, 2), m_);
    __m128i r = _mm_and_si128(_mm_srli_si128(m_, 2), m_);

    __m128i ud = _mm_and_si128(u, d);
    __m128i ul = _mm_and_si128(u, l);
    __m128i ur = _mm_and_si128(u, r);
    __m128i dl = _mm_and_si128(d, l);
    __m128i dr = _mm_and_si128(d, r);
    __m128i lr = _mm_and_si128(l, r);

    __m128i dlr = _mm_and_si128(lr, dl);
    __m128i ulr = _mm_and_si128(lr, ur);
    __m128i udr = _mm_and_si128(ur, ud);
    __m128i udl = _mm_and_si128(ul, ud);

    __m128i threes = _mm_or_si128(_mm_or_si128(dlr, ulr), _mm_or_si128(udr, udl));

    __m128i twos = _mm_or_si128(_mm_or_si128(ud, ul),
                                _mm_or_si128(_mm_or_si128(ur, dl),
                                             _mm_or_si128(dr, lr)));

    __m128i two_u = _mm_and_si128(_mm_slli_epi16(twos, 1), twos);
    __m128i two_d = _mm_and_si128(_mm_srli_epi16(twos, 1), twos);
    __m128i two_l = _mm_and_si128(_mm_slli_si128(twos, 2), twos);
    __m128i two_r = _mm_and_si128(_mm_srli_si128(twos, 2), twos);
    __m128i two_twos = _mm_or_si128(_mm_or_si128(two_u, two_d), _mm_or_si128(two_l, two_r));

    return FieldBits(_mm_or_si128(threes, two_twos));
}
