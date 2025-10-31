#include <iostream>
#include <cmath>
#include <algorithm>
#include <map>
#include <vector>
#include <chrono>
#include "dllist.h"

using namespace std;

double r = 2.0; // method parameter
const double E = 1e-4; // epsilon
const int a = 2, b = 7; // left and right bounds
const int ITERMAX = 500;

map<double (*)(double), double> extremums;

struct info {
	double extremumArg; // значение точки экстремума
	double extremumVal; // значение функции в точке экстремума
	int iterCount; // число совершенных итераций
	info(double extremumArg, double  extremumVal, int iterCount) : extremumArg(extremumArg), extremumVal(extremumVal), iterCount(iterCount) {

	}

};

double f1(double x) {
	if (x < (a + b) / 2) return 7 - x;
	double intersec = 7 - (a + b) / 2;
	return intersec - (a + b) / 2 + x;
}

double f2(double x) {
	return x*sin(x);
}

double f3(double x) {
	return x * cos(x);
}

double f4(double x) {
	return 354318.0 * x * cos(x);
}

double f5(double x) {
	return 3 * x * cos(4 * x);
}

double f6(double x) {
	return x * cos(x * x);
}

double f7(double x) {
	return -x - a;
}

info AGP(double a, double b, double (*func)(double x)) {
	dllist dots({ a,b });
	int dotsCount = 2;

	dllist value ({ func(a), func(b) }); // value[i] = значение функции в точке i 

	dllist Mcandidates({ (value[dotsCount - 1] - value[dotsCount - 2]) / (dots[dotsCount - 1] - dots[dotsCount - 2]) });
	double M = Mcandidates[0];
	int Mindex = 0;

	double m;

	if (M > 0) {
		m = r * M;
	}
	else {
		m = 1;
	}
	double firstR = m * (dots[1] - dots[0]);
	firstR += (value[1] - value[0]) * (value[1] - value[0]) / (m * (dots[1] - dots[0]));
	firstR -= 2 * (value[1] - value[0]);

	dllist R;
	R.insert(0, firstR);
	double Rmax = R[0];
	int Rmaxindex = 0;
	double prevm = m;

	int iteration;
	for (iteration = 0; iteration < ITERMAX; iteration++) {
		
		double rdot = dots[Rmaxindex + 1]; // правая граница подразбиваемого интервала
		double ldot = dots[Rmaxindex]; // левая граница подразбиваемого интервала
		double newDot = 0.5 * (rdot + ldot) - (value[Rmaxindex + 1] - value[Rmaxindex]) * 0.5 / m;
		dots.insert(Rmaxindex + 1, newDot);
		value.insert(Rmaxindex + 1, func(newDot));

		if ((dots[Rmaxindex + 1] - dots[Rmaxindex]) < E || (dots[Rmaxindex + 2] - dots[Rmaxindex + 1]) < E) {
			break;
		}

		// пересчет M для нового интервала:
		Mcandidates[Rmaxindex] = fabs((value[Rmaxindex + 1] - value[Rmaxindex]) / (dots[Rmaxindex + 1] - dots[Rmaxindex]));
		Mcandidates.insert(Rmaxindex + 1, fabs((value[Rmaxindex + 2] - value[Rmaxindex + 1]) / (dots[Rmaxindex + 2] - dots[Rmaxindex + 1])));
		
		// поиск наибольшего M:
		M = max({ M, Mcandidates[Rmaxindex], Mcandidates[Rmaxindex + 1] });

		if (M > 0) {
			m = r * M;
		}
		else {
			m = 1;
		}

		if (prevm != m) { // пересчет R для всех интервалов, т.к. меняется m:
			R.insert(0, 0);
			Rmax = R[0];
			Rmaxindex = 0;

			for (int i = 1; i < dots.size(); i++) {
				R[i - 1] = m * (dots[i] - dots[i - 1])
					+ (value[i] - value[i - 1]) * (value[i] - value[i - 1]) / (m * (dots[i] - dots[i - 1]))
					- 2 * (value[i] - value[i - 1]);
				if (R[i - 1] > Rmax) {
					Rmax = R[i - 1];
					Rmaxindex = i - 1;
				}
			}
		}
		else { // пересчитываем только для нового интервала:
			int i = Rmaxindex + 1;
			R[i - 1] = m * (dots[i] - dots[i - 1])
				+ (value[i] - value[i - 1]) * (value[i] - value[i - 1]) / (m * (dots[i] - dots[i - 1]))
				- 2 * (value[i] - value[i - 1]);
			i++;
			R.insert(i - 1, m * (dots[i] - dots[i - 1])
				+ (value[i] - value[i - 1]) * (value[i] - value[i - 1]) / (m * (dots[i] - dots[i - 1]))
				- 2 * (value[i] - value[i - 1]));

			Rmax = R[0];
			Rmaxindex = 0;

			for (int i = 0; i < R.size(); i++) {
				if (R[i] > Rmax) {
					Rmax = R[i];
					Rmaxindex = i;
				}
			}
		}
		prevm = m;
		dotsCount++;
	}

	double resArg = dots[0], funcMin = func(dots[0]);
	for (int i = 0; i < dots.size(); i++) {
		double dot = dots[i];
		if (func(dot) < funcMin) {
			funcMin = func(dot);
			resArg = dot;
		}
	}

	info res = { resArg, funcMin, iteration };
	return res;
}

double findBestR(double (*testingFuncion)(double)) {
	cout << fixed;

	double extrPoint = extremums[testingFuncion];

	double best = r, diff = fabs(AGP(a, b, testingFuncion).extremumArg - extrPoint);
	for (r = 1.1; r < 100.0; r += 0.1) {
		double curdiff = fabs(AGP(a, b, testingFuncion).extremumArg - extrPoint);
		if (curdiff <= diff) {
			best = r;
			diff = curdiff;
			cout << best << ' ' << diff << '\n';
		}
	}

	cout << best << ' ' << diff << '\n';
	return best;
}

int main() { // реализация на dllist эффективнее при ITERCOUNT <= ~5000, потом время ухудшается из-за выделения и очищения памяти в куче

	extremums[f1] = (a + b) / 2;

	// для a = 2, b = 7:
	extremums[f2] = 4.91318;
	extremums[f3] = 3.42562;
	extremums[f4] = 3.42562;
	extremums[f5] = 7;
	extremums[f6] = 6.86546;

	double (*testingFunction)(double) = f2;
	r = 2.0;
	info res{ 0,0,0 };
	double avgTimeSpent = 0;
	int iters = 100; // почему с увеличением iters среднее время выполнения уменьшается? прогрев?
	double foo = 4;

	for (int i = 0; i < iters; i++) { // усредняем время, т.к. оно колеблется - думаю, что из-за частого выделения памяти
		auto start = chrono::high_resolution_clock::now();
		res = AGP(a, b, testingFunction);
		auto stop = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
		double timeSpent = duration.count() / 1000000.0;
		avgTimeSpent += timeSpent;
		foo += res.extremumArg;
	}
	avgTimeSpent /= iters;

	cout << "Extremum argument: " << res.extremumArg << '\n';
	cout << "Iteration count: " << res.iterCount << '\n';
	cout << "Extremum value: " << res.extremumVal << '\n';
	cout << "Calculated in " << avgTimeSpent << " seconds\n";
	cout << "\nfoo for release:" << foo;
	cout << flush;
	return 0;
}