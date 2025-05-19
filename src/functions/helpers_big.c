// +-------------------------------------------------+
// |        Code Authored by Vladimir Kiselev        |
// |                Year: 2024                       |
// |      > Syntax: Approved | Logic: Verified       |
// +-------------------------------------------------+

#include "../s21_decimal.h"

int s21_get_bit_big(s21_big_decimal number, int index) {
  int bit = (number.big_bits[index / 32] >> (index % 32)) & 1;
  return bit;
}

void s21_set_bit_big(s21_big_decimal *source, int index, int bit) {
  if (bit != 0) {
    source->big_bits[index / 32] =
        source->big_bits[index / 32] | (bit << index);
  } else {
    source->big_bits[index / 32] =
        source->big_bits[index / 32] & ~((bit + 1) << index);
  }
}

void s21_shift_left_big(s21_big_decimal *number) {
  int last_0 = 0;
  int last_1 = 0;
  int last_2 = 0;
  int last_3 = 0;
  int last_4 = 0;
  int last_5 = 0;

  if (s21_get_bit_val(number->big_bits[0], 31) == 1) {
    last_0 = 1;
  }
  if (s21_get_bit_val(number->big_bits[1], 31) == 1) {
    last_1 = 1;
  }
  if (s21_get_bit_val(number->big_bits[2], 31) == 1) {
    last_2 = 1;
  }
  if (s21_get_bit_val(number->big_bits[3], 31) == 1) {
    last_3 = 1;
  }
  if (s21_get_bit_val(number->big_bits[4], 31) == 1) {
    last_4 = 1;
  }
  if (s21_get_bit_val(number->big_bits[5], 31) == 1) {
    last_5 = 1;
  }

  number->big_bits[0] <<= 1;
  number->big_bits[1] <<= 1;
  number->big_bits[2] <<= 1;
  number->big_bits[3] <<= 1;
  number->big_bits[4] <<= 1;
  number->big_bits[5] <<= 1;
  number->big_bits[6] <<= 1;

  if (last_0 == 1) {
    s21_set_bit_val(&number->big_bits[1], 1, 0);
  }
  if (last_1 == 1) {
    s21_set_bit_val(&number->big_bits[2], 1, 0);
  }
  if (last_2 == 1) {
    s21_set_bit_val(&number->big_bits[3], 1, 0);
  }
  if (last_3 == 1) {
    s21_set_bit_val(&number->big_bits[4], 1, 0);
  }
  if (last_4 == 1) {
    s21_set_bit_val(&number->big_bits[5], 1, 0);
  }
  if (last_5 == 1) {
    s21_set_bit_val(&number->big_bits[6], 1, 0);
  }
}

void s21_shift_right_big(s21_big_decimal *number) {
  int first_1 = 0;
  int first_2 = 0;
  int first_3 = 0;
  int first_4 = 0;
  int first_5 = 0;
  int first_6 = 0;
  if (s21_get_bit_val(number->big_bits[1], 0) == 1) {
    first_1 = 1;
  }
  if (s21_get_bit_val(number->big_bits[2], 0) == 1) {
    first_2 = 1;
  }
  if (s21_get_bit_val(number->big_bits[3], 0) == 1) {
    first_3 = 1;
  }
  if (s21_get_bit_val(number->big_bits[4], 0) == 1) {
    first_4 = 1;
  }
  if (s21_get_bit_val(number->big_bits[5], 0) == 1) {
    first_5 = 1;
  }
  if (s21_get_bit_val(number->big_bits[6], 0) == 1) {
    first_6 = 1;
  }
  number->big_bits[0] >>= 1;
  number->big_bits[1] >>= 1;
  number->big_bits[2] >>= 1;
  number->big_bits[3] >>= 1;
  number->big_bits[4] >>= 1;
  number->big_bits[5] >>= 1;
  number->big_bits[6] >>= 1;

  if (first_1 == 1) {
    s21_set_bit_val(&number->big_bits[0], 1, 31);
  }
  if (first_2 == 1) {
    s21_set_bit_val(&number->big_bits[1], 1, 31);
  }
  if (first_3 == 1) {
    s21_set_bit_val(&number->big_bits[2], 1, 31);
  }
  if (first_4 == 1) {
    s21_set_bit_val(&number->big_bits[3], 1, 31);
  }
  if (first_5 == 1) {
    s21_set_bit_val(&number->big_bits[4], 1, 31);
  }
  if (first_6 == 1) {
    s21_set_bit_val(&number->big_bits[5], 1, 31);
  }
}

void s21_clear_big(s21_big_decimal *number) {
  number->big_bits[0] = 0;
  number->big_bits[1] = 0;
  number->big_bits[2] = 0;
  number->big_bits[3] = 0;
  number->big_bits[4] = 0;
  number->big_bits[5] = 0;
  number->big_bits[6] = 0;
}

s21_big_decimal s21_to_big(s21_decimal *number) {
  s21_big_decimal extended_decimal = {0};

  extended_decimal.big_bits[0] = number->bits[0];
  extended_decimal.big_bits[1] = number->bits[1];
  extended_decimal.big_bits[2] = number->bits[2];
  extended_decimal.big_bits[3] = 0;
  extended_decimal.big_bits[4] = 0;
  extended_decimal.big_bits[5] = 0;
  extended_decimal.big_bits[6] = 0;

  return extended_decimal;
}
