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

// настройка параллельных потоков
void EllipticCurveWindow::settingThread() {
	thread = new QThread;
	QEllipticCurve *qec = new QEllipticCurve;
	qec->moveToThread(thread);
	// связи начала вычислений
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
	// связи обработки результатов вычислений
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

// настройка соединения кнопок и сигналов
void EllipticCurveWindow::settingConnectButtons() {
	// блок генерации параметров ЭК
	connect(ui.randomPrimeButton, SIGNAL(clicked()), this, SLOT(onRandPrimeButton()));
	connect(ui.parameterAButton, SIGNAL(clicked()), this, SLOT(onRandParameterAButton()));
	connect(ui.parameterBButton, SIGNAL(clicked()), this, SLOT(onRandParameterBButton()));
	connect(ui.searchPointButton, SIGNAL(clicked()), this, SLOT(onSearchPointButton()));
	// блок суммы точек ЭК
	connect(ui.randomPointButton_1, SIGNAL(clicked()), this, SLOT(onRandPointSumButton_1()));
	connect(ui.randomPointButton_2, SIGNAL(clicked()), this, SLOT(onRandPointSumButton_2()));
	connect(ui.sumButton, SIGNAL(clicked()), this, SLOT(onSumButton()));
	// блок кратности точек ЭК
	connect(ui.randomPointButton_3, SIGNAL(clicked()), this, SLOT(onRandPointMultButton()));
	connect(ui.randomValueButton_1, SIGNAL(clicked()), this, SLOT(onRandValueMultButton()));
	connect(ui.mulButton, SIGNAL(clicked()), this, SLOT(onMulButton()));
	// блок протокола Диффи-Хеллмана
	connect(ui.randomPointDHButton, SIGNAL(clicked()), this, SLOT(onRandPointDHButton()));
	connect(ui.randomValueADHButton, SIGNAL(clicked()), this, SLOT(onRandValueADHButton()));
	connect(ui.randomValueBDHButton, SIGNAL(clicked()), this, SLOT(onRandValueBDHButton()));
	connect(ui.protocolDHButton, SIGNAL(clicked()), this, SLOT(onProtocolDHButton()));
	// блок протокола Мэсси-Омуры
	connect(ui.randomPointMOButton, SIGNAL(clicked()), this, SLOT(onRandPointMOButton()));
	connect(ui.randomValueEMOButton_1, SIGNAL(clicked()), this, SLOT(onRandValueEMOButton_1()));
	connect(ui.randomValueEMOButton_2, SIGNAL(clicked()), this, SLOT(onRandValueEMOButton_2()));
	connect(ui.protocolMOButton, SIGNAL(clicked()), this, SLOT(onProtocolMOButton()));
	// блок криптосистемы Эль-Гамаля
	connect(ui.randomPointBEGButton, SIGNAL(clicked()), this, SLOT(onRandPointBEGButton()));
	connect(ui.randomPointPEGButton, SIGNAL(clicked()), this, SLOT(onRandPointPEGButton()));
	connect(ui.randomValueAEGButton, SIGNAL(clicked()), this, SLOT(onRandValueAEGButton()));
	connect(ui.randomValueBEGButton, SIGNAL(clicked()), this, SLOT(onRandValueBEGButton()));
	connect(ui.cryptosystemEGButton, SIGNAL(clicked()), this, SLOT(onCryptosystemEGButton()));
}

// проверка успешности при чтении точки
// (в качестве параметров принимается проверка успешности чтения при преобразовании в число)
bool EllipticCurveWindow::sucсessReadPoint(const bool &okX, const bool &okY) {
	if (!(okX && okY)) {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: точка введёна не корректно."));
		return false;
	}
	return true;
}

// проверка успешности при чтении числа
// (в качестве параметра принимается проверка успешности чтения при преобразовании в число)
bool EllipticCurveWindow::successReadNumber(const bool &ok) {
	if (!ok) {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: множитель введён не корректно."));
		return false;
	}
	return true;
}

// проверка существования данной точки на данной ЭК
bool EllipticCurveWindow::existsPoint(const Point &point) {
	if (std::find(pointsEC.begin(), pointsEC.end(), point) == pointsEC.end()) {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: такой точки нету."));
		return false;
	}
	return true;
}

// печать точки на указанный Label
void EllipticCurveWindow::printPoint(QLabel *&label, const Point &point) {
	if (point != PointAtInfinity)
		label->setText("R(" + QString::number(point.x) + ", " + QString::number(point.y) + ")");
	else
		label->setText(QString::fromLocal8Bit("Точка на бесконечности"));
}

// печать всех точек
void EllipticCurveWindow::printPoints() {
	QString points("");
	for (Point point : pointsEC) {
		points.push_back("(" + QString::number(point.x) + ", " + QString::number(point.y) + ")\n");
	}
	ui.pointsTextEdit->setPlainText(points);
	ui.countPointsLabel->setText(QString::fromLocal8Bit("Кол-во: ") + QString::number(sizePoints));
}

// генерация случайного числа от -field до field
long long EllipticCurveWindow::randParameterEC(const long long &field) {
	return rand() % (2 * field - 1) - field + 1;
}

/* -*- НАЖАТИЕ НА КНОПКИ -*- */

// - ПАРАМЕТРЫ ЭК -

// кнопка генерации случайного простого числа
void EllipticCurveWindow::onRandPrimeButton() {
	ui.primeLineEdit->setText(QString::number(randomPrime()));
}

// кнопка генерация случайного числа для параметра a ЭК
void EllipticCurveWindow::onRandParameterAButton() {
	bool ok1, ok2;
	const long long p = ui.primeLineEdit->text().toLongLong(&ok1);
	const int r = ui.degreeLineEdit->text().toInt(&ok2);
	if (ok1 && ok2) ui.parameterALineEdit->setText(QString::number(randParameterEC(pow(p, r))));
	else (new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: поле введёно не корректно."));
}

// кнопка генерация случайного числа для параметра b ЭК
void EllipticCurveWindow::onRandParameterBButton() {
	bool ok1, ok2;
	const long long p = ui.primeLineEdit->text().toLongLong(&ok1);
	const int r = ui.degreeLineEdit->text().toInt(&ok2);
	if (ok1 && ok2) ui.parameterBLineEdit->setText(QString::number(randParameterEC(pow(p, r))));
	else (new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: поле введёно не корректно."));
}

// кнопка поиска всех точек ЭК
void EllipticCurveWindow::onSearchPointButton() {
	// проверка чтения параметров
	bool ok;
	prime = ui.primeLineEdit->text().toLongLong(&ok);
	if (!ok) {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: число p введёно не корректно."));
		return;
	}
	degree = ui.degreeLineEdit->text().toInt(&ok);
	if (!ok) {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: степень n введёна не корректно."));
		return;
	}
	field = pow(prime, degree);
	if (!(field > 2 && field < 0x8000)) {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: поле вышло из диапазона 2 < q < 32768."));
		return;
	}
	parameterA = ui.parameterALineEdit->text().toLongLong(&ok);
	if (!ok && parameterA > field) {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: параметр а введён не корректно."));
		return;
	}
	parameterB = ui.parameterBLineEdit->text().toLongLong(&ok);
	if (!ok && parameterB > field) {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: параметр b введён не корректно."));
		return;
	}
	if (primeCheck(prime)) {
		ui.fieldLabel->setText("= " + QString::number(field));
		// проверка дискриминанта D = 4 * а^3 + 27 * b^2 (mod field)
		if (mod(4 * pow_mod(parameterA, 3, prime) + 27 * pow_mod(parameterB, 2, prime), prime) != 0) {
			emit startSearchPoints(parameterA, parameterB, prime, degree);
		}
		// D == 0
		else {
			(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: сингулярная кривая."));
		}
	}
	else {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: введённое число не является простым."));
	}
}

// - СУММА ТОЧЕК ЭК

// кнопка генерации первой случайной точки суммы
void EllipticCurveWindow::onRandPointSumButton_1() {
	const Point point = pointsEC[rand() % pointsEC.size()];
	ui.xSumLineEdit_1->setText(QString::number(point.x));
	ui.ySumLineEdit_1->setText(QString::number(point.y));
}

// кнопка генерации второй случайной точки суммы
void EllipticCurveWindow::onRandPointSumButton_2() {
	const Point point = pointsEC[rand() % pointsEC.size()];
	ui.xSumLineEdit_2->setText(QString::number(point.x));
	ui.ySumLineEdit_2->setText(QString::number(point.y));
}

// кнопка вычисления суммы точек ЭК
void EllipticCurveWindow::onSumButton() {
	bool okX, okY;
	// чтение первой точки
	const Point point1 = { ui.xSumLineEdit_1->text().toLongLong(&okX), ui.ySumLineEdit_1->text().toLongLong(&okY) };
	if (!(sucсessReadPoint(okX, okY) & existsPoint(point1))) return;
	// чтение второй точки
	const Point point2 = { ui.xSumLineEdit_2->text().toLongLong(&okX), ui.ySumLineEdit_2->text().toLongLong(&okY) };
	if (!(sucсessReadPoint(okX, okY) && existsPoint(point2))) return;
	// вычисление суммы
	emit startSumPoints(point1, point2, parameterA, field);
}

// - КРАТНОСТЬ ТОЧКИ ЭК -

// кнопка генерации случайной точки произведения
void EllipticCurveWindow::onRandPointMultButton() {
	const Point point = pointsEC[rand() % pointsEC.size()];
	ui.xMulLineEdit->setText(QString::number(point.x));
	ui.yMulLineEdit->setText(QString::number(point.y));
}

// кнопка генерации случайного числа произведения
void EllipticCurveWindow::onRandValueMultButton() {
	ui.valueMulLineEdit->setText(QString::number(rand()));
}

// кнопка вычисления кратности точки
void EllipticCurveWindow::onMulButton() {
	bool okX, okY;
	// чтение точки
	const Point point = { ui.xMulLineEdit->text().toLongLong(&okX), ui.yMulLineEdit->text().toLongLong(&okY) };
	if (!(sucсessReadPoint(okX, okY) && existsPoint(point))) return;
	// чтение множителя
	bool ok;
	const unsigned value = ui.valueMulLineEdit->text().toLongLong(&ok);
	if (!successReadNumber(ok)) return;
	// вычисление кратности точки
	emit startMultPoint(point, value, parameterA, field, sizePoints);
}

// - ПРОТОКОЛ ДИФФИ-ХЕЛЛМАНА -

// кнопка генерации случайной точки для протокола Д-Х
void EllipticCurveWindow::onRandPointDHButton() {
	const Point point = pointsEC[rand() % pointsEC.size()];
	ui.xDHLineEdit->setText(QString::number(point.x));
	ui.yDHLineEdit->setText(QString::number(point.y));
}

// кнопка генерации первого случайного числа протокола Д-Х
void EllipticCurveWindow::onRandValueADHButton() {
	ui.valueADHLineEdit->setText(QString::number(rand()));
}

// кнопка генерации второго случайного числа протокола Д-Х
void EllipticCurveWindow::onRandValueBDHButton() {
	ui.valueBDHLineEdit->setText(QString::number(rand()));
}

// кнопка вычисления протокола Диффи-Хеллмана
void EllipticCurveWindow::onProtocolDHButton() {
	bool okX, okY, ok;
	// проверка точки
	const Point point = { ui.xDHLineEdit->text().toLongLong(&okX), ui.yDHLineEdit->text().toLongLong(&okY) };
	if (!(sucсessReadPoint(okX, okY) & existsPoint(point))) return;
	// проверка первого числа
	const unsigned Alice = ui.valueADHLineEdit->text().toLongLong(&ok);
	if (!successReadNumber(ok)) return;
	// проверка второго числа
	const unsigned Bob = ui.valueBDHLineEdit->text().toLongLong(&ok);
	if (!successReadNumber(ok)) return;

	// вычисление протокола
	emit startProtocolDH(point, Alice, Bob, parameterA, field, sizePoints);
}

// - ПРОТОКОЛ МЭССИ-ОМУРЫ -

// НОД
long long gcd(const long long &a, const long long &b);

// кнопка генерации случайной точки для протокола М-О
void EllipticCurveWindow::onRandPointMOButton() {
	const Point point = pointsEC[rand() % pointsEC.size()];
	ui.xMOLineEdit->setText(QString::number(point.x));
	ui.yMOLineEdit->setText(QString::number(point.y));
}

// кнопка генерации первого случайного числа Ea протокола М-О
void EllipticCurveWindow::onRandValueEMOButton_1() {
	long long e = 2 + rand() % (sizePoints - 2);
	while (gcd(e, sizePoints) != 1) e = 1 + rand() % (sizePoints - 1);
	ui.valueEMOLineEdit_1->setText(QString::number(e));
}

// кнопка генерации второго случайного числа Eb протокола М-О
void EllipticCurveWindow::onRandValueEMOButton_2() {
	long long e = 2 + rand() % (sizePoints - 2);
	while (gcd(e, sizePoints) != 1) e = 1 + rand() % (sizePoints - 1);
	ui.valueEMOLineEdit_2->setText(QString::number(e));
}

// кнопка вычисления протокола Мэсси-Омуры
void EllipticCurveWindow::onProtocolMOButton() {
	bool okX, okY, ok;
	// проверка точки
	Point point = { ui.xMOLineEdit->text().toLongLong(&okX), ui.yMOLineEdit->text().toLongLong(&okY) };
	if (!(sucсessReadPoint(okX, okY) & existsPoint(point))) return;
	// проверка первого числа
	const unsigned eAlice = ui.valueEMOLineEdit_1->text().toLongLong(&ok);
	if (!successReadNumber(ok)) return;
	// проверка второго числа
	const unsigned eBob = ui.valueEMOLineEdit_2->text().toLongLong(&ok);
	if (!successReadNumber(ok)) return;
	// проверка Е взаимопростое с количеством точек
	if (gcd(eAlice, sizePoints) != 1 || gcd(eBob, sizePoints) != 1) {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: Число е не взаимопростое с N (количество точек)."));
		return;
	}
	// вычисление протокола
	emit startProtocolMO(point, eAlice, eBob, parameterA, field, sizePoints);
}

// - КРИПТОСИСТЕМА ЭЛЬ-ГАМАЛЯ -

// кнопка генерации случайной открытой точки B для криптосистимы Э-Г
void EllipticCurveWindow::onRandPointBEGButton() {
	const Point point = pointsEC[rand() % pointsEC.size()];
	ui.xBEGLineEdit->setText(QString::number(point.x));
	ui.yBEGLineEdit->setText(QString::number(point.y));
}

// кнопка генерации случайной секретной точки P для криптосистимы Э-Г
void EllipticCurveWindow::onRandPointPEGButton() {
	const Point point = pointsEC[rand() % pointsEC.size()];
	ui.xPEGLineEdit->setText(QString::number(point.x));
	ui.yPEGLineEdit->setText(QString::number(point.y));
}

// кнопка генерации случайного числа A криптосистимы Э-Г
void EllipticCurveWindow::onRandValueAEGButton() {
	ui.valueAEGLineEdit->setText(QString::number(rand()));
}

// кнопка генерации случайного числа B криптосистимы Э-Г
void EllipticCurveWindow::onRandValueBEGButton() {
	ui.valueBEGLineEdit->setText(QString::number(rand()));
}

// кнопка вычисления криптосистемы Эль-Гамаля
void EllipticCurveWindow::onCryptosystemEGButton() {
	bool okX, okY, ok;
	// проверка публичной точки
	const Point publicPoint = { ui.xBEGLineEdit->text().toLongLong(&okX), ui.yBEGLineEdit->text().toLongLong(&okY) };
	if (!(sucсessReadPoint(okX, okY) & existsPoint(publicPoint))) return;
	// проверка секретной точки
	const Point secretPoint = { ui.xPEGLineEdit->text().toLongLong(&okX), ui.yPEGLineEdit->text().toLongLong(&okY) };
	if (!(sucсessReadPoint(okX, okY) & existsPoint(secretPoint))) return;
	// проверка первого числа
	const unsigned Alice = ui.valueAEGLineEdit->text().toLongLong(&ok);
	if (!successReadNumber(ok)) return;
	// проверка второго числа
	const unsigned Bob = ui.valueBEGLineEdit->text().toLongLong(&ok);
	if (!successReadNumber(ok)) return;

	// вычисление протокола
	emit startCryptosystemEG(publicPoint, secretPoint, Alice, Bob, parameterA, field, sizePoints);
}

/* -- ОБРАБОТКА РЕЗУЛЬТАТОВ ВЫЧИСЛЕНИЙ -- */

// обработка резльтата поиска точек ЭК
void EllipticCurveWindow::handleSearchPoints(const std::vector<Point> &result) {
	pointsEC = result;
	sizePoints = pointsEC.size() + 1;
	// включаем интерфейс работы с точками
	ui.tabWidget->setEnabled(true);
	// построение графика ЭК
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

// обработка результата суммы точек
void EllipticCurveWindow::handleSumPoints(const Point &result) {
	printPoint(ui.resultSum, result);
}

// обработка результата кратности точки
void EllipticCurveWindow::handleMultPoint(const Point &result) {
	printPoint(ui.resultMul, result);
}

// обработка промежуточных значений протокола ДХ
void EllipticCurveWindow::handleIntermProtocolDH(const Point &pointAlice, const Point &pointBob) {
	printPoint(ui.resultABDHLabel, pointAlice);
	printPoint(ui.resultBBDHLabel, pointBob);
}

// обработка результатов протокола ДХ
void EllipticCurveWindow::handleProtocolDH(const Point &resultPointAlice, const Point &resultPointBob) {
	printPoint(ui.resultABBDHLabel, resultPointAlice);
	printPoint(ui.resultBABDHLabel, resultPointBob);

	if (resultPointAlice == resultPointBob) {
		printPoint(ui.resultProtocolDH, resultPointAlice);
	}
	else {
		(new QErrorMessage(this))->showMessage(QString::fromLocal8Bit("Error: значения не совпали."));
	}
}

// обработка обратных значений к Е протокола М-О
void EllipticCurveWindow::handleInverseValueProtocolMO(const long long &inverseAlice, const long long &inverseBob) {
	ui.resultDMOLabel_1->setText(QString::number(inverseAlice));
	ui.resultDMOLabel_2->setText(QString::number(inverseBob));
}

// обработка результатов протокола М-О
void EllipticCurveWindow::handleProtocolMO(const Point &pointEB, const Point &pointEEB, const Point &pointDEEB, const Point &pointDDEEB) {
	printPoint(ui.resultEMOLabel, pointEB);
	printPoint(ui.resultEEMOLabel, pointEEB);
	printPoint(ui.resultDEEMOLabel, pointDEEB);
	printPoint(ui.resultDDEEMOLabel, pointDDEEB);
	printPoint(ui.resultProtocolMO, pointDDEEB);
}

// обработка результатов криптосистемы Эль-Гамаля
void EllipticCurveWindow::handleCryptosystemEG(const Point &pointBob, const Point &pointKey1, const Point &pointKey2, const Point &secretPoint) {
	printPoint(ui.resultBBEGLabel, pointBob);
	printPoint(ui.resultKeyEGLabel_1, pointKey1);
	printPoint(ui.resultKeyEGLabel_2, pointKey2);
	printPoint(ui.resultCryptosystemEG, secretPoint);
}

/* Вспомогательная функция */
// наибольший общий делитель (бинарный алгоритм Евклида)
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