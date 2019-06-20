#ifndef TARANTOOL_LIB_CORE_DECIMAL_H_INCLUDED
#define TARANTOOL_LIB_CORE_DECIMAL_H_INCLUDED
/*
 * Copyright 2019, Tarantool AUTHORS, please see AUTHORS file.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * 1. Redistributions of source code must retain the above
 *    copyright notice, this list of conditions and the
 *    following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * <COPYRIGHT HOLDER> OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/** Maximum decimal digigts taken by a decimal representation. */
#define DECIMAL_MAX_DIGITS 38
#define DECNUMDIGITS DECIMAL_MAX_DIGITS
#include "third_party/decNumber/decNumber.h"
#include <stdint.h>

typedef decNumber decimal_t;

/**
 * @return decimal precision,
 * i.e. the amount of decimal digits in
 * its representation.
 */
int
decimal_precision(const decimal_t *dec);

/**
 * @return decimal scale,
 * i.e. the number of decimal digits after
 * the decimal separator.
 */
int
decimal_scale(const decimal_t *dec);

/**
 * Initialize a zero decimal number.
 */
decimal_t *
decimal_zero(decimal_t *dec);

/**
 * Initialize a decimal with a value from the string.
 *
 * If the number is less, than 10^DECIMAL_MAX_DIGITS,
 * but has excess digits in fractional part, it will be rounded.
 *
 * @return NULL if string is invalid or
 * the number is too big (>= 10^DECIMAL_MAX_DIGITS)
 */
decimal_t *
decimal_from_string(decimal_t *dec, const char *str);

/**
 * Initialize a decimal from double.
 *
 * @return NULL is double is NaN or Infinity,
 * or is greater than 10^DECIMAL_MAX_DIGITS.
 *         \a dec otherwise.
 */
decimal_t *
decimal_from_double(decimal_t *dec, double d);

/**
 * Initialize a decimal with an integer value.
 */
decimal_t *
decimal_from_int64(decimal_t *dec, int64_t num);

/** @copydoc decimal_from_int */
decimal_t *
decimal_from_uint64(decimal_t *dec, uint64_t num);

/**
 * Write the decimal to a string.
 * Returns a statically allocated buffer containing
 * the decimal representation.
 */
const char *
decimal_to_string(const decimal_t *dec);

/**
 * Compare 2 decimal values.
 * @return -1, lhs < rhs,
 *	    0, lhs = rhs,
 *	    1, lhs > rhs
 */
int
decimal_compare(const decimal_t *lhs, const decimal_t *rhs);

/**
 * Round a given decimal to have not more than
 * scale digits after the decimal point.
 * If scale if greater than current dec scale, do nothing.
 * Scale must be in range [0, DECIMAL_MAX_DIGITS]
 *
 * @return NULL, scale is out of bounds.
 *
 */
decimal_t *
decimal_round(decimal_t *dec, int scale);

/**
 * res is set to the absolute value of dec
 * decimal_abs(&a, &a) is allowed.
 */
decimal_t *
decimal_abs(decimal_t *res, const decimal_t *dec);

/** res is set to -dec. */
decimal_t *
decimal_minus(decimal_t *res, const decimal_t *dec);

/*
 * Arithmetic ops: add, subtract, multiply and divide.
 * Return result pointer on success, NULL on an error (overflow).
 */

decimal_t *
decimal_add(decimal_t *res, const decimal_t *lhs, const decimal_t *rhs);

decimal_t *
decimal_sub(decimal_t *res, const decimal_t *lhs, const decimal_t *rhs);

decimal_t *
decimal_mul(decimal_t *res, const decimal_t *lhs, const decimal_t *rhs);

decimal_t *
decimal_div(decimal_t *res, const decimal_t *lhs, const decimal_t *rhs);

/*
 * log10, ln, pow, exp, sqrt.
 * Calculate the appropriate function with maximum precision
 * (DECIMAL_MAX_DIGITS)
 * Return result pointer on success, NULL on an error (overflow).
 */
decimal_t *
decimal_log10(decimal_t *res, const decimal_t *lhs);

decimal_t *
decimal_ln(decimal_t *res, const decimal_t *lhs);

decimal_t *
decimal_pow(decimal_t *res, const decimal_t *lhs, const decimal_t *rhs);

decimal_t *
decimal_exp(decimal_t *res, const decimal_t *lhs);

decimal_t *
decimal_sqrt(decimal_t *res, const decimal_t *lhs);

/** @return The length in bytes decimal packed representation will take. */
uint32_t
decimal_len(const decimal_t *dec);

/**
 * Convert a decimal \a dec to its packed representation.
 *
 * @return data + decimal_len(dec);
 */
char *
decimal_pack(char *data, const decimal_t *dec);

/**
 * Using a packed representation of size \a len pointed to by
 * *data, unpack it to \a dec.
 *
 * \post *data = *data + decimal_len(dec);
 *
 * @return NULL if value encoding is incorrect
 *         dec otherwise.
 */
decimal_t *
decimal_unpack(const char **data, uint32_t len, decimal_t *dec);

#endif /* TARANTOOL_LIB_CORE_DECIMAL_H_INCLUDED */
