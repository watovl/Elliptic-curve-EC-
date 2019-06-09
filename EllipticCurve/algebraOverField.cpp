#include "algebraOverField.h"

// вычисление модуля
inline long long mod(const long long &base, const long long &field) {
	return base < 0 ? field + (base % field) : base % field;
}

// быстрое возведение в степень по модулю (base^power % field)
long long pow_mod(const long long &in_base, const unsigned long long &power, const long long &field) {
	if (power == 0) {
		return 1;
	}
	long long res = 1;
	long long base = in_base;
	unsigned long long pow = power;
	while (pow) {
		if (pow & 1) {
			res = mod(res * base, field);
		}
		base = mod(base, field) * mod(base, field) % field;
		pow >>= 1;
	}
	return res;
}