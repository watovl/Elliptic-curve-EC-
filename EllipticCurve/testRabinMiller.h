#ifndef TESTRM_H
#define TESTRM_H

// ���������� �������� ��� �����
constexpr unsigned COUNT = 10;

// ��������� �������� �����
unsigned long long randomPrime(const unsigned long long &rightRange = RAND_MAX);
unsigned long long randomPrime(const unsigned long long &leftRange, const unsigned long long &rightRange);

// �������� �������� ����� �� ��������
bool primeCheck(const unsigned long long &value);

#endif