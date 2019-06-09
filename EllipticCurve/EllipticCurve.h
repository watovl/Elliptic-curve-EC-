#ifndef ELLIPTICCURVE_H
#define ELLIPTICCURVE_H

// ����� ��
struct Point {
	long long x;
	long long y;

	friend bool operator== (const Point &p1, const Point &p2) { return p1.x == p2.x && p1.y == p2.y; };
	friend bool operator!= (const Point &p1, const Point &p2) { return !(p1 == p2); };
};

enum statusSymbol : short {non_residue = -1, divisor, residue};

// ����� �� �������������
const Point PointAtInfinity{ -1, -1 };

// ����� ����� ������������� ������
//std::vector<Point> searchPointsEC(const long long &a, const long long &b, const long long &field);
std::vector<Point> searchPointsEC(const long long &a, const long long &b, const long long &p, const int &r);

// ����� ���� ����� ��
Point sumPointsEC(const Point &point1, const Point &point2, const long long &a, const long long &field);

// ��������� ����� �� �����
Point multiplicityPointEC(const Point &point, const long long &value, const long long &a, const long long &field, const long long countPoint = -1);

// ���������� ���������� ����� ��
unsigned long long countPointsEC(const long long &a, const long long &b, const long long &p, const int &r);

#endif