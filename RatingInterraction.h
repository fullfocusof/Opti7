#pragma once
#include <iostream>
#include <iomanip>

#include <fstream>
#include <sstream>

#include <Windows.h>
#include <conio.h>

#include <vector>
#include <string>

#include <algorithm>

using namespace std;

class RatingInterraction
{
	int _cnt, _conditions, _targetSum;
	vector<pair<string, vector<int>>> _table;
	vector<bool> _targetConds; // 0 - min, 1 - max
	vector<vector<int>> expRates;
	vector<double> weights;

public:

	void readFromFile(string filename);
	void printData();
	void printWeightTable(vector<pair<string, vector<double>>>& data);

	bool setExpertRates();

	int getMax(int numOfCond);
	int getMin(int numOfCond);
	vector<pair<string, vector<double>>> makeWeightTable();
	void sortByElementDesc(vector<pair<string, vector<double>>>& resultTable, size_t index);
	void sortByElementAsc(vector<pair<string, vector<double>>>& resultTable, size_t index);
	void sortByElementAscEvalMethod(vector<pair<int, double>>& tempVec);
	
};

bool sortVecPair(pair<int, double> d1, pair<int, double> d2);
void printQuit();