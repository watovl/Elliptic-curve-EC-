#ifndef ALGEBRAOVERFIELD_H
#define ALGEBRAOVERFIELD_H

// ���������� ������
inline long long mod(const long long &base, const long long &field) {
	return base < 0 ? field + (base % field) : base % field;
}

// ������� ���������� � ������� �� ������ (base^power % field)
long long pow_mod(const long long &in_base, const unsigned long long &power, const long long &field);

#endif // !ALGEBRAOVERFIELD_H