#include <random>
#include <time.h>
#include "algebraOverField.h"
#include "testRabinMiller.h"


// ���� ������-�������
bool testRabinMiller(const unsigned long long &n) {
	unsigned long long d = n - 1;
	int s = 0;
	// ������� d ����� ���, n-1 = 2^s * d (��� d �������)
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
			// ���� ���� ���������� ��������������
			if (j == s - 1) {
				return false;
			}
		}
	}
	return true;
}

// ������������ ��������� �������� �����
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

/* ��������� �������� �����
 � ����� ����������, ������� �������� ������ ����������
 ��� ��� ����������, ��� ������ �������� ����� RAND_MAX
*/
unsigned long long randomPrime(const unsigned long long &rightRange) {
	return randPrime(2, rightRange);
}

/* ��������� �������� �����
 � ����� �����������, ����� � ������ ��������,
 ���� ����� �������� ������ �������, ���������� 0
*/
unsigned long long randomPrime(const unsigned long long &leftRange, const unsigned long long &rightRange) {
	return leftRange < rightRange ? randPrime(leftRange, rightRange) : 0;
}

// �������� �������� ����� �� ��������
bool primeCheck(const unsigned long long &value) {
	return value & 1 && value != 1 ? testRabinMiller(value) : false;
}