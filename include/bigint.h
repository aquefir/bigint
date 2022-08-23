/*****************************************************************************\
 *                            Big Integer Library                            *
 *                                                                           *
 *                        Copyright (C) 2022 Aquefir.                        *
 *                  Released under Artisan Software Licence                  *
\*****************************************************************************/

#ifndef INC__ORAS_BIGINT_H
#define INC__ORAS_BIGINT_H

#include <uni/types/int.h>

/* return value enum for comparison operations. */
enum bigint_cmp_retval
{
	BIGINT_FALSE,
	BIGINT_TRUE,
	BIGINT_UNDEFINED
};

struct bigint_s
{
	/* data is stored in little endian, so the least significant octet comes
	 * first in memory. */
	u8 * data;
	/* the size of the data, denominated in octets.
	 * all unused octets can be left zero, so no need to distinguish
	 * it from capacity. */
	u16 sz;
};

/* return value struct to communicate potential overflows. */
struct bigint_s_retval
{
	struct bigint_s val;
	u32 overflow : 1;
};

/* return value struct for division and root operations. */
struct bigint_s_divrt_retval
{
	struct bigint_s quot;
	struct bigint_s rem;
	/* will be set if divide by zero error or if taking root of a negative
	 * number. in the dividing case, quot and rem values are meaningless.
	 * rem is unused in the root case. */
	u32 err : 1;
};

/* unsigned bigint struct. */
struct bigint_u
{
	/* data is stored in little endian, so most-significant octet is first. */
	u8 * data;
	u16 cap;
	u16 sz;
};

/* return value struct to communicate potential overflows. */
struct bigint_u_retval
{
	struct bigint_u val;
	u32 overflow : 1;
};

/* return value struct for division and root operations. */
struct bigint_u_div_retval
{
	struct bigint_u quot;
	struct bigint_u rem;
	/* will be set if divide by zero occurred. also, draggin ball zee */
	u32 dbz : 1;
};

/* initialise a signed bigint, specifying how many octets are desired for the
 * initial capacity. */
struct bigint_s bigint_s_init( u16 );

/* make a deep copy of a signed bigint, with the copy allocating just enough
 * memory to hold the significant bits. */
struct bigint_s bigint_s_dup( struct bigint_s );

/* make a signed bigint from a 64-bit primitive. */
struct bigint_s bigint_s_make64( s64 );

/* make a signed bigint from a 32-bit primitive. */
struct bigint_s bigint_s_make32( s32 );

/* make a signed bigint from a 16-bit primitive. */
struct bigint_s bigint_s_make16( s16 );

/* make a signed bigint from a 8-bit primitive. */
struct bigint_s bigint_s_make8( s8 );

/* finalise a signed bigint, freeing the memory it was using. */
void bigint_s_fini( struct bigint_s );

/* zero out all of the bits of a signed bigint. this does not change its size
 * or its capacity. this function does not allocate, and its return value has
 * the same memory as that given in the first parameter. */
struct bigint_s bigint_s_zero( struct bigint_s );

/* compare two signed bigints, returning truthy if LHS > RHS. */
enum bigint_cmp_retval bigint_s_cmp_gt( struct bigint_s, struct bigint_s );

/* compare two signed bigints, returning truthy if LHS >= RHS. */
enum bigint_cmp_retval bigint_s_cmp_ge( struct bigint_s, struct bigint_s );

/* compare two signed bigints, returning truthy if they are equal. */
enum bigint_cmp_retval bigint_s_cmp_eq( struct bigint_s, struct bigint_s );

/* add RHS to LHS, returning the result. this function does not allocate, and
 * its return value has the same memory as that given in the first parameter.
 */
struct bigint_s_retval bigint_s_add( struct bigint_s, struct bigint_s );

/* subtract RHS from LHS, returning the result. this function does not
 * allocate, and its return value has the same memory as that given in the
 * first parameter. */
struct bigint_s_retval bigint_s_sub( struct bigint_s, struct bigint_s );

/* multiply LHS by RHS, returning the result. this function does not allocate,
 * and its return value has the same memory as that given in the first
 * parameter. */
struct bigint_s_retval bigint_s_mul( struct bigint_s, struct bigint_s );

/* divide LHS by RHS, returning the result. this function does not allocate,
 * and its return value has the same memory as that given in the first
 * parameter. */
struct bigint_s_divrt_retval bigint_s_div( struct bigint_s, struct bigint_s );

/* raise LHS to the power of RHS, returning the result. this function does not
 * allocate, and its return value has the same memory as that given in the
 * first parameter. */
struct bigint_s_retval bigint_s_pow( struct bigint_s, struct bigint_s );

/* take the square root of a signed bigint, returning the result. this
 * function does not allocate, and its return value has the same memory as
 * that given in the first parameter. */
struct bigint_s_divrt_retval bigint_s_sqrt( struct bigint_s );

/* take the cube root of a signed bigint, returning the result. this function
 * does not allocate, and its return value has the same memory as that given
 * in the first parameter. */
struct bigint_s_divrt_retval bigint_s_cbrt( struct bigint_s );

/* bitwise OR two signed bigints, returning the result. this function may not
 * preserve the sign bit. this function does not allocate, and its return
 * value has the same memory as that given in the first parameter. */
struct bigint_s bigint_s_orr( struct bigint_s, struct bigint_s );

/* bitwise AND two signed bigints, returning the result. this function may not
 * preserve the sign bit. this function does not allocate, and its return
 * value has the same memory as that given in the first parameter. */
struct bigint_s bigint_s_and( struct bigint_s, struct bigint_s );

/* bitwise negate a signed bigint, returning the result. this function does
 * not preserve the sign bit. this function does not allocate, and its return
 * value has the same memory as that given in the first parameter. */
struct bigint_s bigint_s_not( struct bigint_s );

/* bitwise exclusive or (XOR) two signed bigints, returning the result. this
 * function may not preserve the sign bit. this function does not allocate,
 * and its return value has the same memory as that given in the first
 * parameter. */
struct bigint_s bigint_s_xor( struct bigint_s, struct bigint_s );

/* logically shift left the LHS by RHS number of bits, returning the result.
 * this function does not allocate, and its return value has the same memory
 * as that given in the first parameter. */
struct bigint_s bigint_s_lsl( struct bigint_s, u32 );

/* logically shift right the LHS by RHS number of bits, returning the result.
 * this function does not respect the sign bit. this function does not
 * allocate, and its return value has the same memory as that given in the
 * first parameter. */
struct bigint_s bigint_s_lsr( struct bigint_s, u32 );

/* arithmetically shift right the LHS by RHS number of bits, returning the
 * result. this function does respect the sign bit. this function does not
 * allocate, and its return value has the same memory as that given in the
 * first parameter. */
struct bigint_s bigint_s_asr( struct bigint_s, u32 );

/* rotate left the LHS by RHS number of bits, returning the result. this
 * function does not respect the sign bit. this function does not allocate,
 * and its return value has the same memory as that given in the first
 * parameter. */
struct bigint_s bigint_s_rol( struct bigint_s, u32 );

/* rotate right the LHS by RHS number of bits, returning the result. this
 * function does not respect the sign bit. this function does not allocate,
 * and its return value has the same memory as that given in the first
 * parameter. */
struct bigint_s bigint_s_ror( struct bigint_s, u32 );

/* count the leading zeroes in a signed bigint, returning the result. make
 * sure to contextualise this by checking the bigint's size. */
u32 bigint_s_clz( struct bigint_s );

/* count the trailing zeroes in a signed bigint, returning the result. make
 * sure to contextualise this by checking the bigint's size. */
u32 bigint_s_ctz( struct bigint_s );

/* count the number of high bits in a signed bigint, returning the result.
 * to get this function's inverse, subtract the result from its size. */
u32 bigint_s_popcount( struct bigint_s );

/* initialise an unsigned bigint, specifying how many octets are desired for
 * the initial capacity. */
struct bigint_u bigint_u_init( u16 );

/* make a deep copy of an unsigned bigint, with the copy allocating just enough
 * memory to hold the significant bits. */
struct bigint_u bigint_u_dup( struct bigint_u );

/* make an unsigned bigint from a 64-bit primitive. */
struct bigint_u bigint_u_make64( u64 );

/* make an unsigned bigint from a 32-bit primitive. */
struct bigint_u bigint_u_make32( u32 );

/* make an unsigned bigint from a 16-bit primitive. */
struct bigint_u bigint_u_make16( u16 );

/* make an unsigned bigint from a 8-bit primitive. */
struct bigint_u bigint_u_make8( u8 );

/* finalise an unsigned bigint, freeing the memory it was using. */
void bigint_u_fini( struct bigint_u );

/* zero out all of the bits of an unsigned bigint. this does not change its
 * size or its capacity. this function does not allocate, and its return value
 * has the same memory as that given in the first parameter. */
struct bigint_u bigint_u_zero( struct bigint_u );

/* compare two unsigned bigints, returning truthy if LHS > RHS. */
enum bigint_cmp_retval bigint_u_cmp_gt( struct bigint_u, struct bigint_u );

/* compare two unsigned bigints, returning truthy if LHS >= RHS. */
enum bigint_cmp_retval bigint_u_cmp_ge( struct bigint_u, struct bigint_u );

/* compare two unsigned bigints, returning truthy if they are equal. */
enum bigint_cmp_retval bigint_u_cmp_eq( struct bigint_u, struct bigint_u );

/* add two unsigned bigints together, returning the result. this function does
 * not allocate, and its return value has the same memory as that given in the
 * first parameter. */
struct bigint_u_retval bigint_u_add( struct bigint_u, struct bigint_u );

/* subtract RHS from LHS, returning the result. this function does not
 * allocate, and its return value has the same memory as that given in the
 * first parameter. */
struct bigint_u_retval bigint_u_sub( struct bigint_u, struct bigint_u );

/* multiply LHS by RHS, returning the result. this function does not allocate,
 * and its return value has the same memory as that given in the first
 * parameter. */
struct bigint_u_retval bigint_u_mul( struct bigint_u, struct bigint_u );

/* divide LHS by RHS, returning the result. this function does not allocate,
 * and its return value has the same memory as that given in the first
 * parameter. */
struct bigint_u_div_retval bigint_u_div( struct bigint_u, struct bigint_u );

/* raise LHS to the power of RHS, returning the result. this function does not
 * allocate, and its return value has the same memory as that given in the
 * first parameter. */
struct bigint_u_retval bigint_u_pow( struct bigint_u, struct bigint_u );

/* take the square root of an unsigned bigint, returning the result. this
 * function does not allocate, and its return value has the same memory as
 * that given in the first parameter. */
struct bigint_u_retval bigint_u_sqrt( struct bigint_u );

/* take the cube root of an unsigned bigint, returning the result. this
 * function does not allocate, and its return value has the same memory as
 * that given in the first parameter. */
struct bigint_u_retval bigint_u_cbrt( struct bigint_u );

/* bitwise OR two unsigned bigints, returning the result. this function does
 * not allocate, and its return value has the same memory as that given in the
 * first parameter. */
struct bigint_u bigint_u_orr( struct bigint_u, struct bigint_u );

/* bitwise AND two unsigned bigints, returning the result. this function does
 * not allocate, and its return value has the same memory as that given in the
 * first parameter. */
struct bigint_u bigint_u_and( struct bigint_u, struct bigint_u );

/* bitwise negatve an unsigned bigint, returning the result. this function
 * does not allocate, and its return value has the same memory as that given
 * in the first parameter. */
struct bigint_u bigint_u_not( struct bigint_u );

/* bitwise exclusive or (XOR) two unsigned bigints, returning the result. this
 * function does not allocate, and its return value has the same memory as
 * that given in the first parameter. */
struct bigint_u bigint_u_xor( struct bigint_u, struct bigint_u );

/* logically shift left the LHS by RHS number of bits, returning the result.
 * this function does not allocate, and its return value has the same memory
 * as that given in the first parameter. */
struct bigint_u bigint_u_lsl( struct bigint_u, u32 );

/* logically shift right the LHS by RHS number of bits, returning the result.
 * this function does not allocate, and its return value has the same memory
 * as that given in the first parameter. */
struct bigint_u bigint_u_lsr( struct bigint_u, u32 );

/* rotate left the LHS by RHS number of bits, returning the result. this
 * function does not allocate, and its return value has the same memory as
 * that given in the first parameter. */
struct bigint_u bigint_u_rol( struct bigint_u, u32 );

/* rotate right the LHS by RHS number of bits, returning the result. this
 * function does not allocate, and its return value has the same memory as
 * that given in the first parameter. */
struct bigint_u bigint_u_ror( struct bigint_u, u32 );

/* count the leading zeroes in an unsigned bigint, returning the result. make
 * sure to contextualise this by checking the bigint's size. */
u32 bigint_u_clz( struct bigint_u );

/* count the trailing zeroes in an unsigned bigint, returning the result. make
 * sure to contextualise this by checking the bigint's size. */
u32 bigint_u_ctz( struct bigint_u );

/* count the number of high bits in a signed bigint, returning the result.
 * to get this function's inverse, subtract the result from its size. */
u32 bigint_u_popcount( struct bigint_u );

#endif /* INC__ORAS_BIGINT_H */
