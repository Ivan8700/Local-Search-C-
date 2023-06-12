# Local Search in C++
## This is a simple algorithm to the "Load scheduling with even sums" NP problem in C++.

<ins>**Problem :</ins>** Given N positive integers (also known as "jobs") and M machines, find a schedule for these jobs on the machines such that the makepsan will be minimal.  
<ins>**Algorithm :</ins>** Local search in C++  
<ins>**Initialization :</ins>** the algorithm contains 2 options - 1) use LPT algorithm. 2) use a trivial schedule which schedules the odd jobs on one machine and the even jobs on the second machine.  
<ins>**Run :</ins>** the algorithm contains a few "Steps" which it takes every time it finds an upgrade, it works as follows :  
Try step 'i' - if an upgrade was found on the schedule (can switch or move jobs between 2 machines to reduce the makespan on both of the machines), go to step 1.  
if no upgrade was found, go to step 'i+1' - if no step 'i+1' exists - exit the algorithm and print the result.  

This Heuristic is simple, fast and efficient compared to other heuristics, both in results and in Time complexity of the algorithm.
