#ifndef GRAPHELLIPTICCURVE_H
#define GRAPHELLIPTICCURVE_H

#include <QObject>
#include <string>
#include <stdio.h>
#include <stdlib.h>

class GraphEllipticCurve : public QObject {
	Q_OBJECT

public slots:
	// построение графика ЭК
	void printGraphEC(const long long &a, const long long &b) {
		FILE *gpipe = _popen("C:\\gnuplot\\bin\\gnuplot -persist", "w");
		if (!gpipe) return;
		std::string nameFunction("ti 'y^{2}=x^{3}");
		if (a > 0) nameFunction += "+" + std::to_string(a) + "*x";
		else if (a < 0) nameFunction += std::to_string(a) + "*x";
		if (b > 0) nameFunction += "+" + std::to_string(b) + "'";
		else if (b < 0) nameFunction += std::to_string(b) + "'";
		std::string command("splot (y**2-x**3-x*" + std::to_string(a) + "-" + std::to_string(b) + ") " + nameFunction + "\n");
		fprintf(gpipe, "set xzeroaxis\n"); //lt -1
		fprintf(gpipe, "set yzeroaxis\n"); //lt -1
		fprintf(gpipe, "set xrange [-100:100]\n");
		fprintf(gpipe, "set yrange [-100:100]\n");
		fprintf(gpipe, "set bar 0\n");
		fprintf(gpipe, "set terminal win\n");
		fprintf(gpipe, "set contour\n");
		fprintf(gpipe, "set cntrparam levels discrete 0\n");
		fprintf(gpipe, "set isosamples 1000, 1000\n");
		fprintf(gpipe, "unset surface\n");
		fprintf(gpipe, "set view map\n");
		fprintf(gpipe, "set title 'Elliptic curve'\n");
		fprintf(gpipe, command.c_str());
		fprintf(gpipe, "pause -1 \n");
		fprintf(gpipe, "exit\n");
		_pclose(gpipe);
		emit finishGraph();
	}

signals:
	void finishGraph();
};

#endif