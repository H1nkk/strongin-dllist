#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>

constexpr double r = 4.0;
const double E = 1e-4; // epsilon
using namespace std;

double func(double x) {
	return sin(x);
}

double AGP(double a, double b, double (*func)(double x)) {
	vector<double> dots = { a,b };
	vector<double> value = { func(a), func(b) }; // value[i] = значение функции в точке i 
	vector<double> R(1);
	double M;
	double m;


	for (int iteration = 0; iteration < 100; iteration++) {
		sort(dots.begin(), dots.end());

		M = fabs((func(b) - func(a)) / (b - a));

		value.push_back(0);
		for (int i = 0; i < dots.size(); i++) {
			value[i] = func(dots[i]);
		}

		for (int i = 2; i < dots.size(); i++) {
			M = max(M, fabs((value[i] - value[i]) / (dots[i] - dots[i - 1])));
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
			R[i - 1] = m * (dots[i] - dots[i - 1]) + (value[i] - value[i - 1]) * (value[i] - value[i - 1]) / (m * (dots[i] - dots[i - 1])) - 2 * (value[i] - value[i - 1]);
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

int main() {
	cout << AGP(1, 3, func);
	return 0;
}