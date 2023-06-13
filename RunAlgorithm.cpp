#include "RunAlgorithm.h"
int max(const int& a, const int& b);
bool comparePairs(const std::pair<int, int>& a, const std::pair<int, int>& b);

RunAlgorithm::RunAlgorithm(const int& option_to_read_input, const int& print)
{
	int cnt_odd = 0;
	this->sum_of_jobs = 0;
	this->counter_of_moves = 0;
	this->print_moves = print;
	if (option_to_read_input == 0) //Read input from .txt file
	{
		std::ifstream infile;
		infile.open("inputs/input1.txt");
		infile >> machine_number;
		cout << "Enter number of machines : ";
		cout << machine_number << "\n";
		infile >> number_of_jobs;
		cout << "Enter Amount of numbers : ";
		cout << number_of_jobs << "\n";
		input_arr = new int[number_of_jobs];
		sum_array = new int[machine_number];
		if (infile)
		{
			int i = 0;
			while (infile >> input_arr[i] && i < number_of_jobs)
			{
				sum_of_jobs += input_arr[i];
				if (input_arr[i] % 2 == 1)
					cnt_odd++;
				i++;
			}

			infile.close();
		}
		else
		{
			cout << "File was not found" << endl;
		}
	}
	else
	{
		if (option_to_read_input == 1) //Manual insertion input
		{
			cout << "Enter amount of machines : "; cin >> machine_number;
			cout << "\n Enter amount of jobs : "; cin >> number_of_jobs; cout << "\n";
			input_arr = new int[number_of_jobs];
			sum_array = new int[machine_number];
			for (int i = 0; i < number_of_jobs; i++)
			{
				cout << "Enter a job : ";
				cin >> input_arr[i]; cout << "\n";
				this->sum_of_jobs += input_arr[i];
				if (input_arr[i] % 2 == 1)
				{
					cnt_odd++;
				}
			}
		}
		else //Random input
		{
			srand((unsigned int)time(NULL));
			cout << "Enter amount of machines : "; cin >> machine_number;
			cout << "\n Enter amount of jobs : "; cin >> number_of_jobs; cout << "\n";
			input_arr = new int[number_of_jobs];
			sum_array = new int[machine_number];
			for (int i = 0; i < number_of_jobs; i++)
			{
				input_arr[i] = (rand() % 40) + 21; // random size of jobs in [21,60]
				this->sum_of_jobs += input_arr[i];
				if (input_arr[i] % 2 == 1)
					cnt_odd++;
			}
		}
	}
	if (cnt_odd % 2 == 1)
	{
		for (int i = 0; i < number_of_jobs; i++) //if there is odd number of odd jobs, change some even job to odd.
		{
			if (input_arr[i] % 2 == 0)
			{
				--input_arr[i];
				--sum_of_jobs;
				break; //economize run time
			}
		}
	}
	opt = ceil(double(sum_of_jobs) / machine_number);
	opt = (opt % 2 == 1) ? ++opt : opt;
	for (int i = 0; i < machine_number; i++)
	{
		sum_array[i] = 0;
	}
	this->schedule = new list<pair<int, int>>[machine_number];
}
RunAlgorithm::~RunAlgorithm()
{
	delete[] input_arr;
	delete[] sum_array;
	delete[] schedule;
}
void RunAlgorithm::printArray()
{
	cout << endl << "The input is : " << endl;
	for (int i = 0; i < number_of_jobs; i++)
	{
		if (i < (number_of_jobs - 1))
			cout << "(" << input_arr[i] << "," << i + 1 << "), ";
		else
			cout << "(" << input_arr[i] << "," << i + 1 << ")";
	}
	cout << endl << "The sum is : " << sum_of_jobs << endl;
	cout << "OPT is bound below by sum/machine number which is : " << opt << "\n\n";
}
/*Placing the odd jobs of the input into the first machine and even jobs in the second machine.*/
void RunAlgorithm::trivialSchedule()
{
	for (int i = 0; i < number_of_jobs; i++)
	{
		if (input_arr[i] % 2 == 1)
		{
			this->schedule[0].push_back(std::make_pair(input_arr[i], i + 1));
			this->sum_array[0] += input_arr[i];
		}
		else
		{
			schedule[1].push_back(std::make_pair(input_arr[i], i + 1));
			sum_array[1] += input_arr[i];
		}
	}
}
/*Sort the input in decreasing order, firstly place the even jobs of the input in the minimal load machine,
next place the biggest and smallest odd jobs in the minimal load machine.
*/
void RunAlgorithm::LPT()
{
	list<pair<int, int>> input_list;
	for (int i = 0; i < number_of_jobs; i++)
	{
		input_list.push_front(std::make_pair(input_arr[i], (i + 1)));
	}
	input_list.sort(comparePairs); //sort the input in decreasing order
	int indexMin = -1;
	list<pair<int, int>>::iterator it, it2;
	//================== Assign even jobs =========================//
	for (it = input_list.begin(); it != input_list.end();)
	{
		if ((*it).first % 2 == 0) //schedule each even job on the minimal load machine.
		{
			if (indexMin == -1)
			{
				schedule[0].push_back((*it));
				indexMin = 1;
				sum_array[0] += (*it).first;
			}
			else
			{
				schedule[indexMin].push_back(*it);
				sum_array[indexMin] += (*it).first;
				indexMin = findMachineWithMinimalLoad();
			}
			it2 = (++it);
			input_list.erase(--it);
			it = it2;
		}
		else
			++it;
	}
	//=============================================================//
	//=================== Assign odd jobs =========================//
	if (indexMin == -1) indexMin = 0; //if there is no even jobs.
	it2 = input_list.end();
	for (it = input_list.begin(); input_list.size() != 0; it = input_list.begin()) //place the biggest and minimal odd job in the minimal load machine.
	{
		schedule[indexMin].push_back(input_list.front());
		schedule[indexMin].push_back(input_list.back());
		sum_array[indexMin] += (input_list.front().first + input_list.back().first);
		input_list.pop_front(); input_list.pop_back();
		indexMin = findMachineWithMinimalLoad();
	}
	//=============================================================//
}
/*Print the schedule*/
void RunAlgorithm::printSchedule()
{
	cout << "------------------------------------------------------\n\n";
	cout << "Printing machine status \n";
	int makespan = 0;
	for (int i = 0; i < machine_number; i++)
	{
		cout << "Machine number " << (i + 1) << "\n";
		cout << "{";
		for (list<pair<int, int>>::iterator it = schedule[i].begin(); it != schedule[i].end(); ++it)
		{
			cout << "(" << (*it).first << "," << (*it).second << ") ";
		}
		cout << "}" << " \n -> Sum of The Machine is : " << sum_array[i] << "\n \n";
		if (sum_array[i] > makespan) makespan = sum_array[i];
	}
	cout << "The makespan is " << makespan << endl;
	cout << "\n------------------------------------------------------\n";
}
/*Traverse the sumArray and find the minimal load machine.*/
int RunAlgorithm::findMachineWithMinimalLoad()
{
	int indexMin = 0;
	for (int i = 0; i < this->machine_number; i++)
		if (sum_array[indexMin] > sum_array[i])
			indexMin = i;
	return indexMin;
}
/* Traversing the jobs on the machines, trying to move some even job to another machine to decrease the load.
if such a move exists - return True, else return False; */
bool RunAlgorithm::movingEvenNum()
{
	list<pair<int, int>>::iterator it, it2;
	for (int first_machine_index = 0; first_machine_index < machine_number; first_machine_index++) //pick some machine with index first_machine_index
	{	//traverse the jobs on the machine first_machine_index
		for (it = schedule[first_machine_index].begin(); it != schedule[first_machine_index].end(); ++it) 
		{
			if ((*it).first % 2 == 0)
			{	//look at another machine that is different from the first_machine_index
				for (int second_machine_index = 0; second_machine_index < machine_number; second_machine_index++) 
				{
					if (second_machine_index != first_machine_index)
					{	//check if moving the job from first_machine_index to second_machine_index might decrease the maximal load on both of the machines.
						if (sum_array[second_machine_index] + ((*it).first) < max(sum_array[first_machine_index], sum_array[second_machine_index])) 
						{
							if (this->print_moves == 1)
							{
								cout << "Moving (" << (*it).first << "," << (*it).second << ") from machine " << first_machine_index + 1 << " to machine " << second_machine_index + 1 << "\n";
								cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ \n";
							}
							sum_array[first_machine_index] -= (*it).first;
							sum_array[second_machine_index] += (*it).first;
							schedule[second_machine_index].push_back(*it);
							it2 = (++it);
							schedule[first_machine_index].erase(--it);
							it = it2;
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}
/*Traversing the schedule and trying to find 2 odd jobs to move from some machine i
to another machine j by decreasing the maximal load in both of the machines.
Return True if such a move was found, else Return False. */
bool RunAlgorithm::movingTwoOddsNumbers()
{
	list<pair<int, int>>::iterator it, it2;
	for (int first_machine_index = 0; first_machine_index < machine_number; first_machine_index++) //pick some machine
	{
		for (it = schedule[first_machine_index].begin(); it != schedule[first_machine_index].end(); ++it) //traverse its jobs
		{
			if ((*it).first % 2 == 1) //if found some odd job
			{
				it2 = (++it); --it;
				for (; it2 != schedule[first_machine_index].end(); ++it2) //continue traversing the same machine to find another odd job
				{
					if ((*it2).first % 2 == 1)
					{	//if found 2 odd jobs, Try to move the jobs from 'first_machine_index' to the machine 'second_machine_index'
						for (int second_machine_index = 0; second_machine_index < machine_number; second_machine_index++)
						{
							if (sum_array[second_machine_index] + (*it).first + (*it2).first < max(sum_array[first_machine_index], sum_array[second_machine_index]))
							{
								if (this->print_moves == 1)
								{
									cout << "Moving 2 odds jobs from machine number " << first_machine_index + 1 << " with numbers (" << (*it).first << "," << (*it).second << ") and (" << (*it2).first << "," << (*it2).second << ") to machine number " << second_machine_index + 1 << "\n";
									cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ \n";
								}
								sum_array[first_machine_index] -= ((*it).first + (*it2).first);
								sum_array[second_machine_index] += ((*it).first + (*it2).first);
								schedule[second_machine_index].push_back((*it));
								schedule[second_machine_index].push_back((*it2));
								list<pair<int, int>>::iterator temp;
								temp = (++it2);
								schedule[first_machine_index].erase(--it2);
								temp = (++it);
								schedule[first_machine_index].erase(--it);
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}
/*Traversing the schedule and try to find some even to switch with another even job or odd job to switch with odd job.
Return True if found solution, else Return false.
Because of some symmetric cases - the traversal of the machines in this method is more limited, the purpose is to economize the run time.*/
bool RunAlgorithm::swappingEvenOrOddNumbers()
{
	list<pair<int, int>>::iterator it, it2;
	list<pair<int, int>>::iterator it3;
	for (int first_machine_index = 0; first_machine_index < (machine_number - 1); first_machine_index++) //pick some machine, don't pick the last machine.
	{
		for (it = schedule[first_machine_index].begin(); it != schedule[first_machine_index].end(); ++it) //traverse the jobs in 'first_index_machine'
		{	//pick a machine from the right-hand machines of 'first_index_machine'
			for (int second_index_machine = first_machine_index + 1; second_index_machine < machine_number; second_index_machine++) 
			{	 //traverse the jobs in the second machine
				for (it2 = schedule[second_index_machine].begin(); it2 != schedule[second_index_machine].end(); ++it2)
				{
					if (((*it2).first % 2) == ((*it).first % 2)) //if both of the jobs are odd or even
					{	// check if switching the jobs between this machines yield a better load. (first machine - the job on it + the incoming job from the second machine) and (second machine - the job on it + the incoming job from the second machine)
						if ((sum_array[first_machine_index] + (*it2).first - (*it).first) < max(sum_array[first_machine_index], sum_array[second_index_machine]) && (sum_array[second_index_machine] + (*it).first - (*it2).first < max(sum_array[first_machine_index], sum_array[second_index_machine])))
						{
							if (this->print_moves == 1)
							{
								if ((*it).first % 2 == 0)
									cout << "Swapped even numbers (" << (*it).first << "," << (*it).second << ") from machine number " << first_machine_index + 1 << " and (" << (*it2).first << "," << (*it2).second << ") from machine number " << second_index_machine + 1 << "\n";
								else
									cout << "Swapped odd numbers (" << (*it).first << "," << (*it).second << ") from machine number " << first_machine_index + 1 << " and (" << (*it2).first << "," << (*it2).second << ") from machine number " << second_index_machine + 1 << "\n";
								cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ \n";
							}
							sum_array[first_machine_index] = sum_array[first_machine_index] + (*it2).first - (*it).first;
							sum_array[second_index_machine] = sum_array[second_index_machine] + (*it).first - (*it2).first;
							schedule[first_machine_index].push_back(*it2);
							schedule[second_index_machine].push_back(*it);
							it3 = (++it);
							schedule[first_machine_index].erase(--it);
							it = it3;
							it3 = (++it2);
							schedule[second_index_machine].erase(--it2);
							it2 = it3;
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}
/*Traverse the schedule and try to switch 2 odd or 2 even jobs for 1 even job in another machine
if found such a switch Return True, else Return False.*/
bool RunAlgorithm::swappingTwoOddsForEvenORtwoEvenForEven()
{
	list<pair<int, int>>::iterator it2, itTemp;
	for (int first_machine_index = 0; first_machine_index < machine_number; first_machine_index++) //pick some machine
	{
		for (list<pair<int, int>>::iterator it1 = schedule[first_machine_index].begin(); it1 != schedule[first_machine_index].end(); ++it1) //traverse the jobs in the machine
		{
			it2 = (++it1); --it1;
			for (; it2 != schedule[first_machine_index].end(); ++it2) //traverse for another job in the same machine such that both of them are odd or even.
			{
				if (((*it2).first % 2) == ((*it1).first % 2)) 
				{
					for (int second_machine_index = 0; second_machine_index < machine_number; second_machine_index++) //traverse the other machines
					{
						if (second_machine_index != first_machine_index)
						{
							for (list<pair<int, int>>::iterator it3 = schedule[second_machine_index].begin(); it3 != schedule[second_machine_index].end(); ++it3) //traverse the jobs in the second machine
							{
								if ((*it3).first % 2 == 0) //if the job is even
								{	//check if the switch yields a better result
									// (first machine - both of the jobs on it + the new incoming job) and (second machine + both of the jobs incoming from the other machine - the job on this machine)
									if ((sum_array[first_machine_index] - (*it1).first - (*it2).first + (*it3).first) < max(sum_array[first_machine_index], sum_array[second_machine_index]) && ((sum_array[second_machine_index] - (*it3).first + (*it1).first + (*it2).first) < max(sum_array[first_machine_index], sum_array[second_machine_index])))
									{
										if (this->print_moves == 1)
										{
											cout << "Swapping the numbers (" << (*it1).first << "," << (*it1).second << ") and (" << (*it2).first << "," << (*it2).second << ") from machine number " << first_machine_index + 1 << " with the even number (" << (*it3).first << "," << (*it3).second << ") from machine " << second_machine_index + 1 << endl;
											cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
										}
										sum_array[first_machine_index] -= (*it1).first; sum_array[first_machine_index] -= (*it2).first; sum_array[first_machine_index] += (*it3).first;
										sum_array[second_machine_index] -= (*it3).first; sum_array[second_machine_index] += (*it1).first; sum_array[second_machine_index] += (*it2).first;
										schedule[first_machine_index].push_back((*it3));
										schedule[second_machine_index].push_back((*it1)); schedule[second_machine_index].push_back((*it2));
										itTemp = (++it1);
										schedule[first_machine_index].erase(--it1);
										it1 = itTemp;
										itTemp = (++it2);
										schedule[first_machine_index].erase(--it2);
										it2 = itTemp;
										itTemp = (++it3);
										schedule[second_machine_index].erase(--it3);
										it3 = itTemp;
										return true;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return false;
}
/*Traverse the Schedule, try to find 2 even/odd jobs and switch them with 2 even/odd jobs from another machine
if found such a switch which results in decreasing load on both of the machines - return True, else return False.
in this method, we traverse less machines due to many symmetric cases in order to economize algorithm run time.*/
bool RunAlgorithm::swappingTwoForTwo()
{
	list<pair<int, int>>::iterator run, run2, temp;
	for (int first_machine_index = 0; first_machine_index < (machine_number - 1); first_machine_index++) //pick the first machine
	{
		for (list<pair<int, int>>::iterator it = schedule[first_machine_index].begin(); it != schedule[first_machine_index].end(); ++it) //traverse the jobs
		{
			run = (++it); --it;
			//traverse for another job in the same machine such that both of the jobs will be odd or even.
			for (; run != schedule[first_machine_index].end(); ++run) 
			{
				if ((*run).first % 2 == ((*it).first % 2))
				{
					for (int second_machine_index = first_machine_index + 1; second_machine_index < machine_number; second_machine_index++) //traverse the machines in the right-hand side of the first machine.
					{
						for (list<pair<int, int>>::iterator it2 = schedule[second_machine_index].begin(); it2 != schedule[second_machine_index].end(); ++it2)
						{
							run2 = (++it2); --it2;
							for (; run2 != schedule[second_machine_index].end(); ++run2) //look for 2 even/odd jobs in the second machine
							{
								if (((*run2).first % 2) == ((*it2).first % 2))
								{	//check if the switch will yield a better result
									//(first machine - 2 of the jobs on it + 2 of the incoming jobs) and (second machine - 2 of the jobs on it + 2 of the incoming jobs)
									if (((sum_array[first_machine_index] + (*run2).first + (*it2).first - (*it).first - (*run).first) < max(sum_array[first_machine_index], sum_array[second_machine_index])) && (sum_array[second_machine_index] + (*run).first + (*it).first - (*run2).first - (*it2).first < max(sum_array[first_machine_index], sum_array[second_machine_index])))
									{
										if (this->print_moves == 1)
										{
											cout << "Switching 2 numbers from machine " << first_machine_index + 1 << "numbers { (" << (*it).first << "," << (*it).second << ") , (" << (*run).first << "," << (*run).second << ")}  with machine number " << second_machine_index + 1 << " with numbers {(" << (*it2).first << "," << (*it2).second << ") , (" << (*run2).first << "," << (*run2).second << ")} \n";
											cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
										}
										sum_array[first_machine_index] += ((*run2).first + (*it2).first - (*it).first - (*run).first);
										sum_array[second_machine_index] += ((*run).first + (*it).first - (*run2).first - (*it2).first);
										schedule[first_machine_index].push_back(*run2); schedule[first_machine_index].push_back(*it2);
										schedule[second_machine_index].push_back(*run); schedule[second_machine_index].push_back(*it);
										temp = ++run; schedule[first_machine_index].erase(--run); run = temp;
										temp = ++it; schedule[first_machine_index].erase(--it); it = temp;
										temp = ++run2; schedule[second_machine_index].erase(--run2); run2 = temp;
										temp = ++it2; schedule[second_machine_index].erase(--it2); it2 = temp;
										return true;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return false;
}
/*This is the main algorithm
it tries to find an upgrade to the schedule using the steps in the following order :
 1) Move even job to another machine
 2) Move 2 odd job to another machine
 3) Swap 2 even jobs or 2 odd jobs
 4) Swap 2 even/odd jobs for 1 even job
 5) Swap 2 even/odd jobs for 2 even/odd jobs.

 The algorithm runs step 'i', if found an upgrade on the step 'i' - do the switching upgrade and go to step 1 again
 else, go to step 'i+1'.
 if there is no step 'i+1', exit the algorithm and go ahead to print the final schedule.
 */
void RunAlgorithm::localSearch()
{
	if (this->print_moves == 1)
		this->printSchedule();
	bool movedEvenNumber(true), movedTwoOdds(true), swappedEvenOrOdd(true), swappedTwoForOne(true), swappedTwoForTwo(true);
	while (movedEvenNumber || movedTwoOdds || swappedEvenOrOdd || swappedTwoForOne || swappedTwoForTwo)
	{
		movedEvenNumber = movingEvenNum();
		if (movedEvenNumber)
		{
			if (this->print_moves == 1)
				this->printSchedule();
			counter_of_moves++;
			continue;
		}
		movedTwoOdds = movingTwoOddsNumbers();
		if (movedTwoOdds)
		{
			if (this->print_moves == 1)
				this->printSchedule();
			counter_of_moves++;
			continue;
		}
		swappedEvenOrOdd = swappingEvenOrOddNumbers();
		if (swappedEvenOrOdd)
		{
			if (this->print_moves == 1)
				this->printSchedule();
			counter_of_moves++;
			continue;
		}
		swappedTwoForOne = swappingTwoOddsForEvenORtwoEvenForEven();
		if (swappedTwoForOne)
		{
			if (this->print_moves == 1)
				this->printSchedule();
			counter_of_moves++;
			continue;
		}
		swappedTwoForTwo = swappingTwoForTwo();
		if (swappedTwoForTwo)
		{
			if (this->print_moves == 1)
				this->printSchedule();
			counter_of_moves++;
		}
	}
	printFinalSchedule();
}
/*Print Final schedule*/
void RunAlgorithm::printFinalSchedule()
{
	cout << "\n \n";
	int makespan = 0;
	list<pair<int, int>>::iterator it;
	cout << "----------------------------------------------------------------------------- \n";
	cout << "The output will be in the form of (number, index) :\n";
	for (int i = 0; i < machine_number; i++)
	{
		cout << "Machine number " << (i + 1) << "\n";
		cout << "{";
		for (it = schedule[i].begin(); it != schedule[i].end(); ++it)
		{
			cout << "(" << (*it).first << "," << (*it).second << ") ";
		}
		cout << "}" << " \n -> Sum of The Machine is : " << sum_array[i] << "\n \n";
		if (sum_array[i] > makespan) makespan = sum_array[i];
	}
	cout << "The makespan is " << makespan << endl;
	cout << "OPT is bound below by sum/machine number which is : " << this->opt << endl;
	cout << "The sum of jobs is : " << this->sum_of_jobs << endl;
	cout << "Amount of changes by the local search is : " << this->counter_of_moves << endl;
}
bool comparePairs(const std::pair<int, int>& a, const std::pair<int, int>& b)
{
	return a.first > b.first;
}
int max(const int& a, const int& b) { return a > b ? a : b; }