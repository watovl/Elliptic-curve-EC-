#ifndef TESTRM_H
#define TESTRM_H

// количество проходов для теста
constexpr unsigned COUNT = 10;

// генерация простого числа
unsigned long long randomPrime(const unsigned long long &rightRange = RAND_MAX);
unsigned long long randomPrime(const unsigned long long &leftRange, const unsigned long long &rightRange);

// проверка входного числа на простоту
bool primeCheck(const unsigned long long &value);

#endif