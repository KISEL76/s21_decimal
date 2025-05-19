#include "../s21_decimal.h"

void s21_div_to_10(s21_decimal *in) {
  int div = 10;
  unsigned rest = 0;
  for (int i = 2; i >= 0; i--) {
    unsigned long int p_rest = rest * 4294967296L + in->bits[i];
    in->bits[i] = p_rest / div;
    rest = p_rest - div * in->bits[i];
  }
}

