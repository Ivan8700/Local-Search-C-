#include <iostream>
#include <list>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include "RunAlgorithm.h"
using namespace std::chrono;
const static int fromMicroToSec = 1000000;
int main()
{
	int print, initialization_schedule,get_input_option;
	cout << "Would you like to print all moves or only the last output? 1 - yes , 0 - no : ";
	cin >> print;
	cout << "\nHow do you want to insert the input ? 0 - read from .txt file, 1 - insert manually, 2 - random input : ";
	cin >> get_input_option;
	cout << "\nWould you like to run LPT or Trivial schedule for the initialization ? 1 - LPT , 0 - Trivial : ";
	cin >> initialization_schedule;
	RunAlgorithm alg(get_input_option, print);
	auto start = high_resolution_clock::now(); //Start clock
	 /* create txt file for the output */
	//std::ofstream out_file; out_file.open("C:/Users/Ivan/Desktop/output.txt"); 
	//std::streambuf* cout_buffer = cout.rdbuf();
	//cout.rdbuf(out_file.rdbuf());
	alg.printArray();
	if (initialization_schedule == 1)
	{
		alg.LPT();
	}
	else
	{
		alg.trivialSchedule();
	}
	alg.localSearch();
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << endl << " Time taken by function : " << float(duration.count()) / fromMicroToSec << " seconds" << endl;
	// Close the file if was opened
	//out_file.close(); 
	//cout.rdbuf(cout_buffer);
	return 0;
}