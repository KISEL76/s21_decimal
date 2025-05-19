// +-------------------------------------------------+
// |        Code Authored by Vladimir Kiselev        |
// |                Year: 2024                       |
// |      > Syntax: Approved | Logic: Verified       |
// +-------------------------------------------------+

#include "../s21_decimal.h"

int s21_is_zero(s21_decimal a, s21_decimal b) {
  int is_null = FALSE;

  if (a.bits[0] == 0 && a.bits[1] == 0 && a.bits[2] == 0 && b.bits[0] == 0 &&
      b.bits[1] == 0 && b.bits[2] == 0)
    is_null = TRUE;

  return is_null;
}

int s21_mantissa_equal_compare(s21_big_decimal A, s21_big_decimal B) {
  int result = FALSE;
  if ((A.big_bits[0] == B.big_bits[0]) && (A.big_bits[1] == B.big_bits[1]) &&
      (A.big_bits[2] == B.big_bits[2]) && (A.big_bits[3] == B.big_bits[3]) &&
      (A.big_bits[4] == B.big_bits[4]) && (A.big_bits[5] == B.big_bits[5]) &&
      (A.big_bits[6] == B.big_bits[6])) {
    result = TRUE;
  } else {
    result = FALSE;
  }
  return result;
}

int s21_less_mantissa_compare(s21_decimal a, s21_decimal b, int sign) {
  int result = FALSE;
  int flag = 0;

  if (sign == NEGATIVE) {
    for (int i = 2; i >= 0 && !flag; i--) {
      if (a.bits[i] == b.bits[i])
        continue;
      else if (a.bits[i] > b.bits[i]) {
        result = TRUE;
        flag = 1;
      } else {
        result = FALSE;
        flag = 1;
      }
    }
    return result;
  } else if (sign == POSITIVE) {
    for (int i = 2; i >= 0 && !flag; i--) {
      if (a.bits[i] == b.bits[i])
        continue;
      else if (a.bits[i] > b.bits[i]) {
        result = FALSE;
        flag = 1;
      } else {
        result = TRUE;
        flag = 1;
      }
    }
  }
  return result;
}

int s21_less_big_mantissa_compare(s21_big_decimal A, s21_big_decimal B,
                                  int sign) {
  int result = FALSE;
  int flag = 0;

  if (sign == NEGATIVE) {
    for (int i = 6; i >= 0 && !flag; i--) {
      if (A.big_bits[i] == B.big_bits[i])
        continue;
      else if (A.big_bits[i] > B.big_bits[i]) {
        result = TRUE;
        flag = 1;
      } else {
        result = FALSE;
        flag = 1;
      }
    }
  } else if (sign == POSITIVE) {
    for (int i = 6; i >= 0 && !flag; i--) {
      if (A.big_bits[i] == B.big_bits[i])
        continue;
      else if (A.big_bits[i] > B.big_bits[i]) {
        result = FALSE;
        flag = 1;
      } else {
        result = TRUE;
        flag = 1;
      }
    }
  }
  return result;
}

int s21_is_equal(s21_decimal a, s21_decimal b) {
  int result = FALSE;
  if (s21_is_zero(a, b))
    result = TRUE;
  else if (s21_get_sign(a) == s21_get_sign(b)) {
    if (s21_get_scale(a) == s21_get_scale(b)) {
      if (a.bits[0] == b.bits[0] && a.bits[1] == b.bits[1] &&
          a.bits[2] == b.bits[2])
        result = TRUE;
    } else {
      s21_big_decimal A = {{0}};
      s21_big_decimal B = {{0}};
      s21_normalize(a, b, &A, &B);
      result = s21_mantissa_equal_compare(A, B);
    }
  } else {
    result = FALSE;
  }
  return result;
}

int s21_is_not_equal(s21_decimal a, s21_decimal b) {
  return (!s21_is_equal(a, b));
}

int s21_is_less(s21_decimal a, s21_decimal b) {
  int result = FALSE;
  int sign_a = s21_get_sign(a);
  int sign_b = s21_get_sign(b);

  if (s21_is_equal(a, b) == TRUE)
    result = FALSE;
  else if (sign_a == sign_b) {
    if (s21_get_scale(a) == s21_get_scale(b)) {
      result = s21_less_mantissa_compare(a, b, sign_a);
    } else {
      s21_big_decimal A = {{0}};
      s21_big_decimal B = {{0}};
      s21_normalize(a, b, &A, &B);
      result = s21_less_big_mantissa_compare(A, B, sign_a);
    }
  } else if (sign_a > sign_b) {
    result = TRUE;
  } else {
    result = FALSE;
  }
  return result;
}

int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
  int result = FALSE;

  if (s21_is_less(a, b) || s21_is_equal(a, b)) result = TRUE;

  return result;
}

int s21_is_greater(s21_decimal a, s21_decimal b) {
  int result = FALSE;

  result = !s21_is_less(a, b);
  if (s21_is_equal(a, b) == TRUE) result = FALSE;

  return result;
}

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
  int result = FALSE;

  result = !s21_is_less(a, b);

  return result;
}
