#include "EllipticCurveWindow.h"
#include <QtWidgets/QApplication>
#include <time.h>

int main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));

	QApplication a(argc, argv);
	EllipticCurveWindow w;
	w.show();
	return a.exec();
}
