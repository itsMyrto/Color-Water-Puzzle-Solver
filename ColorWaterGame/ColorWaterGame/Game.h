#include "Container.h"
#pragma once
class Game {
	
private:
	Container** containers;
	Game* previous;
	string actionName;
	int numberOfContainers;
public:
	Game(char** glasses,char* top,int numberOfContainers) {

		this->numberOfContainers = numberOfContainers;
		containers = new Container * [numberOfContainers];
		for (int i = 0; i < numberOfContainers; i++) {
			containers[i] = new Container(glasses, top, i);
		}
		setPrevious(nullptr);
		setActionName("");
	}

	~Game() {
		for (int i = 0; i < numberOfContainers; i++) {
			delete[] containers[i];
		}

		delete[] containers;
	}

	Game(Game& g) {
		this->numberOfContainers = g.numberOfContainers;
		containers = new Container * [numberOfContainers];
		for (int i = 0; i < numberOfContainers; i++) {
			containers[i] = new Container(g.containers[i]->getInstance());
		}
		previous = g.previous;
		setPrevious(nullptr);
		setActionName("");
	}

	Game* getPrevious() const {
		return previous;
	}

	void setActionName(string s) {
		actionName = s;
	}

	string getActionName() const {
		return actionName;
	}

	void setPrevious(Game* c) {
		previous = c;
	}

	string getKey() {
		int** key = new int*[4];
		int* score = new int[numberOfContainers];
		string finalKey = "";

		for (int i = 0; i < 4 ; i++) {
			key[i] = new int[numberOfContainers];
		}

		for (int i = 0; i < numberOfContainers; i++) {
			score[i] = 0;
			for (int j = 0; j < 4; j++) {
				score[i] += containers[i]->getSpecificColor(j);
				key[j][i] = containers[i]->getSpecificColor(j);
				
			}
		}

	

		for (int j = 0; j < numberOfContainers; j++) {
			for (int i = 0; i < 4; i++) {
				//cout << key[i][j] << endl;
			}
		}

	

		for (int i = 1; i < numberOfContainers; i++) {
			for (int j = numberOfContainers - 1; j >= i; j--) {
				if (score[j] < score[j - 1]) {
					int temp = score[j - 1];
					score[j - 1] = score[j];
					score[j] = temp;
					for (int k = 0; k < 4; k++) {
						char temp2 = key[k][j];
						key[k][j] = key[k][j-1];
						key[k][j-1] = temp2;
					}
				}
				else if (score[j] == score[j - 1]) {
					int isBigger = -1;
					int pos = 0;
					while (pos < 4) {
						if (key[pos][j] < key[pos][j - 1]) {
							isBigger == 1;
							break;
						}

						pos++;
					}

					if (isBigger == 1) {
						int temp = score[j - 1];
						score[j - 1] = score[j];
						score[j] = temp;
						for (int k = 0; k < 4; k++) {
							char temp2 = key[k][j];
							key[k][j] = key[k][j - 1];
							key[k][j - 1] = temp2;
						}
					}
				}
			}
		}

		for (int j = 0; j < numberOfContainers; j++) {
			for (int i = 0; i < 4; i++) {
				//cout << key[i][j] << endl;
				finalKey += to_string(key[i][j]);
			}
			//cout << finalKey << endl;
		}

		

		return finalKey;
	}

	Game operator= (Game c) {

		for (int i = 0; i < numberOfContainers; i++) {
			for (int j = 0; j < 4; j++) {
				containers[i]->setSpecificColor(c.containers[i]->getSpecificColor(j), j);
			}
			containers[i]->setTop(c.containers[i]->getTop());
		}

		previous = c.previous;
		actionName = c.actionName;
		return *this;
	}

	string getAllActions() {
		string action = "";
		Game* p = this;

		string s = "";
		cout << endl;
		cout << "Printing States: " << endl;
		while (p != nullptr)
		{
			p->printCurrentState();
			cout << endl;
			s = p->getActionName() + " " + s;
			p = p->getPrevious();
		}

		return s;
	}

	int getDepth() {
		int counter = 0;
		Game* p = this;
		while (p->previous != nullptr)
		{
			p = p->previous;
			counter++;
		}
		return counter;

	}

	void printCurrentState() {
		for (int i = 3; i >= 0; i--) {
			for (int j = 0; j < numberOfContainers; j++) {
				cout << "|" << setw(3) << containers[j]->getState(i) << setw(3) << "| ";
			}
			cout << endl;
		}
	}
	
	bool isFinal() {
		bool isFinalState = true;
		for (int i = 0; i < numberOfContainers; i++) {
			if (!containers[i]->isComplete()) {
				isFinalState = false;
			}
		}

		return isFinalState;
	}

	bool isFree(int startContainer, int destinationContainer, int destinationTop, int ml, int color, Game& c) {
		//cout << "Starting:" << startContainer + 1 << ", Destination:" << destinationContainer + 1 << ", Color:" << color << ", Space:" << destinationTop << ", ml:"<<ml << endl;

		if (destinationTop == 3) {
			//cout << "The " << destinationContainer + 1 << " is full" << endl;
			return false;
		}

		if (containers[startContainer]->isComplete()) {
			return false;
		}

		if (c.getPrevious() != nullptr) {
			if (c.getPrevious() == c.getPrevious()->getPrevious()) {
				return false;
			}
		}
		
		if (destinationTop == -1) {
			c = *this;
			c.setPrevious(this);
			string action = "Move " + to_string(ml);
			action += " ml of Color " + to_string(color);
			action += " from container " + to_string(startContainer + 1);
			action += " to container " + to_string(destinationContainer + 1);
			action += '\n';

			while (ml > 0) {
				c.containers[destinationContainer]->increaseTop();
				c.containers[destinationContainer]->setSpecificColor(color, c.containers[destinationContainer]->getTop());
				ml--;
				c.containers[startContainer]->setSpecificColor(-1, c.containers[startContainer]->getTop());
				c.containers[startContainer]->decreaseTop();
			}

			//c.printCurrentState();
			//cout << endl;
			//cout << action << endl;

			c.setActionName(action);
			return true;
		}

		if (destinationTop != -1) {
			if (c.containers[destinationContainer]->getSpecificColor(c.containers[destinationContainer]->getTop()) != color) {
				//cout << "Different Color" << endl;
				return false;
			}

			c = *this;
			c.setPrevious(this);
			int movingMl = ml;

			while (ml > 0 && (3 - c.containers[destinationContainer]->getTop() > 0)) {
				c.containers[destinationContainer]->increaseTop();
				c.containers[destinationContainer]->setSpecificColor(color, c.containers[destinationContainer]->getTop());
				ml--;
				c.containers[startContainer]->setSpecificColor(-1, c.containers[startContainer]->getTop());
				c.containers[startContainer]->decreaseTop();
			}

			string action = "Move " + to_string(movingMl - ml);
			action += " ml of Color " + to_string(color);
			action += " from container " + to_string(startContainer + 1);
			action += " to container " + to_string(destinationContainer + 1);
			action += '\n';

			//cout << action <<endl;
			//c.printCurrentState();
			//cout << endl;

			c.setActionName(action);
			return true;
		}
	}

	vector <Game*> expand() {
		vector <Game*> children;
		Game* child;
		for (int j = 0; j < numberOfContainers; j++) {
			if (containers[j]->getTop() != -1) {
				for (int k = 0; k < numberOfContainers; k++) {
					if (k == j) {
						continue;
					}

					int ml = 1;
					int color = containers[j]->getSpecificColor(containers[j]->getTop());
					int position = containers[j]->getTop() - 1;
					while (position >= 0) {
						if (containers[j]->getSpecificColor(position) == color) {
							ml++;
							position--;
						}
						else {
							break;
						}
					}
					child = new Game(*this);
					if (isFree(j, k, containers[k]->getTop(), ml, color, *child)) {
						children.push_back(child);
					}
					else {
						delete child;
					}
				}
			} 
		}
		return children;
	}

};
