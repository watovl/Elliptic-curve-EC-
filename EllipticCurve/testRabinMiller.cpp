#include <random>
#include <time.h>
#include "algebraOverField.h"
#include "testRabinMiller.h"


// “ест –абина-ћаллера
bool testRabinMiller(const unsigned long long &n) {
	unsigned long long d = n - 1;
	int s = 0;
	// Ќаходим d такое что, n-1 = 2^s * d (где d нечЄтно)
	while ((d & 1) == 0) {
		d >>= 1;
		++s;
	}
	for (int i = 0; i < COUNT; ++i) {
		unsigned long long a = 2 + rand() % (n - 2);
		unsigned long long z = pow_mod(a, d, n);
		if (z != 1 && z != n - 1) {
			int j = 0;
			for (; j < s - 1; ++j) {
				z = pow_mod(z, 2, n);
				if (z == 1) {
					return false;
				}
				if (z == n - 1) {
					break;
				}
			}
			// если цикл закончилс€ самосто€тельно
			if (j == s - 1) {
				return false;
			}
		}
	}
	return true;
}

// подпрограмма генерации простого числа
const unsigned long long randPrime(const unsigned long long &leftRange, const unsigned long long &rightRange) {
	unsigned long long result;
	srand((unsigned)time(NULL));
	do {
		result = leftRange + (rand() % (rightRange - leftRange));
		if (result == 2)
			return result;
		result |= 1;
	} while (!testRabinMiller(result));
	return result;
}

/* √енераци€ простого числа
 с одним параметром, который €лв€етс€ правым диапозоном
 или без параметров, где правый диапозон равен RAND_MAX
*/
unsigned long long randomPrime(const unsigned long long &rightRange) {
	return randPrime(2, rightRange);
}

/* √енераци€ простого числа
 с двум€ параметрами, левый и правый диапозон,
 если левый диапозон больше правого, возвращает 0
*/
unsigned long long randomPrime(const unsigned long long &leftRange, const unsigned long long &rightRange) {
	return leftRange < rightRange ? randPrime(leftRange, rightRange) : 0;
}

// проверка входного числа на простоту
bool primeCheck(const unsigned long long &value) {
	return value & 1 && value != 1 ? testRabinMiller(value) : false;
}