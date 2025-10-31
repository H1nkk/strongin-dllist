#pragma once
#include <vector>

using namespace std;

class dllist {
	struct Node
	{
		double x;
		Node* prev;
		Node* next;
		Node() {
			x = 0.0;
			prev = nullptr;
			next = nullptr;
		}
		Node(double x, Node* prev, Node* next) : x(x), prev(prev), next(next) {}
	};
	Node* lastUsed; // �������� ��������� �������������� node ��� ������������ ������� �� ������
	int lastUsedInd; // ������ ��������� �������������� node
	Node* start;
	int sz;
public:
	dllist() : sz(0) {
		start = nullptr;
		lastUsed = nullptr;
		lastUsedInd = -1;
	}
	dllist(const vector<double>& v): sz(0) {
		start = nullptr;
		for (int i = v.size() - 1; i >= 0;i--) {
			insert(0, v[i]);
		}
	}
	void insert(int index, double x) { // index �� ������� ���������
		if (index > sz) throw "dllist index out of range";
		sz++;
		if (index == 0) {
			if (start == nullptr) {
				start = new Node(x, nullptr, start);
				return;
			}
			Node* tmp = new Node(x, nullptr, start);
			start->prev = tmp;
			start = tmp;
			return;
		}
		lastUsed = nullptr; // ����� ���-�� ������������ ����� lastused, ���� �� ����
		Node* cur = start;
		while (--index) {
			cur = cur->next;
		}
		Node* tmp = cur->next;
		cur->next = new Node(x, cur, tmp);
	}
	double& get(int index) {
		lastUsed = nullptr; // TODO ������ ###########################################################
		int tmpIndex = index;
		Node* cur = start;
		if (lastUsed) {
			if (index >= lastUsedInd) {
				cur = lastUsed;
				int tmp = index;
				index -= lastUsedInd;
			}
			else { 
				int dist = lastUsedInd - index; // ���������� �� lastused �� ������� node
				if (dist < index) { // ���� ���������� �� lastused ������ ��� �� start, �� ���� �����, ������� � lastUsed
					cur = lastUsed;
					while (dist--) {
						cur = cur->prev;
					}
					lastUsed = cur;
					lastUsedInd = tmpIndex;
					return cur->x;
				}
			}
		}

		while (index--) {
			cur = cur->next;
		}
		lastUsed = cur;
		lastUsedInd = tmpIndex;
		return cur->x;
	}
	void change(int index, double x) {
		Node* cur = start;
		while (index--) {
			cur = cur->next;
		}
		cur->x = x;
	}
	int size() {
		return sz;
	}
	double& operator[](int index) {
		return get(index);
	}
};