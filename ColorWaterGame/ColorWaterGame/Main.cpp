#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <limits>
#include <ctime>
#include "Container.h"
#include "Game.h"
#include <queue>
#include <algorithm>
#include <iomanip>
#include <chrono>

using namespace chrono;
using namespace std;

int main() {

	int numberOfContainers;
	char** glasses;
	char* colors;
	char* top;
	bool filled = false;
	int sum = 0;

	srand(time(NULL));
	cout << "Enter the number of containers: " << endl;
	cin >> numberOfContainers;
	cout << endl;

	glasses = new char* [4];
	int size = numberOfContainers - 2;
	colors = new char[size];
	top = new char[numberOfContainers];

	for (int i = 0; i < 4; i++) {
		glasses[i] =  new char[numberOfContainers];
		for (int j = 0; j < numberOfContainers; j++) {
			glasses[i][j] = -1;
			top[j] = 0;
		}

	}

	for (int i = 0; i < numberOfContainers - 2; i++) {
		colors[i] = 0;
	}

	while (!filled) {
		char j = rand() % (numberOfContainers - 2);
		if (top[j] < 4) {

			char color = rand() % (numberOfContainers - 2);
			if (colors[color] < 4) {
				glasses[top[j]][j] = color;
				top[j]++;
				colors[color]++;
			}
		}

		filled = true;
		for (int i = 0; i < numberOfContainers - 2; i++) {
			if (colors[i] != 4) {
				filled = false;
			}
		}
	}

	for (int i = 0; i < numberOfContainers; i++) {
		top[i]--;
	}
		
	/*
	for (int i = 3; i >= 0; i--) {
		for (int j = 0; j < numberOfContainers; j++) {
			cout << "|" << setw(3) << glasses[i][j] << setw(3) << "| ";
		}
		cout << endl;
	}
	*/
	//cout << endl;

	Game initial(glasses, top, numberOfContainers);
	Game* initialState = &initial;
	queue<Game* > agenda;
	unordered_map <string, Game*> closed;
	agenda.push(initialState);
	int examined = 0;
	long mem = 1;
	int depth;
	bool foundSolution = false;
	string allActions = "";

	auto start = high_resolution_clock::now();
	while (agenda.size() > 0) {

		if (agenda.size() + closed.size() > mem) {
			mem = agenda.size() + closed.size();
		}
		Game* c = agenda.front();
		agenda.pop();
		if (closed.count(c->getKey()) == 0)
		{
			examined++;
			if (c->isFinal()) {
				depth = c->getDepth();
				allActions = c->getAllActions();
				foundSolution = true;
				break;
			}

			pair<string, Game*> k(c->getKey(), c);
			closed.insert(k);

			vector<Game*> children = c->expand();
			for (unsigned int i = 0; i < children.size(); i++) {
				if (closed.count(children[i]->getKey()) == 0) {
					agenda.push(children[i]);
				}
			}
		}

	}

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast <microseconds> (stop - start);

	if (foundSolution == true) {
		cout << allActions << endl;
		cout << "Problem Solved" << endl;
		cout << "Depth is: " << depth << endl;
		cout << "Situations Examined: " << examined << endl;
		cout << "Mem is: " << mem << endl;
		cout << "Time Needed: " << duration.count() << endl;
	}
	else {
		cout << "Problem unsolvable" << endl;
	}

	return 0;
}

/*
containers[0][0] = 0;
containers[1][0] = 1;
containers[2][0] = 1;
containers[3][0] = -1;
containers[0][1] = 0;
containers[1][1] = 2;
containers[2][1] = 1;
containers[3][1] = -1;
containers[0][2] = 2;
containers[1][2] = 0;
containers[2][2] = 2;
containers[3][2] = -1;
containers[0][3] = 0;
containers[1][3] = 1;
containers[2][3] = 2;
containers[3][3] = -1;
containers[0][4] = -1;
containers[1][4] = -1;
containers[2][4] = -1;
containers[3][4] = -1;
top[0] = 2;
top[1] = 2;
top[2] = 2;
top[3] = 2;
top[4] = -1;
*/

/*
containers[0][0] = 0; //red
containers[1][0] = 1; //cherry
containers[2][0] = 2; //yellow
containers[3][0] = 2; //yellow
containers[0][1] = 1; //cherry
containers[1][1] = 3; //green
containers[2][1] = 3; //green
containers[3][1] = 4; //white
containers[0][2] = 3; //green
containers[1][2] = 3; //green
containers[2][2] = 0; //red
containers[3][2] = 2; //yellow
containers[0][3] = 4; //white
containers[1][3] = 0; //red
containers[2][3] = 0; //red
containers[3][3] = 4; //white
containers[0][4] = 1; //cherry
containers[2][4] = 2; //yellow
containers[3][4] = 1; //cherry
containers[0][5] = -1;
containers[1][5] = -1;
containers[2][5] = -1;
containers[3][5] = -1;
containers[0][6] = -1;
containers[1][6] = -1;
containers[2][6] = -1;
containers[3][6] = -1;

top[0] = 3;
top[1] = 3;
top[2] = 3;
top[3] = 3;
top[4] = 3;
top[5] = -1;
top[6] = -1;
*/