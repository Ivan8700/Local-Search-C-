#pragma once
#include <iostream>
#include <fstream>
#include <time.h>
#include <list>
using std::cout; using std::cin; using std::endl; using std::list; using std::pair;
class RunAlgorithm
{
private:
	int machine_number;
	int number_of_jobs;
	int sum_of_jobs;
	int opt;
	int counter_of_moves;
	int print_moves;
	int* input_arr;
	int* sum_array;
	list<pair<int, int>>* schedule;
public:
	RunAlgorithm(const int& option_to_read_input,const int& print);
	~RunAlgorithm();
	void printArray();
	void trivialSchedule();
	void LPT();
	void printSchedule();
	void printFinalSchedule();
	int findMachineWithMinimalLoad();
	void localSearch();
	bool movingEvenNum();
	bool movingTwoOddsNumbers();
	bool swappingEvenOrOddNumbers();
	bool swappingTwoOddsForEvenORtwoEvenForEven();
	bool swappingTwoForTwo();

};

