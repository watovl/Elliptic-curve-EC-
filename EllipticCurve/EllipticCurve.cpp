#include <vector>
#include <queue>
#include "algebraOverField.h"
#include "EllipticCurve.h"

// Символ Лежандра для простых нечётных полей
// возвращает 0 (divisor), если а делится на p;
// - 1 (non_residue), если не является квадратичным вычетом;
// 1 (residue) - квадратичный вычет
short LegendreSymbol(const long long &value, const long long &field) {
	long long a = mod(value, field);
	if (!a) return 0;
	if (a == 1) return 1;
	long long p = field;
	short res = 1;
	while (a) {
		// удаление двоек из а, пока а чётное
		int s = 0;
		while (!(a & 1)) {
			a >>= 1;
			++s;
		}
		// если s - четное, выбор знака
		if (s & 1) {
			// формула (-1) ^ ((p^2 - 1) / 8) заменена на взятие по модулю 8
			const short m = (short)mod(p, 8);
			res *= m == 3 || m == 5 ? -1 : 1;
		}
		// а - нечётное, переворот символа
		// формула (-1) ^ ((a - 1) * (p - 1) / 4) заменена на взятие по модулю 4
		res *= (mod(a, 4) & mod(p, 4)) == 3 ? -1 : 1;
		std::swap(a, p);
		a = mod(a, p);
	}
	return res;
}

// Символ Кронекера для всех целых чисел
// возвращает 0 (divisor), если а делится на p;
// - 1 (non_residue), если не является квадратичным вычетом;
// 1 (residue) - квадратичный вычет
short KroneckerSymbol(const long long &value, const long long &field) {
	// value и field чётные
	if (value & 1 && field & 1) return 0;
	long long a = mod(value, field);
	long long p = field;
	short res = 1;
	// удаление двоек из p, пока p чётное
	int s = 0;
	while (!(p & 1)) {
		p >>= 1;
		++s;
	}
	// s - нечётное, выбор знака
	if (!(s & 1)) {
		// формула (-1) ^ ((a^2 - 1) / 8) заменена на взятие по модулю 8
		const short m = (short)mod(a, 8);
		res = m == 3 || m == 5 ? -1 : 1;
	}
	while (a) {
		s = 0;
		// удаление двоек из a, пока a чётное
		while (!(a & 1)) {
			a >>= 1;
			++s;
		}
		// если s - четное, выбор знака
		if (s & 1) {
			// формула (-1) ^ ((p^2 - 1) / 8) заменена на взятие по модулю 8
			const short m = (short)mod(p, 8);
			res *= m == 3 || m == 5 ? -1 : 1;
		}
		// а - нечётное, переворот символа
		// формула (-1) ^ ((a - 1) * (p - 1) / 4) заменена на взятие по модулю 4
		res *= (mod(a, 4) & mod(p, 4)) == 3 ? -1 : 1;
		std::swap(a, p);
		a = mod(a, p);
	}
	return p == 1 ? res : statusSymbol::non_residue;
}

// алгоритм Тонелли-Шенкса извлечения квадратного корня по модулю
// при значении третьего параметра = -1, происходит поиск невычета
long long TonelliShanksAlgorithm(const long long &n, const long long &p, long long &nonResidue) {
	// квадратичный вычет не найден
	if (nonResidue == -1) {
		bool check = false;
		// поиск невычита
		while (!check) {
			nonResidue = rand() % p;
			if (LegendreSymbol(nonResidue, p) == statusSymbol::non_residue) {
				check = true;
			}
		}
	}
	// Находим q такое что, p-1 = 2^s * q (где q нечётно)
	static unsigned long long q = 0;
	static unsigned long long previousP = 0;
	static int s = 0;
	if (previousP != p) {
		q = p - 1;
		s = 0;
		while ((q & 1) == 0) {
			q >>= 1;
			++s;
		}
		previousP = p;
	}
	// при p = 3 (mod 4) - возвращаем n^((p+1)/4) mod p
	if (s == 1) {
		return pow_mod(n, (p + 1) >> 2, p);
	}
	int m = s;
	long long c = pow_mod(nonResidue, q, p);
	long long r = pow_mod(n, (q + 1) >> 1, p);
	long long t = pow_mod(n, q, p);
	while (t != 1) {
		// находим i, 0 < i < m, такое, что t^(2^i) = 1 (mod p)
		int i = 1;
		for (; i < m; ++i) {
			if (pow_mod(t, 1ui64 << i, p) == 1) break;
		}
		long long b = pow_mod(c, 1ui64 << (m - i - 1), p); //pow(2, m - i - 1)
		r = mod(r * b, p);
		b = pow_mod(b, 2, p);
		t = mod(t * b, p);
		c = b;
		m = i;
	}
	return r;
}

// поиск точек эллиптической кривой
std::vector<Point> searchPointsEC(const long long &a, const long long &b, const long long &p, const int &r) {
	std::vector<Point> pointsEC;
	const long long field = pow(p, r);
	long long nonResidue = -1;
	for (long long x = 0; x < field; ++x) {
		// f(x) = x^3 + a*x + b (mod field)
		const long long functionEllipticCurve = mod(pow_mod(x, 3, field) + mod(a * x, field) + b, field);
		// проверка, является ли число квадратичным вычитом
		const short residueStatus = LegendreSymbol(functionEllipticCurve, p);
		// если символ лежандра == 0, то вторая координата равно 0
		if (residueStatus == statusSymbol::divisor) {
			pointsEC.emplace_back(Point{ x, 0 });
		}
		// если символ лежандра == 1, то f(х) - вычет
		else if (residueStatus == statusSymbol::residue) {
			long long y = TonelliShanksAlgorithm(mod(functionEllipticCurve, p), p, nonResidue);
			// повышение степени вычета с p^r на p^(r+1)
			for (int i = 1; i < r; ++i) {
				//y = y + решение сравнения 2x=-(x^2-a)/p^i (mod p) * p^i
				y = mod(y + mod(pow_mod(mod(2 * y, p), p - 2, p) * mod(-(y*y - functionEllipticCurve) / pow(p, i), p), p) * pow(p, i), pow(p, i + 1));
			}
			pointsEC.emplace_back(Point{ x, y });
			pointsEC.emplace_back(Point{ x, field - y });
		}
	}
	return pointsEC;
}

// сумма двух точек ЭК
Point sumPointsEC(const Point &point1, const Point &point2, const long long &a, const long long &field) {
	Point result;
	long long lambda;
	// суммирование происходит с точкой на бесконечности
	if (point1 == PointAtInfinity) return point2;
	if (point2 == PointAtInfinity) return point1;

	if (point1.x == point2.x) {
		// удвоение точки
		if (point1.y == point2.y) {
			lambda = mod(mod(3 * point1.x * point1.x + a, field) * pow_mod(2 * point1.y, field - 2, field), field);
		}
		// обратные точки, возвращаем точку на бесконечности
		else {
			return PointAtInfinity;
		}
	}
	else {
		lambda = mod(mod(point2.y - point1.y, field) * pow_mod(point2.x - point1.x, field - 2, field), field);
	}
	result.x = mod(pow_mod(lambda, 2, field) - mod(point1.x + point2.x, field), field);
	result.y = mod(mod(lambda * (point1.x - result.x), field) - point1.y, field);
	return result;
}

// преобразование из 10 СС в 2 СС в обратном порядке
std::queue<bool> reversTransDecToBin(const long long &number) {
	std::queue<bool> result;
	long long num = number;
	while (num) {
		result.push(num % 2);
		num >>= 1;
	}
	return result;
}

// умножение точки на число
Point multiplicityPointEC(const Point &point, const long long &value, const long long &a, const long long &field, const long long countPoint) {
	std::queue<bool> binNumber = reversTransDecToBin(countPoint != 1 ? mod(value, countPoint) : value);
	if (!binNumber.size()) return PointAtInfinity;
	Point doublePoint = point;
	Point result;
	binNumber.front() ? result = doublePoint : result = PointAtInfinity;
	binNumber.pop();
	while (!binNumber.empty()) {
		doublePoint = sumPointsEC(doublePoint, doublePoint, a, field);
		if (binNumber.front()) {
			result = sumPointsEC(doublePoint, result, a, field);
		}
		binNumber.pop();
	}
	return result;
}

// подсчёт количества точек ЭК
unsigned long long countPointsEC(const long long &a, const long long &b, const long long &p, const int &r) {
	long long field = pow(p, r);
	unsigned long long count = field + 1;
	for (long long x = 0; x < field; ++x) {
		// f(x) = x^3 + a*x + b (mod field)
		const long long functionEllipticCurve = mod(pow_mod(x, 3, p) + mod(a * x, p) + b, p);
		// проверка, является ли число квадратичным вычитом
		count += LegendreSymbol(functionEllipticCurve, p);
	}
	return count;
}