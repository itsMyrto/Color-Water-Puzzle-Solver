#include <string>
#include <sstream>
#include <vector>
#include <ctime>
#include <iomanip>
#pragma once
using namespace std;

class Container {
private:
	char top;
	char container[4];
public:
	Container(Container& c) {
		for (int i = 0; i < 4; i++) {
			container[i] = c.container[i];
		}
		top = c.top;
	}
	
	Container(char** glasses, char* top,int i) {
		for (int j = 0; j < 4; j++) {
			container[j] = glasses[j][i];
			this->top = top[i];
		}
	}

	int getSpecificColor(int position) {
		return container[position];
	}

	int getTop() {
		return top;
	}

	void setSpecificColor(int c,int position) {
		container[position] = c;
	}

	void setTop(int t) {
		top = t;
	}

	void decreaseTop() {
		top--;
	}

	Container& getInstance() {
		return *this;
	}

	void increaseTop() {
		top++;
		return;
	}

	bool isComplete() {
		int color = container[0];
		bool isSameColor = true;
		for (int i = 1; i < 4; i++) {
			if (container[i] != color) {
				isSameColor = false;
			}
		}

		return isSameColor;
	}

	int getState(int position) {
		return container[position];
	}
};