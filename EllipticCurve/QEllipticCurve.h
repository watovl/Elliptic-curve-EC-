#ifndef QELLIPTICCURVE_H
#define QELLIPTICCURVE_H

#include <QObject>
#include <QVector>
#include "EllipticCurve.h"
#include "algebraOverField.h"

class QEllipticCurve : public QObject {
	Q_OBJECT

public slots:
	void searchPoints(const long long &a, const long long &b, const long long &p, const int &r) {
		emit resultSearchPoints(QVector<Point>::fromStdVector(searchPointsEC(a, b, p, r)));
	}

	void sumPoints(const Point &point1, const Point &point2, const long long &a, const long long &field) {
		emit resultSumPoints(sumPointsEC(point1, point2, a, field));
	}

	void multiplicityPoint(const Point &point, const long long &value, const long long &a, const long long &field, const long long countPoint = -1) {
		emit resultMultPoint(multiplicityPointEC(point, value, a, field, countPoint));
	}

	void protocolDiffieHellman(const Point &point, const long long &Alice, const long long &Bob, const long long &a, const long long &field, const long long countPoint = -1) {
		const Point pointAlice = multiplicityPointEC(point, Alice, a, field, countPoint);
		const Point pointBob = multiplicityPointEC(point, Bob, a, field, countPoint);
		emit resultIntermProtocolDH(pointAlice, pointBob);
		const Point resultAlice = multiplicityPointEC(pointBob, Alice, a, field, countPoint);
		const Point resultBob = multiplicityPointEC(pointAlice, Bob, a, field, countPoint);
		emit resultProtocolDH(resultAlice, resultBob);
	}

	void protocolMasseyOmura(const Point &point, const long long &e1, const long long &e2, const long long &a, const long long &field, const long long countPoint) {
		const long long d1 = pow_mod(e1, countPoint - 2, countPoint);
		const long long d2 = pow_mod(e2, countPoint - 2, countPoint);
		emit resultInverseValueProtocolMO(d1, d2);
		const Point eB = multiplicityPointEC(point, e1, a, field, countPoint);
		const Point eeB = multiplicityPointEC(eB, e2, a, field, countPoint);
		const Point deeB = multiplicityPointEC(eeB, d1, a, field, countPoint);
		const Point ddeeB = multiplicityPointEC(deeB, d2, a, field, countPoint);
		emit resultProtocolMO(eB, eeB, deeB, ddeeB);
	}

	void cryptosystemElGamal(const Point &publicPoint, const Point &secretPoint, const long long &Alice, const long long &Bob, const long long &a, const long long &field, const long long countPoint = -1) {
		const Point pointBob = multiplicityPointEC(publicPoint, Bob, a, field, countPoint);
		const Point pointAliceKey_1 = multiplicityPointEC(publicPoint, Alice, a, field, countPoint);
		const Point pointAliceKey_2 = sumPointsEC(secretPoint, multiplicityPointEC(pointBob, Alice, a, field, countPoint), a, field);
		const Point tempPoint = multiplicityPointEC(pointAliceKey_1, Bob, a, field, countPoint);
		const Point reversePoint = { tempPoint.x, field - tempPoint.y };
		const Point secretPointBob = sumPointsEC(pointAliceKey_2, reversePoint, a, field);
		emit resultCryptosystemEG(pointBob, pointAliceKey_1, pointAliceKey_2, secretPointBob);
	}

signals:
	void resultSearchPoints(const QVector<Point> &result);
	void resultSumPoints(const Point &resultPoint);
	void resultMultPoint(const Point &resultPoint);
	void resultIntermProtocolDH(const Point &Alice, const Point &Bob);
	void resultProtocolDH(const Point &resultAlice, const Point &resultBob);
	void resultInverseValueProtocolMO(const long long &d1, const long long &d2);
	void resultProtocolMO(const Point &ePoint, const Point &eePoint, const Point &deePoint, const Point &ddeePoint);
	void resultCryptosystemEG(const Point &pointBob, const Point &pointKey1, const Point &pointKey2, const Point &secretPoint);
};

#endif // !QEC_H