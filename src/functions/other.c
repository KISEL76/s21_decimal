#include <stdint.h>

#include "../s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int err = 0;
  if (result == NULL || s21_decimal_check(value)) {
    err = 1;
  } else {
    s21_clear(result);
    s21_decimal fract_five = {{0x5, 0x0, 0x0, 0x10000}};
    s21_decimal one = {{0x1, 0x0, 0x0, 0x00000}};
    s21_decimal tmp_value = {0};
    s21_decimal remainder = {0};

    s21_truncate(value, &tmp_value);
    s21_sub(value, tmp_value, &remainder);

    int sign = s21_get_sign(value);
    s21_set_sign(&tmp_value, 0);
    s21_set_sign(&remainder, 0);
    if (s21_is_equal(remainder, fract_five)) {
      if ((tmp_value.bits[0] & 1) == 0 ||
          (tmp_value.bits[0] == 0 && tmp_value.bits[1] == 0 &&
           tmp_value.bits[2] == 0 && tmp_value.bits[3] == 0)) {
        *result = tmp_value;
      } else {
        s21_add(tmp_value, one, result);
      }
    } else if (s21_is_greater(remainder, fract_five)) {
      s21_add(tmp_value, one, result);
    } else {
      *result = tmp_value;
    }
    s21_set_sign(result, sign);
  }
  return err;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  int err = 0;
  if (result == NULL || s21_decimal_check(value)) {
    err = 1;
  } else {
    err = s21_truncate(value, result);
    if (s21_get_sign(*result) && s21_is_not_equal(*result, value)) {
      result->bits[0] += 1;
    }
  }
  return err;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  int err = 0;
  if (result == NULL || s21_decimal_check(value)) {
    err = 1;
  } else {
    s21_clear(result);
    int sign = s21_get_sign(value);
    *result = value;
    if (sign == 1) {
      s21_set_sign(result, 0);
    } else {
      s21_set_sign(result, 1);
    }
  }
  return err;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int err = 0;
  if (result == NULL || s21_decimal_check(value)) {
    err = 1;
  } else {
    int degree = s21_get_degree(value);
    s21_set_degree(&value, 0);
    for (int k = 0; k < degree; k++) {
      s21_div_to_10(&value);
    }
    *result = value;
  }
  return err;
}
