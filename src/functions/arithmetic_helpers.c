#include <math.h>

#include "../s21_decimal.h"

int s21_simple_add(s21_big_decimal value_1, s21_big_decimal value_2,
                   s21_big_decimal *result) {
  int in_mind = 0;
  for (int i = 0; i < 224; i++) {
    int bit_val_1 = s21_get_bit_big(value_1, i);
    int bit_val_2 = s21_get_bit_big(value_2, i);
    int res = bit_val_1 + bit_val_2 + in_mind;
    if (res == 0)
      s21_set_bit_big(result, i, 0);
    else if (res == 1) {
      s21_set_bit_big(result, i, 1);
      in_mind = 0;
    } else if (res == 2) {
      s21_set_bit_big(result, i, 0);
      in_mind = 1;
    } else if (res == 3) {
      s21_set_bit_big(result, i, 1);
    }
  }
  return 0;
}

int s21_simple_mul(s21_big_decimal value_1, s21_big_decimal value_2,
                   s21_big_decimal *result) {
  for (int i = 0; i < 224; i++) {
    int bit = s21_get_bit_big(value_1, i);
    if (bit == 1) {
      s21_big_decimal tmp_val_2 = value_2;
      for (int p = 0; p < i; p++) s21_shift_left_big(&tmp_val_2);
      s21_simple_add(*result, tmp_val_2, result);
    }
  }
  return 0;
}

int s21_simple_sub(s21_big_decimal value_1, s21_big_decimal value_2,
                   s21_big_decimal *result) {
  s21_big_decimal copy_value_1 = value_1;
  for (int i = 0; i < 224; i++) {
    int bit1 = s21_get_bit_big(copy_value_1, i);
    int bit2 = s21_get_bit_big(value_2, i);
    if (bit1 == bit2)
      s21_set_bit_big(result, i, 0);
    else if (bit1 - bit2 == 1)
      s21_set_bit_big(result, i, 1);
    // случай, когда 0 - 1:
    else {
      int counter = i + 1;
      while (s21_get_bit_big(copy_value_1, counter) == 0) {
        s21_set_bit_big(&copy_value_1, counter, 1);
        counter += 1;
      }
      s21_set_bit_big(&copy_value_1, counter, 0);
      s21_set_bit_big(result, i, 1);
    }
  }
  return 0;
}

void s21_normalize(s21_decimal value_1, s21_decimal value_2,
                   s21_big_decimal *val1, s21_big_decimal *val2) {
  int pow1 = s21_get_scale(value_1);
  int pow2 = s21_get_scale(value_2);
  int diff = pow1 - pow2;
  if (diff > 0) {
    val1->big_bits[0] = value_1.bits[0];
    val1->big_bits[1] = value_1.bits[1];
    val1->big_bits[2] = value_1.bits[2];
    s21_normalize_to_upper(value_2, diff, val2);
  } else if (diff < 0) {
    val2->big_bits[0] = value_2.bits[0];
    val2->big_bits[1] = value_2.bits[1];
    val2->big_bits[2] = value_2.bits[2];
    s21_normalize_to_upper(value_1, -diff, val1);
  } else {
    val1->big_bits[0] = value_1.bits[0];
    val1->big_bits[1] = value_1.bits[1];
    val1->big_bits[2] = value_1.bits[2];
    val2->big_bits[0] = value_2.bits[0];
    val2->big_bits[1] = value_2.bits[1];
    val2->big_bits[2] = value_2.bits[2];
  }
}

void s21_normalize_to_upper(s21_decimal value, int exp, s21_big_decimal *val) {
  s21_big_decimal ten_decimal = {{10, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal ten_pow = {{1, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal tmp_res_ten = {{0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal big_value = {
      {value.bits[0], value.bits[1], value.bits[2], 0, 0, 0, 0}};

  for (int i = 0; i < exp; i++) {
    s21_simple_mul(ten_pow, ten_decimal, &tmp_res_ten);
    ten_pow = tmp_res_ten;
    s21_clear_big(&tmp_res_ten);
  }
  s21_simple_mul(big_value, ten_pow, val);
}

int s21_div_with_remainder(s21_big_decimal value_1, s21_big_decimal value_2,
                           s21_big_decimal *result,
                           s21_big_decimal *remainder) {
  s21_big_decimal a = value_1;
  s21_big_decimal previous_res = value_2;
  s21_big_decimal add_value = {{1, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal result_tmp = {{0}};
  int q = 0;

  while (s21_less_big_mantissa_compare(value_2, a, 0) ||
         s21_mantissa_equal_compare(value_2, a)) {
    while (s21_less_big_mantissa_compare(previous_res, a, 0) ||
           s21_mantissa_equal_compare(previous_res, a)) {
      if (q != 0) {
        s21_shift_left_big(&add_value);
        s21_shift_left_big(&previous_res);
      }
      q++;
    }
    s21_shift_right_big(&add_value);
    s21_shift_right_big(&previous_res);
    s21_simple_add(add_value, *result, &result_tmp);
    *result = result_tmp;
    s21_clear_big(&result_tmp);

    s21_big_decimal acp = a;
    s21_clear_big(&a);

    s21_simple_sub(acp, previous_res, &a);
    s21_clear_big(&add_value);
    previous_res = value_2;
    add_value.big_bits[0] = 1;
  }
  *remainder = a;

  return 0;
}

int s21_big_to_decimal(s21_big_decimal *value, s21_decimal *res, int pow,
                       int sign) {
  int flag = OK;
  int round_flag = 0;
  int pow_copy = pow;
  s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal remainder = {{0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal tmp_val = {{0, 0, 0, 0, 0, 0, 0}};

  while ((!(value->big_bits[6] == 0 && value->big_bits[5] == 0 &&
            value->big_bits[4] == 0 && value->big_bits[3] == 0) &&
          (pow_copy >= 0)) ||
         (pow_copy > 28) || (round_flag == 1)) {
    s21_div_with_remainder(*value, ten, &tmp_val, &remainder);

    pow_copy -= 1;
    *value = tmp_val;
    s21_clear_big(&tmp_val);

    if ((value->big_bits[6] == 0 && value->big_bits[5] == 0 &&
         value->big_bits[4] == 0 && value->big_bits[3] == 0) &&
        (pow_copy <= 28) &&
        ((remainder.big_bits[0] > 5) ||
         (remainder.big_bits[0] == 5 && s21_get_bit_big(*value, 0) == 1))) {
      s21_big_decimal one = {{1, 0, 0, 0, 0, 0, 0}};
      s21_simple_add(*value, one, &tmp_val);
      if (tmp_val.big_bits[3] == 0) {
        *value = tmp_val;
        round_flag = 0;
      } else {
        s21_clear_big(&tmp_val);
        round_flag = 1;
      }
    }
    s21_clear_big(&remainder);
  }
  if (pow_copy < 0 && sign == 0)
    flag = TOO_BIG;
  else if (pow_copy < 0 && sign == 1)
    flag = TOO_SMALL;
  else {
    res->bits[0] = value->big_bits[0];
    res->bits[1] = value->big_bits[1];
    res->bits[2] = value->big_bits[2];
    s21_set_scale(res, pow_copy);
    s21_set_bit(res, 127, sign);
  }
  return flag;
}

int s21_max(int val1, int val2) {
  int max_val = val1;
  if (val2 > val1) max_val = val2;
  return max_val;
}
