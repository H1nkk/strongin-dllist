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
const int ITERMAX = 500;

map<double (*)(double), double> extremums;

struct info {
	double extremumArg; // �������� ����� ����������
	double extremumVal; // �������� ������� � ����� ����������
	int iterCount; // ����� ����������� ��������
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

	dllist value ({ func(a), func(b) }); // value[i] = �������� ������� � ����� i 

	double foo = (dots[dotsCount - 1] - dots[dotsCount - 2]); // UDALIST

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
	double firstR = m * (dots[1] - dots[0]);
	firstR += (value[1] - value[0]) * (value[1] - value[0]) / (m * (dots[1] - dots[0]));
	firstR -= 2 * (value[1] - value[0]);

	vector<double> R = { firstR };
	double Rmax = R[0];
	int Rmaxindex = 0;
	double prevm = m;

	int iteration;
	for (iteration = 0; iteration < ITERMAX; iteration++) {
		
		double rdot = dots[Rmaxindex + 1]; // ������ ������� ��������������� ���������
		double ldot = dots[Rmaxindex]; // ����� ������� ��������������� ���������
		double newDot = 0.5 * (rdot + ldot) - (value[Rmaxindex + 1] - value[Rmaxindex]) * 0.5 / m;
		dots.insert(Rmaxindex + 1, newDot);
		value.insert(Rmaxindex + 1, func(newDot));

		if ((dots[Rmaxindex + 1] - dots[Rmaxindex]) < E || (dots[Rmaxindex + 2] - dots[Rmaxindex + 1]) < E) {
			break;
		}

		// �������� M ��� ������ ���������:
		Mcandidates[Rmaxindex] = fabs((value[Rmaxindex + 1] - value[Rmaxindex]) / (dots[Rmaxindex + 1] - dots[Rmaxindex]));
		Mcandidates.insert(Rmaxindex + 1, fabs((value[Rmaxindex + 2] - value[Rmaxindex + 1]) / (dots[Rmaxindex + 2] - dots[Rmaxindex + 1])));
		
		// ����� ����������� M:
		M = max({ M, Mcandidates[Rmaxindex], Mcandidates[Rmaxindex + 1] });
		//for (int i = 0; i < Mcandidates.size();i++) {
		//	M = max(M, Mcandidates[i]);
		//}

		if (M > 0) {
			m = r * M;
		}
		else {
			m = 1;
		}

		// �������� R ��� ���� ����������, �.�. �������� m:
		R.push_back(0);
		Rmax = R[0];
		Rmaxindex = 0;
		if (prevm != m) {
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
		else { // ����� ������������� ������ ��� ������ ���������, �� ���� ���
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
		prevm = m;
		//R.insert(Rmaxindex, Rmax);

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
			//if (curdiff == 0) break;
		}
	}

	cout << best << ' ' << diff << '\n';
	return best;
}

int main() {

	extremums[f1] = (a + b) / 2;

	// ��� a = 2, b = 7:
	extremums[f2] = 4.91318;
	extremums[f3] = 3.42562;
	extremums[f4] = 3.42562;
	extremums[f5] = 7;
	extremums[f6] = 6.86546;

	double (*testingFunction)(double) = f2;
	r = 2.0;
	cout << AGP(a, b, testingFunction).extremumArg << '\n';

	dllist list;
	for (int i = 0; i < 10; i++) {
		list.insert(i, i + 3);
	}

	list.insert(4, 543);


	for (int i = 0; i < 10; i++) {
		cout << list[i] << '\n';
	}

	return 0;
}