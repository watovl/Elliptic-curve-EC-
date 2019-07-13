#include "EllipticCurveWindow.h"
#include <Windows.h>
#include <QtWidgets/QApplication>
#include <time.h>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpcmdline, int ncmdshow) {
	srand((unsigned)time(NULL));

	QApplication app(ncmdshow, (char **)lpcmdline);
	EllipticCurveWindow w;
	w.show();
	return app.exec();
}