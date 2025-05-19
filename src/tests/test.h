// +-------------------------------------------------+
// |        Code Authored by Vladimir Kiselev        |
// |                Year: 2024                       |
// |      > Syntax: Approved | Logic: Verified       |
// +-------------------------------------------------+

#ifndef TEST_H_
#define TEST_H_

#include <check.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#include "../s21_decimal.h"

#define TRUE 1
#define FALSE 0

#define TEST_OTHER_OK 0
#define TEST_OTHER_ERROR 1

#define TEST_CONVERSION_OK 0
#define TEST_CONVERSION_ERROR 1

#define TEST_ARITHMETIC_OK 0
#define TEST_ARITHMETIC_BIG 1
#define TEST_ARITHMETIC_SMALL 2
#define TEST_ARITHMETIC_ZERO_DIV 3

typedef struct {
  int fails;
} fail;

void run_tests(fail *fails);
void run_testcase(Suite *testcase, fail *fails);

Suite *int_to_dec(void);
Suite *dec_to_int(void);
Suite *float_to_dec(void);
Suite *dec_to_float(void);

Suite *suite_s21_sub_mini(void);
Suite *suite_s21_add_mini(void);
Suite *suite_s21_mul_mini(void);
Suite *suite_s21_div_mini(void);

Suite *s21_test_is_equal(void);
Suite *s21_test_is_less(void);
Suite *s21_test_is_less_or_equal(void);
Suite *s21_test_is_greater(void);
Suite *s21_test_is_greater_or_equal(void);
Suite *s21_test_is_not_equal(void);

Suite *suite_truncate_mini(void);
Suite *suite_round_mini(void);
Suite *suite_negate_mini(void);
Suite *suite_floor_mini(void);

#endif