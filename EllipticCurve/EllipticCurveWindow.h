#ifndef ELLIPTICCURVEWINDOW_H
#define ELLIPTICCURVEWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QThread>
#include <QVector>
#include "QEllipticCurve.h"
#include "ui_EllipticCurve.h"

class EllipticCurveWindow : public QMainWindow
{
	Q_OBJECT

public:
	EllipticCurveWindow(QWidget *parent = Q_NULLPTR);
	~EllipticCurveWindow();

private slots:
	/* -- НАЖАТИЕ НА КНОПКИ -- */
	
	// блок кнопок для параметров ЭК
	void onRandPrimeButton();
	void onRandParameterAButton();
	void onRandParameterBButton();
	void onSearchPointButton();
	// блок кнопок суммы двух ЭК
	void onRandPointSumButton_1();
	void onRandPointSumButton_2();
	void onSumButton();
	// блок кнопок кратности точки ЭК
	void onRandPointMultButton();
	void onRandValueMultButton();
	void onMulButton();
	// блок кнопок протокола Диффи-Хеллмана на ЭК
	void onRandPointDHButton();
	void onRandValueADHButton();
	void onRandValueBDHButton();
	void onProtocolDHButton();
	// блок кнопок протокола Мэсси-Омуры на ЭК
	void onRandPointMOButton();
	void onRandValueEMOButton_1();
	void onRandValueEMOButton_2();
	void onProtocolMOButton();
	// блок кнопок криптосистемы Эль-Гамаля
	void onRandPointBEGButton();
	void onRandPointPEGButton();
	void onRandValueAEGButton();
	void onRandValueBEGButton();
	void onCryptosystemEGButton();

	/* -- ОБРАБОТКА РЕЗУЛЬТАТОВ ПОСЛЕ ЗАВЕРШЕНИЯ ВЫЧИСЛЕНИЙ -- */
	void handleSearchPoints(const QVector<Point> &result);
	void handleSumPoints(const Point &result);
	void handleMultPoint(const Point &result);
	void handleIntermProtocolDH(const Point &pointAlice, const Point &pointBob);
	void handleProtocolDH(const Point &resultPointAlice, const Point &resultPointBob);
	void handleInverseValueProtocolMO(const long long &inverseAlice, const long long &inverseBob);
	void handleProtocolMO(const Point &pointEB, const Point &pointEEB, const Point &pointDEEB, const Point &pointDDEEB);
	void handleCryptosystemEG(const Point &pointBob, const Point &pointKey1, const Point &pointKey2, const Point &secretPoint);

signals:
	void startSearchPoints(const long long &a, const long long &b, const long long &p, const int &r);
	void startGraphEC(const long long &a, const long long &b);
	void startSumPoints(const Point &point1, const Point &point2, const long long &a, const long long &field);
	void startMultPoint(const Point &point, const long long &value, const long long &a, const long long &field, const long long countPoint = -1);
	void startProtocolDH(const Point &point, const long long &Alice, const long long &Bob, const long long &a, const long long &field, const long long countPoint = -1);
	void startProtocolMO(const Point &point, const long long &e1, const long long &e2, const long long &a, const long long &field, const long long &countPoint = -1);
	void startCryptosystemEG(const Point &publicPoint, const Point &secretPoint, const long long &Alice, const long long &Bob, const long long &a, const long long &field, const long long countPoint = -1);

private:
	Ui::EllipticCurveClass ui;
	QThread *thread;
	long long prime;
	int degree;
	long long field;
	long long parameterA;
	long long parameterB;
	QVector<Point> pointsEC;
	unsigned sizePoints;

	// подфункции
	void settingThread();
	void settingConnectButtons();
	bool sucсessReadPoint(const bool &okX, const bool &okY);
	bool successReadNumber(const bool &ok);
	bool existsPoint(const Point &point);
	void printPoint(QLabel *&label, const Point &point);
	void printPoints();
	long long randParameterEC(const long long &field);
};

#endif