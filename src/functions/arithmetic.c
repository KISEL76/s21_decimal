#include "../s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int flag = 0;
  if (s21_decimal_check(value_1) || s21_decimal_check(value_2) ||
      result == NULL) {
    flag = 1;
  } else {
    int pow1 = s21_get_scale(value_1);
    int pow2 = s21_get_scale(value_2);

    int pow = s21_max(pow1, pow2);

    int sign1 = s21_get_sign(value_1);
    int sign2 = s21_get_sign(value_2);

    s21_big_decimal val_1 = {{0, 0, 0, 0, 0, 0, 0}};
    s21_big_decimal val_2 = {{0, 0, 0, 0, 0, 0, 0}};
    s21_big_decimal res = {{0, 0, 0, 0, 0, 0, 0}};
    s21_normalize(value_1, value_2, &val_1, &val_2);

    if (sign1 == sign2) {
      s21_simple_add(val_1, val_2, &res);

      flag = s21_big_to_decimal(&res, result, pow, sign1);
    } else {
      if (s21_less_big_mantissa_compare(val_2, val_1, 0)) {
        s21_simple_sub(val_1, val_2, &res);
        flag = s21_big_to_decimal(&res, result, pow, sign1);
      } else {
        s21_simple_sub(val_2, val_1, &res);
        flag = s21_big_to_decimal(&res, result, pow, sign2);
      }
    }
  }
  return flag;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int flag = 0;
  if (s21_decimal_check(value_1) || s21_decimal_check(value_2) ||
      result == NULL) {
    flag = 1;
  } else {
    int sign2 = 0;
    if (s21_get_sign(value_2) == 0) sign2 = 1;

    s21_decimal opposite_value_2 = value_2;
    s21_set_bit(&opposite_value_2, 127, sign2);
    flag = s21_add(value_1, opposite_value_2, result);
  }
  return flag;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int flag = 0;
  if (s21_decimal_check(value_1) || s21_decimal_check(value_2) ||
      result == NULL) {
    flag = 1;
  } else {
    int pow1 = s21_get_scale(value_1);
    int pow2 = s21_get_scale(value_2);

    int pow = pow1 + pow2;

    int sign1 = s21_get_sign(value_1);
    int sign2 = s21_get_sign(value_2);

    s21_big_decimal val_1 = s21_to_big(&value_1);
    s21_big_decimal val_2 = s21_to_big(&value_2);
    s21_big_decimal res = {{0, 0, 0, 0, 0, 0, 0}};

    if (sign1 == sign2) {
      s21_simple_mul(val_1, val_2, &res);
      flag = s21_big_to_decimal(&res, result, pow, 0);
    } else {
      s21_simple_mul(val_1, val_2, &res);
      flag = s21_big_to_decimal(&res, result, pow, 1);
    }
  }
  return flag;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int flag = 0;
  if (s21_decimal_check(value_1) || s21_decimal_check(value_2) ||
      result == NULL) {
    flag = 1;
  } else {
    int pow = 0;
    int sign1 = s21_get_sign(value_1);
    int sign2 = s21_get_sign(value_2);
    int sign = 0;
    if (sign1 != sign2) sign = 1;

    s21_big_decimal zero = {{0, 0, 0, 0, 0, 0, 0}};
    s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0}};
    s21_big_decimal val_1 = {{0, 0, 0, 0, 0, 0, 0}};
    s21_big_decimal val_2 = {{0, 0, 0, 0, 0, 0, 0}};
    s21_big_decimal res = {{0, 0, 0, 0, 0, 0, 0}};
    s21_big_decimal tmp_res = {{0, 0, 0, 0, 0, 0, 0}};
    s21_big_decimal tmp_res1 = {{0, 0, 0, 0, 0, 0, 0}};
    s21_big_decimal remainder = {{0, 0, 0, 0, 0, 0, 0}};
    s21_big_decimal tmp_remainder = {{0, 0, 0, 0, 0, 0, 0}};
    s21_big_decimal tmp_remainder1 = {{0, 0, 0, 0, 0, 0, 0}};

    s21_normalize(value_1, value_2, &val_1, &val_2);

    if (value_2.bits[0] == 0 && value_2.bits[1] == 0 && value_2.bits[2] == 0)
      flag = ZERO_DIVISION;
    else
      s21_div_with_remainder(val_1, val_2, &res, &remainder);

    while (!s21_mantissa_equal_compare(remainder, zero) && pow < 29) {
      pow++;
      s21_simple_mul(res, ten, &tmp_res1);
      s21_clear_big(&res);
      res = tmp_res1;
      s21_clear_big(&tmp_res1);

      s21_simple_mul(remainder, ten, &tmp_remainder1);
      s21_clear_big(&remainder);
      remainder = tmp_remainder1;
      s21_clear_big(&tmp_remainder1);

      s21_div_with_remainder(remainder, val_2, &tmp_res, &tmp_remainder);

      s21_simple_add(res, tmp_res, &tmp_res1);
      s21_clear_big(&res);
      res = tmp_res1;
      s21_clear_big(&tmp_res1);

      s21_clear_big(&remainder);
      remainder = tmp_remainder;
      s21_clear_big(&tmp_remainder);
      s21_clear_big(&tmp_res);
    }
    if (flag != ZERO_DIVISION)
      flag = s21_big_to_decimal(&res, result, pow, sign);
  }
  return flag;
}
