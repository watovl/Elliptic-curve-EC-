#include "EllipticCurveWindow.h"
#include <vector>
#include <QTextCodec>
#include <QMessageBox>
#include <QErrorMessage>
#include <QThread>
#include "testRabinMiller.h"
#include "algebraOverField.h"
#include "GraphEllipticCurve.h"

EllipticCurveWindow::EllipticCurveWindow(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows - 1251"));
	qRegisterMetaType<Point>("Point");
	qRegisterMetaType<std::vector<Point>>("std::vector<Point>");
	ui.primeLineEdit->setFocus();
	settingThread();
	settingConnectButtons();
}

EllipticCurveWindow::~EllipticCurveWindow() {
	thread->quit();
	thread->deleteLater();
	thread->wait();
	delete thread;
}

// ��������� ������������ �������
void EllipticCurveWindow::settingThread() {
	thread = new QThread;
	QEllipticCurve *qec = new QEllipticCurve;
	qec->moveToThread(thread);
	// ����� ������ ����������
	connect(this, SIGNAL(startSearchPoints(const long long &, const long long &, const long long &, const int &)), 
		qec, SLOT(searchPoints(const long long &, const long long &, const long long &, const int &)));
	connect(this, SIGNAL(startSumPoints(const Point &, const Point &, const long long &, const long long &)),
		qec, SLOT(sumPoints(const Point &, const Point &, const long long &, const long long &)));
	connect(this, SIGNAL(startMultPoint(const Point &, const long long &, const long long &, const long long &, const long long)),
		qec, SLOT(multiplicityPoint(const Point &, const long long &, const long long &, const long long &, const long long)));
	connect(this, SIGNAL(startProtocolDH(const Point &, const long long &, const long long &, const long long &, const long long &, const long long &)),
		qec, SLOT(protocolDiffieHellman(const Point &, const long long &, const long long &, const long long &, const long long &, const long long &)));
	connect(this, SIGNAL(startProtocolMO(const Point &, const long long &, const long long &, const long long &, const long long &, const long long &)),
		qec, SLOT(protocolMasseyOmura(const Point &, const long long &, const long long &, const long long &, const long long &, const long long &)));
	connect(this, SIGNAL(startCryptosystemEG(const Point &, const Point &, const long long &, const long long &, const long long &, const long long &, const long long &)),
		qec, SLOT(cryptosystemElGamal(const Point &, const Point &, const long long &, const long long &, const long long &, const long long &, const long long &)));
	// ����� ��������� ����������� ����������
	connect(qec, SIGNAL(resultSearchPoints(const std::vector<Point> &)), this, SLOT(handleSearchPoints(const std::vector<Point> &)));
	connect(qec, SIGNAL(resultSumPoints(const Point &)), this, SLOT(handleSumPoints(const Point &)));
	connect(qec, SIGNAL(resultMultPoint(const Point &)), this, SLOT(handleMultPoint(const Point &)));
	connect(qec, SIGNAL(resultIntermProtocolDH(const Point &, const Point &)), this, SLOT(handleIntermProtocolDH(const Point &, const Point &)));
	connect(qec, SIGNAL(resultProtocolDH(const Point &, const Point &)), this, SLOT(handleProtocolDH(const Point &, const Point &)));
	connect(qec, SIGNAL(resultInverseValueProtocolMO(const long long &, const long long &)), 
		this, SLOT(handleInverseValueProtocolMO(const long long &, const long long &)));
	connect(qec, SIGNAL(resultProtocolMO(const Point &, const Point &, const Point &, const Point &)), 
		this, SLOT(handleProtocolMO(const Point &, const Point &, const Point &, const Point &)));
	connect(qec, SIGNAL(resultCryptosystemEG(const Point &, const Point &, const Point &, const Point &)),
		this, SLOT(handleCryptosystemEG(const Point &, const Point &, const Point &, const Point &)));

	thread->start();
}

// ��������� ���������� ������ � ��������
void EllipticCurveWindow::settingConnectButtons() {
	// ���� ��������� ���������� ��
	connect(ui.randomPrimeButton, SIGNAL(clicked()), this, SLOT(onRandPrimeButton()));
	connect(ui.parameterAButton, SIGNAL(clicked()), this, SLOT(onRandParameterAButton()));
	connect(ui.parameterBButton, SIGNAL(clicked()), this, SLOT(onRandParameterBButton()));
	connect(ui.searchPointButton, SIGNAL(clicked()), this, SLOT(onSearchPointButton()));
	// ���� ����� ����� ��
	connect(ui.randomPointButton_1, SIGNAL(clicked()), this, SLOT(onRandPointSumButton_1()));
	connect(ui.randomPointButton_2, SIGNAL(clicked()), this, SLOT(onRandPointSumButton_2()));
	connect(ui.sumButton, SIGNAL(clicked()), this, SLOT(onSumButton()));
	// ���� ��������� ����� ��
	connect(ui.randomPointButton_3, SIGNAL(clicked()), this, SLOT(onRandPointMultButton()));
	connect(ui.randomValueButton_1, SIGNAL(clicked()), this, SLOT(onRandValueMultButton()));
	connect(ui.mulButton, SIGNAL(clicked()), this, SLOT(onMulButton()));
	// ���� ��������� �����-��������
	connect(ui.randomPointDHButton, SIGNAL(clicked()), this, SLOT(onRandPointDHButton()));
	connect(ui.randomValueADHButton, SIGNAL(clicked()), this, SLOT(onRandValueADHButton()));
	connect(ui.randomValueBDHButton, SIGNAL(clicked()), this, SLOT(onRandValueBDHButton()));
	connect(ui.protocolDHButton, SIGNAL(clicked()), this, SLOT(onProtocolDHButton()));
	// ���� ��������� �����-�����
	connect(ui.randomPointMOButton, SIGNAL(clicked()), this, SLOT(onRandPointMOButton()));
	connect(ui.randomValueEMOButton_1, SIGNAL(clicked()), this, SLOT(onRandValueEMOButton_1()));
	connect(ui.randomValueEMOButton_2, SIGNAL(clicked()), this, SLOT(onRandValueEMOButton_2()));
	connect(ui.protocolMOButton, SIGNAL(clicked()), this, SLOT(onProtocolMOButton()));
	// ���� ������������� ���-������
	connect(ui.randomPointBEGButton, SIGNAL(clicked()), this, SLOT(onRandPointBEGButton()));
	connect(ui.randomPointPEGButton, SIGNAL(clicked()), this, SLOT(onRandPointPEGButton()));
	connect(ui.randomValueAEGButton, SIGNAL(clicked()), this, SLOT(onRandValueAEGButton()));
	connect(ui.randomValueBEGButton, SIGNAL(clicked()), this, SLOT(onRandValueBEGButton()));
	connect(ui.cryptosystemEGButton, SIGNAL(clicked()), this, SLOT(onCryptosystemEGButton()));
}

// �������� ���������� ��� ������ �����
// (� �������� ���������� ����������� �������� ���������� ������ ��� �������������� � �����)
bool EllipticCurveWindow::suc�essReadPoint(const bool &okX, const bool &okY) {
	if (!(okX && okY)) {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: ����� ������ �� ���������."));
		return false;
	}
	return true;
}

// �������� ���������� ��� ������ �����
// (� �������� ��������� ����������� �������� ���������� ������ ��� �������������� � �����)
bool EllipticCurveWindow::successReadNumber(const bool &ok) {
	if (!ok) {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: ��������� ����� �� ���������."));
		return false;
	}
	return true;
}

// �������� ������������� ������ ����� �� ������ ��
bool EllipticCurveWindow::existsPoint(const Point &point) {
	if (std::find(pointsEC.begin(), pointsEC.end(), point) == pointsEC.end()) {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: ����� ����� ����."));
		return false;
	}
	return true;
}

// ������ ����� �� ��������� Label
void EllipticCurveWindow::printPoint(QLabel *&label, const Point &point) {
	if (point != PointAtInfinity)
		label->setText("R(" + QString::number(point.x) + ", " + QString::number(point.y) + ")");
	else
		label->setText(QString::fromLocal8Bit("����� �� �������������"));
}

// ������ ���� �����
void EllipticCurveWindow::printPoints() {
	QString points("");
	for (Point point : pointsEC) {
		points.push_back("(" + QString::number(point.x) + ", " + QString::number(point.y) + ")\n");
	}
	ui.pointsTextEdit->setPlainText(points);
	ui.countPointsLabel->setText(QString::fromLocal8Bit("���-��: ") + QString::number(sizePoints));
}

// ��������� ���������� ����� �� -field �� field
long long EllipticCurveWindow::randParameterEC(const long long &field) {
	return rand() % (2 * field - 1) - field + 1;
}

/* -*- ������� �� ������ -*- */

// - ��������� �� -

// ������ ��������� ���������� �������� �����
void EllipticCurveWindow::onRandPrimeButton() {
	ui.primeLineEdit->setText(QString::number(randomPrime()));
}

// ������ ��������� ���������� ����� ��� ��������� a ��
void EllipticCurveWindow::onRandParameterAButton() {
	bool ok1, ok2;
	const long long p = ui.primeLineEdit->text().toLongLong(&ok1);
	const int r = ui.degreeLineEdit->text().toInt(&ok2);
	if (ok1 && ok2) ui.parameterALineEdit->setText(QString::number(randParameterEC(pow(p, r))));
	else (new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: ���� ������ �� ���������."));
}

// ������ ��������� ���������� ����� ��� ��������� b ��
void EllipticCurveWindow::onRandParameterBButton() {
	bool ok1, ok2;
	const long long p = ui.primeLineEdit->text().toLongLong(&ok1);
	const int r = ui.degreeLineEdit->text().toInt(&ok2);
	if (ok1 && ok2) ui.parameterBLineEdit->setText(QString::number(randParameterEC(pow(p, r))));
	else (new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: ���� ������ �� ���������."));
}

// ������ ������ ���� ����� ��
void EllipticCurveWindow::onSearchPointButton() {
	// �������� ������ ����������
	bool ok;
	prime = ui.primeLineEdit->text().toLongLong(&ok);
	if (!ok) {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: ����� p ������ �� ���������."));
		return;
	}
	degree = ui.degreeLineEdit->text().toInt(&ok);
	if (!ok) {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: ������� n ������ �� ���������."));
		return;
	}
	field = pow(prime, degree);
	if (!(field > 2 && field < 0x8000)) {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: ���� ����� �� ��������� 2 < q < 32768."));
		return;
	}
	parameterA = ui.parameterALineEdit->text().toLongLong(&ok);
	if (!ok && parameterA > field) {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: �������� � ����� �� ���������."));
		return;
	}
	parameterB = ui.parameterBLineEdit->text().toLongLong(&ok);
	if (!ok && parameterB > field) {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: �������� b ����� �� ���������."));
		return;
	}
	if (primeCheck(prime)) {
		ui.fieldLabel->setText("= " + QString::number(field));
		// �������� ������������� D = 4 * �^3 + 27 * b^2 (mod field)
		if (mod(4 * pow_mod(parameterA, 3, prime) + 27 * pow_mod(parameterB, 2, prime), prime) != 0) {
			emit startSearchPoints(parameterA, parameterB, prime, degree);
		}
		// D == 0
		else {
			(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: ����������� ������."));
		}
	}
	else {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: �������� ����� �� �������� �������."));
	}
}

// - ����� ����� ��

// ������ ��������� ������ ��������� ����� �����
void EllipticCurveWindow::onRandPointSumButton_1() {
	const Point point = pointsEC[rand() % pointsEC.size()];
	ui.xSumLineEdit_1->setText(QString::number(point.x));
	ui.ySumLineEdit_1->setText(QString::number(point.y));
}

// ������ ��������� ������ ��������� ����� �����
void EllipticCurveWindow::onRandPointSumButton_2() {
	const Point point = pointsEC[rand() % pointsEC.size()];
	ui.xSumLineEdit_2->setText(QString::number(point.x));
	ui.ySumLineEdit_2->setText(QString::number(point.y));
}

// ������ ���������� ����� ����� ��
void EllipticCurveWindow::onSumButton() {
	bool okX, okY;
	// ������ ������ �����
	const Point point1 = { ui.xSumLineEdit_1->text().toLongLong(&okX), ui.ySumLineEdit_1->text().toLongLong(&okY) };
	if (!(suc�essReadPoint(okX, okY) & existsPoint(point1))) return;
	// ������ ������ �����
	const Point point2 = { ui.xSumLineEdit_2->text().toLongLong(&okX), ui.ySumLineEdit_2->text().toLongLong(&okY) };
	if (!(suc�essReadPoint(okX, okY) && existsPoint(point2))) return;
	// ���������� �����
	emit startSumPoints(point1, point2, parameterA, field);
}

// - ��������� ����� �� -

// ������ ��������� ��������� ����� ������������
void EllipticCurveWindow::onRandPointMultButton() {
	const Point point = pointsEC[rand() % pointsEC.size()];
	ui.xMulLineEdit->setText(QString::number(point.x));
	ui.yMulLineEdit->setText(QString::number(point.y));
}

// ������ ��������� ���������� ����� ������������
void EllipticCurveWindow::onRandValueMultButton() {
	ui.valueMulLineEdit->setText(QString::number(rand()));
}

// ������ ���������� ��������� �����
void EllipticCurveWindow::onMulButton() {
	bool okX, okY;
	// ������ �����
	const Point point = { ui.xMulLineEdit->text().toLongLong(&okX), ui.yMulLineEdit->text().toLongLong(&okY) };
	if (!(suc�essReadPoint(okX, okY) && existsPoint(point))) return;
	// ������ ���������
	bool ok;
	const unsigned value = ui.valueMulLineEdit->text().toLongLong(&ok);
	if (!successReadNumber(ok)) return;
	// ���������� ��������� �����
	emit startMultPoint(point, value, parameterA, field, sizePoints);
}

// - �������� �����-�������� -

// ������ ��������� ��������� ����� ��� ��������� �-�
void EllipticCurveWindow::onRandPointDHButton() {
	const Point point = pointsEC[rand() % pointsEC.size()];
	ui.xDHLineEdit->setText(QString::number(point.x));
	ui.yDHLineEdit->setText(QString::number(point.y));
}

// ������ ��������� ������� ���������� ����� ��������� �-�
void EllipticCurveWindow::onRandValueADHButton() {
	ui.valueADHLineEdit->setText(QString::number(rand()));
}

// ������ ��������� ������� ���������� ����� ��������� �-�
void EllipticCurveWindow::onRandValueBDHButton() {
	ui.valueBDHLineEdit->setText(QString::number(rand()));
}

// ������ ���������� ��������� �����-��������
void EllipticCurveWindow::onProtocolDHButton() {
	bool okX, okY, ok;
	// �������� �����
	const Point point = { ui.xDHLineEdit->text().toLongLong(&okX), ui.yDHLineEdit->text().toLongLong(&okY) };
	if (!(suc�essReadPoint(okX, okY) & existsPoint(point))) return;
	// �������� ������� �����
	const unsigned Alice = ui.valueADHLineEdit->text().toLongLong(&ok);
	if (!successReadNumber(ok)) return;
	// �������� ������� �����
	const unsigned Bob = ui.valueBDHLineEdit->text().toLongLong(&ok);
	if (!successReadNumber(ok)) return;

	// ���������� ���������
	emit startProtocolDH(point, Alice, Bob, parameterA, field, sizePoints);
}

// - �������� �����-����� -

// ���
long long gcd(const long long &a, const long long &b);

// ������ ��������� ��������� ����� ��� ��������� �-�
void EllipticCurveWindow::onRandPointMOButton() {
	const Point point = pointsEC[rand() % pointsEC.size()];
	ui.xMOLineEdit->setText(QString::number(point.x));
	ui.yMOLineEdit->setText(QString::number(point.y));
}

// ������ ��������� ������� ���������� ����� Ea ��������� �-�
void EllipticCurveWindow::onRandValueEMOButton_1() {
	long long e = 2 + rand() % (sizePoints - 2);
	while (gcd(e, sizePoints) != 1) e = 1 + rand() % (sizePoints - 1);
	ui.valueEMOLineEdit_1->setText(QString::number(e));
}

// ������ ��������� ������� ���������� ����� Eb ��������� �-�
void EllipticCurveWindow::onRandValueEMOButton_2() {
	long long e = 2 + rand() % (sizePoints - 2);
	while (gcd(e, sizePoints) != 1) e = 1 + rand() % (sizePoints - 1);
	ui.valueEMOLineEdit_2->setText(QString::number(e));
}

// ������ ���������� ��������� �����-�����
void EllipticCurveWindow::onProtocolMOButton() {
	bool okX, okY, ok;
	// �������� �����
	Point point = { ui.xMOLineEdit->text().toLongLong(&okX), ui.yMOLineEdit->text().toLongLong(&okY) };
	if (!(suc�essReadPoint(okX, okY) & existsPoint(point))) return;
	// �������� ������� �����
	const unsigned eAlice = ui.valueEMOLineEdit_1->text().toLongLong(&ok);
	if (!successReadNumber(ok)) return;
	// �������� ������� �����
	const unsigned eBob = ui.valueEMOLineEdit_2->text().toLongLong(&ok);
	if (!successReadNumber(ok)) return;
	// �������� � ������������� � ����������� �����
	if (gcd(eAlice, sizePoints) != 1 || gcd(eBob, sizePoints) != 1) {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: ����� � �� ������������� � N (���������� �����)."));
		return;
	}
	// ���������� ���������
	emit startProtocolMO(point, eAlice, eBob, parameterA, field, sizePoints);
}

// - ������������� ���-������ -

// ������ ��������� ��������� �������� ����� B ��� ������������� �-�
void EllipticCurveWindow::onRandPointBEGButton() {
	const Point point = pointsEC[rand() % pointsEC.size()];
	ui.xBEGLineEdit->setText(QString::number(point.x));
	ui.yBEGLineEdit->setText(QString::number(point.y));
}

// ������ ��������� ��������� ��������� ����� P ��� ������������� �-�
void EllipticCurveWindow::onRandPointPEGButton() {
	const Point point = pointsEC[rand() % pointsEC.size()];
	ui.xPEGLineEdit->setText(QString::number(point.x));
	ui.yPEGLineEdit->setText(QString::number(point.y));
}

// ������ ��������� ���������� ����� A ������������� �-�
void EllipticCurveWindow::onRandValueAEGButton() {
	ui.valueAEGLineEdit->setText(QString::number(rand()));
}

// ������ ��������� ���������� ����� B ������������� �-�
void EllipticCurveWindow::onRandValueBEGButton() {
	ui.valueBEGLineEdit->setText(QString::number(rand()));
}

// ������ ���������� ������������� ���-������
void EllipticCurveWindow::onCryptosystemEGButton() {
	bool okX, okY, ok;
	// �������� ��������� �����
	const Point publicPoint = { ui.xBEGLineEdit->text().toLongLong(&okX), ui.yBEGLineEdit->text().toLongLong(&okY) };
	if (!(suc�essReadPoint(okX, okY) & existsPoint(publicPoint))) return;
	// �������� ��������� �����
	const Point secretPoint = { ui.xPEGLineEdit->text().toLongLong(&okX), ui.yPEGLineEdit->text().toLongLong(&okY) };
	if (!(suc�essReadPoint(okX, okY) & existsPoint(secretPoint))) return;
	// �������� ������� �����
	const unsigned Alice = ui.valueAEGLineEdit->text().toLongLong(&ok);
	if (!successReadNumber(ok)) return;
	// �������� ������� �����
	const unsigned Bob = ui.valueBEGLineEdit->text().toLongLong(&ok);
	if (!successReadNumber(ok)) return;

	// ���������� ���������
	emit startCryptosystemEG(publicPoint, secretPoint, Alice, Bob, parameterA, field, sizePoints);
}

/* -- ��������� ����������� ���������� -- */

// ��������� ��������� ������ ����� ��
void EllipticCurveWindow::handleSearchPoints(const std::vector<Point> &result) {
	pointsEC = result;
	sizePoints = pointsEC.size() + 1;
	// �������� ��������� ������ � �������
	ui.tabWidget->setEnabled(true);
	// ���������� ������� ��
	if (ui.graphCheckBox->isChecked()) {
		QThread *threadGraph = new QThread;
		GraphEllipticCurve *graph = new GraphEllipticCurve;
		graph->moveToThread(threadGraph);
		connect(this, SIGNAL(startGraphEC(const long long &, const long long &)), graph, SLOT(printGraphEC(const long long &, const long long &)));
		connect(graph, SIGNAL(finishGraph()), threadGraph, SLOT(quit()));
		connect(threadGraph, SIGNAL(finished()), graph, SLOT(deleteLater()));
		threadGraph->start();
		emit startGraphEC(parameterA, parameterB);
	}
	printPoints();
}

// ��������� ���������� ����� �����
void EllipticCurveWindow::handleSumPoints(const Point &result) {
	printPoint(ui.resultSum, result);
}

// ��������� ���������� ��������� �����
void EllipticCurveWindow::handleMultPoint(const Point &result) {
	printPoint(ui.resultMul, result);
}

// ��������� ������������� �������� ��������� ��
void EllipticCurveWindow::handleIntermProtocolDH(const Point &pointAlice, const Point &pointBob) {
	printPoint(ui.resultABDHLabel, pointAlice);
	printPoint(ui.resultBBDHLabel, pointBob);
}

// ��������� ����������� ��������� ��
void EllipticCurveWindow::handleProtocolDH(const Point &resultPointAlice, const Point &resultPointBob) {
	printPoint(ui.resultABBDHLabel, resultPointAlice);
	printPoint(ui.resultBABDHLabel, resultPointBob);

	if (resultPointAlice == resultPointBob) {
		printPoint(ui.resultProtocolDH, resultPointAlice);
	}
	else {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: �������� �� �������."));
	}
}

// ��������� �������� �������� � � ��������� �-�
void EllipticCurveWindow::handleInverseValueProtocolMO(const long long &inverseAlice, const long long &inverseBob) {
	ui.resultDMOLabel_1->setText(QString::number(inverseAlice));
	ui.resultDMOLabel_2->setText(QString::number(inverseBob));
}

// ��������� ����������� ��������� �-�
void EllipticCurveWindow::handleProtocolMO(const Point &pointEB, const Point &pointEEB, const Point &pointDEEB, const Point &pointDDEEB) {
	printPoint(ui.resultEMOLabel, pointEB);
	printPoint(ui.resultEEMOLabel, pointEEB);
	printPoint(ui.resultDEEMOLabel, pointDEEB);
	printPoint(ui.resultDDEEMOLabel, pointDDEEB);
	printPoint(ui.resultProtocolMO, pointDDEEB);
}

// ��������� ����������� ������������� ���-������
void EllipticCurveWindow::handleCryptosystemEG(const Point &pointBob, const Point &pointKey1, const Point &pointKey2, const Point &secretPoint) {
	printPoint(ui.resultBBEGLabel, pointBob);
	printPoint(ui.resultKeyEGLabel_1, pointKey1);
	printPoint(ui.resultKeyEGLabel_2, pointKey2);
	printPoint(ui.resultCryptosystemEG, secretPoint);
}

/* ��������������� ������� */
// ���������� ����� �������� (�������� �������� �������)
long long gcd(const long long &a, const long long &b) {
	if (!a || !b) return a | b;
	long long degree = 0;
	long long m = a, n = b;
	while (!((m | n) & 1)) {
		m >>= 1;
		n >>= 1;
		++degree;
	}
	while (!(m & 1)) m >>= 1;
	while (n) {
		while (!(n & 1)) n >>= 1;
		if (m < n) n -= m;
		else {
			const long long diff = m - n;
			m = n;
			n = diff;
		}
		n >>= 1;
	}
	return m << degree;
}