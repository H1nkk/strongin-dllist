#include <iostream>
#include <cmath>
#include <algorithm>
#include <map>
#include <vector>

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
	vector<double> dots = { a,b };
	vector<double> value = { func(a), func(b) }; // value[i] = значение функции в точке i 
	vector<double> R(1);
	double M;
	double m;


	for (int iteration = 0; iteration < ITERMAX; iteration++) {
		sort(dots.begin(), dots.end());

		int dotsCount = dots.size();

		M = fabs((value[dotsCount - 1] - value[dotsCount - 2]) / (dots[dotsCount - 2] - dots[dotsCount - 1]));

		value.push_back(0);
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
		}

		double Rmax = -INFINITY;
		int minInd = -1;
		for (int i = 1; i < dots.size(); i++) {
			R[i - 1] = m * (dots[i] - dots[i - 1])
					   + (value[i] - value[i - 1]) * (value[i] - value[i - 1]) / (m * (dots[i] - dots[i - 1]))
					   - 2 * (value[i] - value[i - 1]);

			if (R[i - 1] > Rmax) {
				Rmax = R[i - 1];
				minInd = i - 1;
			}
		}
		R.push_back(0);
		double newDot = 0.5 * (dots[minInd + 1] + dots[minInd]) - (value[minInd + 1] - value[minInd]) * 0.5 / m;
		if ((dots[minInd + 1] - dots[minInd]) < E) {
			dots.push_back(newDot);
			break;
		}
		dots.push_back(newDot);
	}

	double res = dots[0], funcMin = func(dots[0]);
	for (auto dot : dots) {
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

	r = findBestR(f6);
	cout << AGP(a, b, f6);
	return 0;
}