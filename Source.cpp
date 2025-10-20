#include <iostream>
#include <cmath>
#include <algorithm>
#include <map>
#include <vector>
#include "dllist.h"

using namespace std;

double r = 2.0; // method parameter
const double E = 1e-4; // epsilon
const int a = 2, b = 7; // left and right bounds
const int ITERMAX = 100;

map<double (*)(double), double> extremums;

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

double AGP(double a, double b, double (*func)(double x)) {
	dllist dots({ a,b });
	int dotsCount = 2;

	dllist value ({ func(a), func(b) }); // value[i] = значение функции в точке i 

	dllist Mcandidates({ (value[dotsCount - 1] - value[dotsCount - 2]) / (dots[dotsCount - 1] - dots[dotsCount - 2]) }); // 
	double M = Mcandidates[0];
	int Mindex = 0;

	double m;

	if (M > 0) {
		m = r * M;
	}
	else {
		m = 1;
	}

	dllist R({ m * (dots[1] - dots[0]) +
			   (value[1] - value[0]) * (value[1] - value[0]) / (m * (dots[1] - dots[0])) -
			   2 * (value[1] - value[0]) });
	double Rmax = R[0];
	int Rmaxindex = 0;

	for (int iteration = 0; iteration < ITERMAX; iteration++) {
		// sort(dots.begin(), dots.end());

		//M = fabs((value[dotsCount - 1] - value[dotsCount - 2]) / (dots[dotsCount - 1] - dots[dotsCount - 2])); надо пересчитывать при добавлении новой точки

		/*value.push_back(0);
		for (int i = 0; i < dots.size(); i++) {
			value[i] = func(dots[i]);
		}

		for (int i = 1; i < dots.size(); i++) {
			M = max(M, fabs((value[i] - value[i - 1]) / (dots[i] - dots[i - 1])));
		}

		if (M > 0) {
			m = r * M;
		}
		else {
			m = 1;
		}*/ // Это всё при добавлении новой точки

		
		/*for (int i = 1; i < dots.size(); i++) {
			R[i - 1] = m * (dots[i] - dots[i - 1])
					   + (value[i] - value[i - 1]) * (value[i] - value[i - 1]) / (m * (dots[i] - dots[i - 1]))
					   - 2 * (value[i] - value[i - 1]);

			if (R[i - 1] > Rmax) {
				Rmax = R[i - 1];
				minInd = i - 1;
			}
		}*/
		double newDot = 0.5 * (dots[Rmaxindex + 1] + dots[Rmaxindex]) - (value[Rmaxindex + 1] - value[Rmaxindex]) * 0.5 / m;
		dots.insert(Rmaxindex + 1, newDot);
		value.insert(Rmaxindex + 1, func(newDot));

		if ((dots[Rmaxindex + 1] - dots[Rmaxindex]) < E) {
			break;
		}

		// TODO тут пересчет M для нового интервала:
		Mcandidates[Rmaxindex] = fabs((value[Rmaxindex + 1] - value[Rmaxindex]) / (dots[Rmaxindex + 1] - dots[Rmaxindex]));
		Mcandidates.insert(Rmaxindex + 1, fabs((value[Rmaxindex + 2] - value[Rmaxindex + 1]) / (dots[Rmaxindex + 2] - dots[Rmaxindex + 1])));
		//Mcandidates[Rmaxindex + 1] = fabs((value[Rmaxindex + 2] - value[Rmaxindex + 1]) / (dots[Rmaxindex + 2] - dots[Rmaxindex + 1]));

		// поиск наибольшего M:
		for (int i = 0; i < Mcandidates.size();i++) {
			M = max(M, Mcandidates[i]);
		}

		if (M > 0) {
			m = r * M;
		}
		else {
			m = 1;
		}

		// TODO тут пересчет R для нового интервала:
		int i = 1;
		R[Rmaxindex + i - 1] = m * (dots[i] - dots[i - 1])
		+ (value[i] - value[i - 1]) * (value[i] - value[i - 1]) / (m * (dots[i] - dots[i - 1]))
		- 2 * (value[i] - value[i - 1]);
		i = 2;
		R.insert(Rmaxindex + i - 1, m * (dots[i] - dots[i - 1])
			+ (value[i] - value[i - 1]) * (value[i] - value[i - 1]) / (m * (dots[i] - dots[i - 1]))
			- 2 * (value[i] - value[i - 1]));

		// поиск наибольшего R:
		for (int i = 1; i < dots.size(); i++) {
			if (R[i - 1] > Rmax) {
				Rmax = R[i - 1];
				Rmaxindex = i - 1;
			}
		}
		R.insert(Rmaxindex, Rmax);

		dotsCount++;
	}

	double res = dots[0], funcMin = func(dots[0]);
	for (int i = 0; i < dots.size(); i++) {
		double dot = dots[i];
		if (func(dot) < funcMin) {
			funcMin = func(dot);
			res = dot;
		}
	}

	return res;
}

double findBestR(double (*testingFuncion)(double)) {
	cout << fixed;

	double extrPoint = extremums[testingFuncion];

	double best = r, diff = fabs(AGP(a, b, testingFuncion) - extrPoint);
	for (r = 1.1; r < 100.0; r += 0.1) {
		double curdiff = fabs(AGP(a, b, testingFuncion) - extrPoint);
		if (curdiff <= diff) {
			best = r;
			diff = curdiff;
			cout << best << ' ' << diff << '\n';
			//if (curdiff == 0) break;
		}
	}

	cout << best << ' ' << diff << '\n';
	return best;
}

int main() {

	extremums[f1] = (a + b) / 2;

	// для a = 2, b = 7:
	extremums[f2] = 4.91318;
	extremums[f3] = 3.42562;
	extremums[f4] = 3.42562;
	extremums[f5] = 7;
	extremums[f6] = 6.86546;

	double (*testingFunction)(double) = f4;
	r = findBestR(testingFunction);
	cout << AGP(a, b, testingFunction);

	//vector<double> v{ 4,54,3,123123 };
	//dllist list({23423,234,234,23,42,34});
	//list.insert(0, 4);
	//list.insert(1, 5434);
	//list.insert(2, 2);
	//list.insert(3, -543);
	//list.insert(4, 4522.543);

	//for (int i=0;i<list.size();i++)
	//	cout << list.get(i)<<' ';

	//list.insert(5, 534);
	//cout << list.get(list.size() - 1);
	return 0;
}