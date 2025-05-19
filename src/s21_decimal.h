#ifndef S21_DECIMAL_H_
#define S21_DECIMAL_H_

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DEC powf(2.0f, 96.0f) - 1.0
#define MIN_DEC -powf(2.0f, 96.0f) + 1.0

#define FALSE 0
#define TRUE 1

#define POSITIVE 0
#define NEGATIVE 1

#define OK 0
#define TOO_BIG 1
#define TOO_SMALL 2
#define ZERO_DIVISION 3

#define ERROR_DEC 0b01111111000000001111111111111111
#define NULL_DEC 0x00000000

#define SIGN_MASK 0x80000000

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int big_bits[7];
} s21_big_decimal;

enum bits { low, middle, high, scale };

typedef union {
  int ui;
  float fl;
} floatbits;


int s21_get_bit(s21_decimal number, int index);
void s21_set_bit(s21_decimal *source, int index, int bit);
int s21_get_bit_val(unsigned int number, int index);
void s21_set_bit_val(unsigned int *number, int bit, int index);
void s21_shift_left(s21_decimal *number);
void s21_shift_right(s21_decimal *number);
int s21_get_sign(s21_decimal number);
void s21_set_sign(s21_decimal *number, int sign);
int s21_get_scale(s21_decimal number);
void s21_set_scale(s21_decimal *number, int pow);
void s21_clear(s21_decimal *number);
int s21_decimal_check(s21_decimal temp);
void s21_decimal_init(s21_decimal *target);
void s21_set_degree(s21_decimal *val, int degree);
int s21_get_degree(s21_decimal val);
void s21_floatround(double *temp_number);
void s21_scale_tuning(double *temp_number, int *scale, int *scale_diff,
                      floatbits mantissa);
int s21_exp(floatbits mantissa);


int s21_get_bit_big(s21_big_decimal number, int index);
void s21_set_bit_big(s21_big_decimal *source, int index, int bit);
void s21_shift_left_big(s21_big_decimal *number);
void s21_shift_right_big(s21_big_decimal *number);
void s21_clear_big(s21_big_decimal *number);
s21_big_decimal s21_to_big(s21_decimal *number);


int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);


int s21_simple_add(s21_big_decimal value_1, s21_big_decimal value_2,
                   s21_big_decimal *result);
int s21_simple_mul(s21_big_decimal value_1, s21_big_decimal value_2,
                   s21_big_decimal *result);
int s21_simple_sub(s21_big_decimal value_1, s21_big_decimal value_2,
                   s21_big_decimal *result);
void s21_normalize_to_upper(s21_decimal value, int exp, s21_big_decimal *val);
void s21_normalize(s21_decimal value_1, s21_decimal value_2,
                   s21_big_decimal *val1, s21_big_decimal *val2);
int s21_div_with_remainder(s21_big_decimal value_1, s21_big_decimal value_2,
                           s21_big_decimal *result, s21_big_decimal *remainder);
int s21_big_to_decimal(s21_big_decimal *value, s21_decimal *res, int pow,
                       int sign);
int s21_max(int val1, int val2);


int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);
int s21_is_zero(s21_decimal, s21_decimal);
int s21_mantissa_equal_compare(s21_big_decimal, s21_big_decimal);
int s21_less_mantissa_compare(s21_decimal, s21_decimal, int);
int s21_less_big_mantissa_compare(s21_big_decimal, s21_big_decimal, int);


int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);  
int s21_from_decimal_to_float(s21_decimal src, float *dst);


int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

void s21_div_to_10(s21_decimal *in);

void s21_normalize_to_upper(s21_decimal value, int exp, s21_big_decimal *val);
void s21_normalize(s21_decimal value_1, s21_decimal value_2,
                   s21_big_decimal *val1, s21_big_decimal *val2);

#endif
