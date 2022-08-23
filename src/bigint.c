/*****************************************************************************\
 *                            Big Integer Library                            *
 *                                                                           *
 *                        Copyright (C) 2022 Aquefir.                        *
 *                  Released under Artisan Software Licence                  *
\*****************************************************************************/

#include "bigint.h"

#include <uni/memory.h>

struct bigint_s bigint_s_init( u16 sz )
{
	struct bigint_s ret;

	uni_memset( &ret, 0, sizeof ret );

	if(sz > 0)
	{
		ret.sz = sz;
		ret.data = uni_alloc0( sz );
	}

	return ret;
}

struct bigint_s bigint_s_dup( struct bigint_s in )
{
	struct bigint_s ret;
	u16 i, new_sz;

	uni_memcpy( &ret, &in, sizeof in );

	for(i = ret.sz - 1, new_sz = ret.sz; i >= 0; --i)
	{
		new_sz -= ret.data[i] == 0 ? 1 : 0;
	}

	if(new_sz != ret.sz)
	{
		u8 * const new_data = uni_alloc( new_sz );

		uni_free( ret.data );

		ret.data = new_data;
		ret.sz = new_sz;
	}

	return ret;
}

struct bigint_s bigint_s_make64( s64 n )
{
	struct bigint_s ret = bigint_s_init( sizeof n );

	uni_memcpy( ret.data, &n, sizeof n );

	ret.sz = sizeof n;

	return ret;
}

struct bigint_s bigint_s_make32( s32 n )
{
	struct bigint_s ret = bigint_s_init( sizeof n );

	uni_memcpy( ret.data, &n, sizeof n );

	ret.sz = sizeof n;

	return ret;
}

struct bigint_s bigint_s_make16( s16 n )
{
	struct bigint_s ret = bigint_s_init( sizeof n );

	uni_memcpy( ret.data, &n, sizeof n );

	ret.sz = sizeof n;

	return ret;
}

struct bigint_s bigint_s_make8( s8 n )
{
	struct bigint_s ret = bigint_s_init( sizeof n );

	uni_memcpy( ret.data, &n, sizeof n );

	ret.sz = sizeof n;

	return ret;
}

void bigint_s_fini( struct bigint_s in )
{
	if(in.data != NULL)
	{
		uni_free( in.data );
	}
}

struct bigint_s bigint_s_zero( struct bigint_s in )
{
	if(in.sz > 0)
	{
		uni_memset( in.data, 0, in.sz );
	}

	return in;
}

static int isnegative( struct bigint_s * in )
{
	return in->data[in->sz - 1] >> 7;
}

static enum bigint_cmp_retval bigint_s_cmp_impl( struct bigint_s * a, struct bigint_s * b, int orequ )
{
	if(a->sz == 0 || b->sz == 0)
	{
		return BIGINT_UNDEFINED;
	}

	/* first we must do sign comparison,
	 * if they are different, our job is easy
	 * otherwise, we must count the ways */
	{
		const int a_neg = isnegative( a );
		const int b_neg = isnegative( b );

		if(a_neg != b_neg)
		{
			return b_neg ? BIGINT_TRUE : BIGINT_FALSE;
		}

		/* from here, the signs are the same, so we only use one;
		 * negative sign simply inverts our reporting, nothing more;
		 * let us clear the signs themselves so they do not interfere;
		 * we noted them above for our own use so it's OK */
		a->data[a->sz - 1] = a->data[a->sz - 1] & 0x7F;
		b->data[b->sz - 1] = b->data[b->sz - 1] & 0x7F;

		/* the numbers having a different size complicates things somewhat;
		 * we need to zip down the larger of the two, and if any octets are
		 * nonzero we can return */
		if(a->sz != b->sz)
		{
			const int a_big = a->sz > b->sz;
			struct bigint_s * const big = a_big ? a : b;
			const u16 sml_sz = a->sz < b->sz ? a->sz : b->sz;
			u16 i;

			for(i = big->sz - 1; i >= sml_sz; --i)
			{
				if(big->data[i] != 0)
				{
					return a_big ?
						a_neg ? BIGINT_FALSE : BIGINT_TRUE
						: a_neg ? BIGINT_TRUE : BIGINT_FALSE;
				}
			}
		}

		/* the algorithm is simple and linear:
		 * we simply count down from the MSB to the LSB, octet-by-octet,
		 * and if one is greater than the other, we know who's bigger */
		{
			u16 i;
			const u16 sml_sz = a->sz < b->sz ? a->sz : b->sz;

			for(i = sml_sz; i >= 0; --i)
			{
				if(a->data[i] > b->data[i])
				{
					return a_neg ? BIGINT_FALSE : BIGINT_TRUE;
				}
				else if(a->data[i] < b->data[i])
				{
					return a_neg ? BIGINT_TRUE : BIGINT_FALSE;
				}
			}
		}
	}

	return orequ ? BIGINT_TRUE : BIGINT_FALSE;
}

enum bigint_cmp_retval bigint_s_cmp_gt( struct bigint_s a, struct bigint_s b )
{
	return bigint_s_cmp_impl( &a, &b, 0 );
}

enum bigint_cmp_retval bigint_s_cmp_ge( struct bigint_s a, struct bigint_s b )
{
	return bigint_s_cmp_impl( &a, &b, 1 );
}

enum bigint_cmp_retval bigint_s_cmp_eq( struct bigint_s a, struct bigint_s b )
{
	if(a.sz == 0 || b.sz == 0)
	{
		return BIGINT_UNDEFINED;
	}

	/* no special logic this time around. simple comparison of octets */
	if(a.sz != b.sz)
	{
		return BIGINT_FALSE;
	}
	else
	{
		u16 i;
		const u16 sz = a.sz;

		/* we compare from LSB to MSB because in many situations they are
		 * more likely to be different (conjecture) */
		for(i = 0; i < sz; ++i)
		{
			if(a.data[i] != b.data[i])
			{
				return BIGINT_FALSE;
			}
		}
	}

	return BIGINT_TRUE;
}
