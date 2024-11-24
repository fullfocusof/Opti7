#include "RatingInterraction.h"

void printQuit()
{
	cout << endl << endl << "Backspace - возврат в меню";

	int answ = _getch();
	bool press = false;

	while (!press)
	{
		if (answ == 8)
		{
			press = true;
			break;
		}
		else
		{
			answ = _getch();
		}
	}

	system("cls");
}

void RatingInterraction::readFromFile(string filename)
{
	ifstream ifs(filename);

	if (ifs.is_open())
	{
		char skip;
		string sign;
		ifs >> _cnt >> _conditions;
		for (int i = 0; i < _conditions; i++)
		{
			ifs >> sign;
			if (sign == ">") _targetConds.push_back(true);
			else _targetConds.push_back(false);
		}
		ifs >> _targetSum;
		ifs.get(skip);

		for (int i = 0; i < _cnt; i++)
		{
			int tempValue;
			string curFullname;
			vector<int> curConds;

			getline(ifs, curFullname, '\t');
			for (int j = 0; j < _conditions; j++)
			{
				ifs >> tempValue;
				curConds.push_back(tempValue);
			}

			_table.push_back(make_pair(curFullname, curConds));
			ifs.get(skip);
		}

		ifs.close();
	}
	else throw exception("Не удалось открыть файл");
}

void RatingInterraction::printData()
{
	if (_table.empty()) throw exception("Данные неполные или отсутствуют");
	else
	{
		cout << left;
		cout << setw(5) << "№\t\t" << setw(30) << "ФИО";
		for (int i = 0; i < _conditions; i++)
		{
			string target = _targetConds[i] ? "(max)" : "(min)";
			cout << setw(5) << "Кр" << (i + 1) << target << "\t";
		}
		cout << endl;
		
		for (int i = 0; i < _cnt; i++)
		{
			cout << setw(5) << i + 1 << "\t";
			cout << setw(30) << _table[i].first << "\t\t";
			for (int j = 0; j < _conditions; j++)
			{
				cout << setw(5) << _table[i].second[j] << "\t\t";
			}
			cout << endl;
		}
		cout << endl;

		cout << "Целевая сумма: " << _targetSum << endl;

		if (expRates.empty()) cout << "Экспертные оценки отсутствуют";
		else
		{
			int cnt = expRates.size();
			for (int i = 0; i < cnt; i++)
			{
				int cntOfCurRate = expRates[i].size();
				cout << "Оценка " << i + 1 << ": ";
				for (int j = 0; j < cntOfCurRate; j++)
				{
					cout << expRates[i][j] << " ";
				}
				cout << endl;
			}
			cout << "Веса: ";
			for (int i = 0; i < _conditions; i++)
			{
				cout << setprecision(2) << weights[i] << " ";
			}
		}
	}
}

void RatingInterraction::printWeightTable(vector<pair<string, vector<double>>>& data)
{
	if (data.empty()) throw exception("Данные неполные или отсутствуют");
	else
	{
		int cols = data[0].second.size();
		cout << left;
		cout << setw(5) << "№\t\t" << setw(30) << "ФИО";
		for (int i = 0; i < cols; i++)
		{
			if (i == cols - 2) cout << setw(10) << "Общий коэффициент\t";
			else if (i == cols - 1) cout << setw(10) << "Премия";
			else
			{
				string target = _targetConds[i] ? "(max)" : "(min)";
				cout << setw(5) << "Кр" << (i + 1) << target << "\t";
			}
			
		}
		cout << endl;

		auto prevPrec = cout.precision();
		for (int i = 0; i < _cnt; i++)
		{
			cout << setw(5) << i + 1 << "\t";
			cout << setw(30) << data[i].first << "\t\t";
			for (int j = 0; j < cols; j++)
			{
				if (j == cols - 1) cout << setprecision(prevPrec) << setw(10) << data[i].second[j];
				else if (j == cols - 2) cout << setprecision(3) << setw(10) << data[i].second[j] << "\t\t";
				else cout << setw(10) << setprecision(3) << data[i].second[j] << "\t";
			}
			cout << endl;
		}
		cout << endl;
	}
}

bool RatingInterraction::setExpertRates()
{
	int size;
	cout << "Введите количество экспертных оценок: ";
	cin >> size;	
	cin.clear();
	cin.ignore();

	for (int i = 0; i < size; i++)
	{
		vector<int> curExpRate;
		string str;
		int tempValue;

		cout << "Введите " << i + 1 << " экспертную оценку: ";
		getline(cin, str);
		istringstream iss(str);
		for (int i = 0; i < _conditions; i++) // ???
		{
			iss >> tempValue;
			curExpRate.push_back(tempValue);
		}
		if (curExpRate.size() != _conditions)
		{
			cout << "Количество оценок должно быть равно " + _conditions;
			return false;
		}

		expRates.push_back(curExpRate);
	}

	vector<int> sumsCols;
	for (int i = 0; i < _conditions; i++)
	{
		int curSum = 0;
		for (int j = 0; j < size; j++)
		{	
			curSum += expRates[j][i];
		}
		sumsCols.push_back(curSum);
	}

	int sumMainRow = 0;
	for (auto& el : sumsCols)
	{
		sumMainRow += el;
	}

	for (int i = 0; i < _conditions; i++)
	{
		weights.push_back((double)sumsCols[i] / sumMainRow);
	}

	return true;
}

int RatingInterraction::getMax(int numOfCond)
{
	int max = INT_MIN;
	for (int i = 0; i < _cnt; i++)
	{
		if (_table[i].second[numOfCond] > max) max = _table[i].second[numOfCond];
	}
	return max;
}

int RatingInterraction::getMin(int numOfCond)
{
	int min = INT_MAX;
	for (int i = 0; i < _cnt; i++)
	{
		if (_table[i].second[numOfCond] < min) min = _table[i].second[numOfCond];
	}
	return min;
}

void RatingInterraction::sortByElement(vector<pair<string, vector<double>>>& resultTable, size_t index)
{
	for (const auto& pair : resultTable)
	{
		if (index >= pair.second.size()) throw exception("Индекс выходит за границы массива");
	}

	sort(resultTable.begin(), resultTable.end(),
		[index](const pair<string, vector<double>>& a, const pair<string, vector<double>>& b)
		{
			return a.second[index] < b.second[index];
		});
}

vector<pair<string, vector<double>>> RatingInterraction::makeWeightTable()
{
	if (expRates.empty()) throw exception("Экспертные оценки отсутствуют");

	vector<pair<string, vector<double>>> resultTable;

	for (int i = 0; i < _cnt; i++)
	{
		string curFullname = _table[i].first;
		vector<double> curCoefs;
		for (int j = 0; j < _conditions; j++)
		{
			int max = getMax(j), min = getMin(j);
			double curCoef = 0.0;
			if (_targetConds[j]) curCoef = (double)(max - _table[i].second[j]) / (max - min);
			else curCoef = (double)(_table[i].second[j] - min) / (max - min);
			curCoefs.push_back(curCoef);
		}

		double overallCoef = 0.0;
		for (int j = 0; j < _conditions; j++)
		{
			overallCoef += curCoefs[j] * weights[j];
		}
		curCoefs.push_back(overallCoef);

		resultTable.push_back(make_pair(curFullname, curCoefs));
	}

	try
	{
		sortByElement(resultTable, resultTable[0].second.size() - 1);
	}
	catch (const exception& ex)
	{
		cerr << ex.what();
	}

	int targetFunc = _targetSum;
	for (int i = 0; i < _cnt; i++)
	{
		int curFunc = targetFunc / 2;
		resultTable[i].second.push_back(curFunc);
		targetFunc -= curFunc;
	}

	return resultTable;
}