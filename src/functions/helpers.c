// +-------------------------------------------------+
// |        Code Authored by Vladimir Kiselev        |
// |                Year: 2024                       |
// |      > Syntax: Approved | Logic: Verified       |
// +-------------------------------------------------+

#include "../s21_decimal.h"

int s21_get_bit(s21_decimal number, int index) {
  int bit = (number.bits[index / 32] >> (index % 32)) & 1;
  return bit;
}

void s21_set_bit(s21_decimal *source, int index, int bit) {
  if (bit != 0) {
    source->bits[index / 32] = source->bits[index / 32] | (bit << index);
  } else {
    source->bits[index / 32] = source->bits[index / 32] & ~((bit + 1) << index);
  }
}

int s21_get_bit_val(unsigned int number, int index) {
  int val = (number >> (index % 32)) & 1;
  return val;
}

void s21_set_bit_val(unsigned int *number, int bit, int index) {
  if (bit != 0 && s21_get_bit_val(*number, index) == 0) {
    *number = *number | (bit << index);
  } else if (bit == 0 && s21_get_bit_val(*number, index) == 1) {
    *number = *number & ~((bit + 1) << index);
  }
}

void s21_shift_left(s21_decimal *number) {
  int low_last = 0;
  int middle_last = 0;
  if (s21_get_bit_val(number->bits[low], 31) == 1) {
    low_last = 1;
  }
  if (s21_get_bit_val(number->bits[middle], 31) == 1) {
    middle_last = 1;
  }
  number->bits[low] <<= 1;
  number->bits[middle] <<= 1;
  number->bits[high] <<= 1;

  if (low_last == 1) {
    s21_set_bit_val(&number->bits[middle], 1, 0);
  }
  if (middle_last == 1) {
    s21_set_bit_val(&number->bits[high], 1, 0);
  }
}

void s21_shift_right(s21_decimal *number) {
  int middle_first = 0;
  int high_first = 0;
  if (s21_get_bit_val(number->bits[middle], 31) == 1) {
    middle_first = 1;
  }
  if (s21_get_bit_val(number->bits[high], 31) == 1) {
    high_first = 1;
  }
  number->bits[low] >>= 1;
  number->bits[middle] >>= 1;
  number->bits[high] >>= 1;

  if (middle_first == 1) {
    s21_set_bit_val(&number->bits[low], 1, 31);
  }
  if (high_first == 1) {
    s21_set_bit_val(&number->bits[middle], 1, 31);
  }
}

int s21_get_sign(s21_decimal number) {
  return (s21_get_bit_val(number.bits[scale], 31));
}

void s21_set_sign(s21_decimal *number, int sign) {
  s21_set_bit_val(&number->bits[scale], sign, 31);
}

int s21_get_scale(s21_decimal number) {
  int mask = 127 << 16;
  return (mask & number.bits[scale]) >> 16;
  ;
}

void s21_set_scale(s21_decimal *number, int pow) {
  int mask = pow << 16;
  int isMinus = s21_get_sign(*number);
  number->bits[scale] = mask;
  if (isMinus) s21_set_sign(number, 1);
}

void s21_clear(s21_decimal *number) {
  number->bits[0] = 0;
  number->bits[1] = 0;
  number->bits[2] = 0;
  number->bits[3] = 0;
}

int s21_decimal_check(s21_decimal temp) {
  int status = 0;
  if (s21_get_scale(temp) < 0 || s21_get_scale(temp) > 28) {
    status = 1;
  }
  if (NULL_DEC != (temp.bits[3] &= ERROR_DEC)) {
    status = 1;
  }
  return status;
}

void s21_set_degree(s21_decimal *val, int degree) {
  int sign = s21_get_sign(*val);
  val->bits[3] = (degree << 16) | (sign << 31);
}
int s21_get_degree(s21_decimal val) { return (val.bits[3] & 0xFF0000) >> 16; }

void s21_decimal_init(s21_decimal *target) {
  for (int i = 0; i < 3 + 1; i++) {
    target->bits[i] = 0;
  }
}

void s21_floatround(double *temp_number) {
  double mantissa = *temp_number;
  long int whole_part = *temp_number;
  whole_part *= 100000000;
  mantissa *= 100000000;
  long int remainder = mantissa;
  remainder -= whole_part;

  if (remainder == 50000000) {
    long int digit = whole_part / 100000000;
    digit -= (whole_part / 1000000000) * 10;

    if ((digit % 2) == 1) {
      *temp_number += 1.0;
    }
  }
  if (remainder != 50000000) {
    *temp_number = roundl(*temp_number);
  }
}

void s21_scale_tuning(double *temp_number, int *scale, int *scale_diff,
                      floatbits mantissa) {
  int scale_whole_part = 0;
  double whole_part = *temp_number;
  while ((*scale < 28) && (((*temp_number < 999999) && (mantissa.fl >= 1)) ||
                           ((*temp_number < 1000000) && (mantissa.fl < 1)))) {
    *temp_number *= 10;
    (*scale)++;
  }
  if (*scale == 0) {
    // уменьшение масштаба
    while (whole_part > 1) {
      whole_part /= 10;
      scale_whole_part++;
    }
  }
  while (scale_whole_part > 7) {
    *temp_number /= 10;
    scale_whole_part--;
    (*scale_diff)++;
  }
  while (((int)fmod(*temp_number, 10) == 0) && (*scale > 0) &&
         ((int)fmod(*temp_number * 10, 10) <= 5)) {
    *temp_number /= 10;
    (*scale)--;
  }
}

int s21_exp(floatbits mantissa) {
  return ((mantissa.ui & ~(1u << 31)) >> 23) - 127;
}
