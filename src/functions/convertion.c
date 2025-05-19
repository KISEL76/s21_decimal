#include "../s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int status = 0;
  if (dst == NULL || abs(src) > MAX_DEC || abs(src) < MIN_DEC) {
    status = 1;
  } else {
    s21_clear(dst);
    if (src < 0) {
      src = -1 * src;
      s21_set_sign(dst, 1);
    }
    dst->bits[0] = src;
  }
  return status;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int status = 0;
  if (dst == NULL || s21_decimal_check(src)) {
    status = 1;
  } else {
    s21_decimal temp;
    s21_truncate(src, &temp);
    // если была дробная часть - отсекаем
    int sign = s21_get_sign(temp);
    if ((temp.bits[2] == 0) && (temp.bits[1] == 0) &&
        (temp.bits[0] < 2147483648 || (sign && temp.bits[0] == 2147483648))) {
      // убеждаемся, что число не больше максимально возможного инта и
      // преобразуем
      *dst = temp.bits[0];
      if (sign) {
        *dst = -*dst;
      }
    } else {
      status = 1;
    }
  }
  return status;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int status = 0;
  if (dst == NULL || s21_decimal_check(src)) {
    status = 1;
  } else {
    *dst = 0;
    int sign = 1;
    long double result = 0;
    long double binary = 1;
    int exp = s21_get_scale(src);
    if (s21_get_sign(src) == 1) {
      sign = -1;
    }
    for (int i = 0; i < 96; i++) {
      // если бит=1 - добавляем в result
      if (s21_get_bit(src, i)) {
        result += binary;
      }
      binary *= 2;
      // переход к следующему биту
    }
    for (int i = 0; i < exp; i++) {
      result /= 10;
    }
    *dst = sign * result;
  }
  return status;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int status = 0;
  if (dst == NULL || (fabs(src) > MAX_DEC) || (fabs(src) == INFINITY) ||
      isnan(fabs(src))) {
    status = 1;
  } else if (fabs(src) > 0 && fabs(src) < 1e-28) {
    status = 1;
    s21_clear(dst);
  } else if (src == 0.0) {
    s21_clear(dst);
  } else {
    s21_clear(dst);
    floatbits mantissa = {0};
    int scale = 0;
    mantissa.fl = src;
    int sign = (src < 0) ? 1 : 0;
    if (sign == 1) {
      mantissa.fl *= -1;
      s21_set_sign(dst, 1);
    }
    int exp = s21_exp(mantissa);
    double temp_number = (double)mantissa.fl;

    if (exp >= -94 && exp < 96) {
      // если уложились в диапазон доопустимой экспоненты
      int scale_diff = 0;

      s21_scale_tuning(&temp_number, &scale, &scale_diff, mantissa);
      s21_floatround(&temp_number);
      mantissa.fl = (float)temp_number;
      int exponent = s21_exp(mantissa);
      s21_set_bit(dst, exponent, 1);
      for (int i = exponent - 1, j = 22; j >= 0 && i >= 0; i--, j--) {
        // установка битов мантиссы
        if ((mantissa.ui & (1 << j)) != 0) {
          s21_set_bit(dst, i, 1);
        }
      }
      s21_set_scale(dst, scale);
      if (scale_diff > 0) {
        // коррекци масштаба
        s21_decimal ten = {{0xA, 0x0, 0x0, 0x0}};
        for (int i = scale_diff; i > 0; i--) s21_mul(*dst, ten, dst);
      }
    }
  }
  return status;
}