#ifndef ELLIPTICCURVE_H
#define ELLIPTICCURVE_H

// точка ЭК
struct Point {
	long long x;
	long long y;

	friend bool operator== (const Point &p1, const Point &p2) { return p1.x == p2.x && p1.y == p2.y; };
	friend bool operator!= (const Point &p1, const Point &p2) { return !(p1 == p2); };
};

enum statusSymbol : short {non_residue = -1, divisor, residue};

// точка на бесконечности
const Point PointAtInfinity{ -1, -1 };

// поиск точек эллиптической кривой
//std::vector<Point> searchPointsEC(const long long &a, const long long &b, const long long &field);
std::vector<Point> searchPointsEC(const long long &a, const long long &b, const long long &p, const int &r);

// сумма двух точек ЭК
Point sumPointsEC(const Point &point1, const Point &point2, const long long &a, const long long &field);

// умножение точки на число
Point multiplicityPointEC(const Point &point, const long long &value, const long long &a, const long long &field, const long long countPoint = -1);

// вычисление количества точек ЭК
unsigned long long countPointsEC(const long long &a, const long long &b, const long long &p, const int &r);

#endif