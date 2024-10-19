/***************************************************************************
* File: main.c
* Author: David Garvin
* Procedures:
* main - creats 1000 task streams of 1000 elements each and runs them through
*			FCFS, SPN, SRT, and RRq1 and average how long
*				each would take to go through the same stream
*				
* uniform - uniform distribution (Author: Professor Goodrum)
* normal - normal distribution (Author: Professor Goodrum)
* placeMemory - places next task into memory using First Fit.
* notEmpty - checks if the memory is empty.
* FCFS - "First Come First Serve" scheduling algorithm.
* SPN - "Shortest Process Next" scheduling algorithm.
* SRT - "Shortest Remaining Time" scheduling algorithm.
* RRq1 - "Round Robin quantum = 1" scheduling algorithm.
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int STREAMSIZE = 1000;			//Global variable setting the size of the stream.
int MEMORYAVAIL = 56;			//Global variable setting how many units are in the memory.

struct task {					//Structure for each element in the stream.
	int time;					//Time is how long the task will take to complete.
	int size;					//Size is how many memory units the task will take.
	int pid;					//Process ID.
};

struct memory {					//Structure for each memory unit.
	int time;					//Time is how long the task will take to complete.
	int pid;					//Process ID.
	int arrival;				//Arrival time.
};

/***************************************************************************
* int uniform(int lo, int hi)
* Author: Professor Goodrum
* Date: Copied 3 May 2020
* Description: Returns a random number using uniform distribution.
*
* Parameters:
* 	lo			I/P		int				Low value of uniform distribution.
*	hi			I/P		int				High value of uniform distribution.
* 	uniform		O/P 	int			 	A uniformly distributed random number.
**************************************************************************/
int uniform(int lo, int hi) {
	int x, y = hi - lo + 1, z = RAND_MAX / y;	
	
	while((x = random()) > y * z);
	
	return x / z + lo;
}

/***************************************************************************
* int normal(int loops, int ulimit)
* Author: Professor Goodrum
* Date: Copied 3 May 2020
* Description: Returns a random number using normal distribution.
*
* Parameters:
* 	loops		I/P		int				Loops used for normal distribution.
*	ulimit		I/P		int				The high value for the called uniform distribution.
* 	normal		O/P 	int			 	A normally distributed random number.
**************************************************************************/
int normal(int loops, int ulimit) {
	int x = 0, i;
	
	for(i = 0; i < loops; i++) {
		x += uniform(0, ulimit);
	}
	
	return x;
}

/***************************************************************************
* int placeMemory(int arrivalTime, struct task task, struct memory mUnits[])
* Author: David Garvin
* Date: 3 May 2020
* Description: Places next task into memory using First Fit.
*
* Parameters:
* 	arrivalTime	I/P		int				Arrival time of the task.
*	task		I/P		struct task		Next task to place into memory.
*	mUnits[]	I/P		struct memory	Memory array.
* 	placeMemory	O/P 	int			 	0 if successfully placed into memory, -1 if failed.
**************************************************************************/
int placeMemory(int arrivalTime, struct task task, struct memory mUnits[]) {
	int i, j;													//Index variables.
	int bool = 0;													//Bool variable.
	
	//For loop that goes through the memory unit looking for open space.
	for(i = 0; i <= (MEMORYAVAIL - task.size); i++) {
		
		bool = 0;												//Bool value set to 0 so that we can check if an element is empty.
		//For loop that starts at the current 'i' index and checks enough units for our task.
		for(j = i; j < (task.size + i); j++) {
			
			if(mUnits[j].time != 0) {							//Checks if current unit is not 0. If so we mark 'bool' as 1 and break out of For loop.
				bool = 1;
				break;
			}
		}
		
		if(bool == 0) {											//Checks if 'bool' is 0. If so we know that we have room or else it would be 1.
			 
			//For loop that starts at the current 'i' index and checks enough units for our task. Assigns time to each unit and marked 'placed' boolean as 1.
			for(j = i; j < (task.size + i); j++) {
				mUnits[j].time = task.time;
				mUnits[j].arrival = arrivalTime;
				mUnits[j].pid = task.pid;
			}
			return 0;											//If we successfully placed the task, return 0 to show it was successfull.
		}
	}
	
	return -1;													//If we were unsuccessful in placing the task, return -1 to show it failed.
}

/***************************************************************************
* int notEmpty(struct memory mUnits[])
* Author: David Garvin
* Date: 3 May 2020
* Description: Checks if the memory is empty. Returns 0 if not empty and -1 if empty.
*
* Parameters:
* 	mUnits[]	I/P		struct memory	Memory.
* 	notEmpty	O/P 	int			 	0 if not empty, -1 if empty.
**************************************************************************/
int notEmpty(struct memory mUnits[]) {
	int i;														//Index variable.
	
	//For loop that checks through the memory. If any of the time values returned are not 0 then we know it is not empty and return 0.
	for(i = 0; i < MEMORYAVAIL; i++) {
		if(mUnits[i].time != 0) {
			return 0;
		}
	}
	return -1;													//If we make it through the memory without returning 0 then we know it is empty and return -1.
}
	
/***************************************************************************
* int FCFS(struct task stream[])
* Author: David Garvin
* Date: 3 May 2020
* Description: "First Come First Serve" scheduling algorithm.
*
* Parameters:
* 	stream		I/P		struct task		Task stream.
* 	FCFS		O/P 	int			 	Total time it takes to complete each process.
**************************************************************************/
int FCFS(struct task stream[]) {
	int i, j;													//Index variables.
	int bool;													//Boolean variable.
	int totalTime = 0, currentPID = 0;							//totalTime is time it takes to complete each process. currentPID is current process ID we're looking at.
	int turnaround = 0, countTime = 0;							//turnaround temporarily holds turnaround time for completed task. countTime is how long we've been going.
	struct memory mUnits[MEMORYAVAIL];							//Memory units.
	
	//Initializes memory units.
	for(i = 0; i < MEMORYAVAIL; i++) {
		mUnits[i].time = 0;
		mUnits[i].pid = -1;
	}
	
	//While loop places tasks until we're full. This allows second while loop to not end early because of an empty memory.
	i = 0;
	while(placeMemory(countTime, stream[i], mUnits) == 0) {
		i++;
	}
	
	//While loop places tasks until we're full, runs FCFS, then places more tasks. This continues until the memory is again empty.
	while(notEmpty(mUnits) == 0) {
		
		//While loop places tasks until we're full.
		while(placeMemory(countTime, stream[i], mUnits) == 0 && i < STREAMSIZE) {
			i++;
		}
		countTime++;											//Increment countTime. This is our total time transpired.
		
		//For loop goes through memory looking for first process.
		bool = 0;												//Set bool to 0. This lets us know when we're done with a process.
		for(j = 0; j < MEMORYAVAIL; j++) {
			
			if(mUnits[j].pid == currentPID) {					//If PID matches what we're looking for, decrement time on that task.
				mUnits[j].time--;
				
				if(mUnits[j].time == 0) {						//If decrementing time causes task's time to go to 0...
					mUnits[j].pid = -1;							//Set its process ID to -1 to show no ID.
					turnaround = countTime - mUnits[j].arrival;	//Set turnaround, how long it took for this process to complete (including wait time).
					bool = 1;									//Set bool to 1 to show that we will need a new PID.
				}
			}
		}
		
		if(bool == 1) {											//If bool is 1 and therefore we need a new PID...
			currentPID++;										//New PID will be the next one in line since this is FCFS.
			totalTime += turnaround;							//Add turnaround time to the totalTime.
		}
	}
	
	return totalTime;											//Return totalTime, the total turnaround times from all the processes.
}

/***************************************************************************
* int SPN(struct task stream[])
* Author: David Garvin
* Date: 3 May 2020
* Description: "Shortest Process Next" scheduling algorithm.
*
* Parameters:
* 	stream		I/P		struct task		Task stream.
* 	FCFS		O/P 	int			 	Total time it takes to complete each process.
**************************************************************************/
int SPN(struct task stream[]) {
	int i, j;													//Index variables.
	int bool;													//Boolean variable.
	int totalTime = 0, currentPID = 0;							//totalTime is time it takes to complete each process. currentPID is current process ID we're looking at.
	int turnaround = 0, countTime = 0;							//turnaround temporarily holds turnaround time for completed task. countTime is how long we've been going.
	int small;													//Holds smallest value when finding shortest process.
	struct memory mUnits[MEMORYAVAIL];							//Memory units.
	
	//Initializes memory units.
	for(i = 0; i < MEMORYAVAIL; i++) {
		mUnits[i].time = 0;
		mUnits[i].pid = -1;
	}
	
	//While loop places tasks until we're full. This allows second while loop to not end early because of an empty memory.
	i = 0;
	while(placeMemory(countTime, stream[i], mUnits) == 0) {
		i++;
	}
	
	//For loop goes through memory to check for smallest process and set its process ID as the currentPID. Ignores times of 0.
	small = 100;
	for(j = 0; j < MEMORYAVAIL; j++) {
		if(mUnits[j].time < small && mUnits[j].time != 0) {
			small = mUnits[j].time;
			currentPID = mUnits[j].pid;
		}
	}
	
	//While loop places tasks until we're full, runs FCFS, then places more tasks. This continues until the memory is again empty.
	while(notEmpty(mUnits) == 0) {
		
		//While loop places tasks until we're full.
		while(placeMemory(countTime, stream[i], mUnits) == 0 && i < STREAMSIZE) {
			i++;
		}
		countTime++;											//Increment countTime. This is our total time transpired.
		
		//For loop goes through memory looking for first process.
		bool = 0;												//Set bool to 0. This lets us know when we're done with a process.
		for(j = 0; j < MEMORYAVAIL; j++) {
			
			if(mUnits[j].pid == currentPID) {					//If PID matches what we're looking for, decrement time on that task.
				mUnits[j].time--;
				
				if(mUnits[j].time == 0) {						//If decrementing time causes task's time to go to 0...
					mUnits[j].pid = -1;							//Set its process ID to -1 to show no ID.
					turnaround = countTime - mUnits[j].arrival;	//Set turnaround, how long it took for this process to complete (including wait time).
					bool = 1;									//Set bool to 1 to show that process has completed.
				}
			}
		}
		
		if(bool == 1) {											//If bool is 1 and therefore we need a new PID...
			
			//For loop goes through memory to check for smallest process and set its process ID as the currentPID. Ignores times of 0.
			small = 100;
			for(j = 0; j < MEMORYAVAIL; j++) {
				if(mUnits[j].time < small && mUnits[j].time != 0) {
					small = mUnits[j].time;
					currentPID = mUnits[j].pid;
				}
			}
			
			totalTime += turnaround;							//Add turnaround time to the totalTime.
		}
	}
	
	return totalTime;											//Return totalTime, the total turnaround times from all the processes.
}

/***************************************************************************
* int SRT(struct task stream[])
* Author: David Garvin
* Date: 3 May 2020
* Description: "Shortest Remaining Time" scheduling algorithm.
*
* Parameters:
* 	stream		I/P		struct task		Task stream.
* 	FCFS		O/P 	int			 	Total time it takes to complete each process.
**************************************************************************/
int SRT(struct task stream[]) {
	int i, j;													//Index variables.
	int bool;													//Boolean variable.
	int totalTime = 0, currentPID = 0;							//totalTime is time it takes to complete each process. currentPID is current process ID we're looking at.
	int turnaround = 0, countTime = 0;							//turnaround temporarily holds turnaround time for completed task. countTime is how long we've been going.
	int small;													//Holds smallest value when finding shortest process.
	struct memory mUnits[MEMORYAVAIL];							//Memory units.
	
	//Initializes memory units.
	for(i = 0; i < MEMORYAVAIL; i++) {
		mUnits[i].time = 0;
		mUnits[i].pid = -1;
	}
	
	//While loop places tasks until we're full. This allows second while loop to not end early because of an empty memory.
	i = 0;
	while(placeMemory(countTime, stream[i], mUnits) == 0) {
		i++;
	}
	
	//While loop places tasks until we're full, runs FCFS, then places more tasks. This continues until the memory is again empty.
	while(notEmpty(mUnits) == 0) {
		
		//While loop places tasks until we're full.
		while(placeMemory(countTime, stream[i], mUnits) == 0 && i < STREAMSIZE) {
			i++;
		}
		countTime++;											//Increment countTime. This is our total time transpired.
		
		//For loop goes through memory to check for smallest process and set its process ID as the currentPID. Ignores times of 0.
		small = 100;
		for(j = 0; j < MEMORYAVAIL; j++) {
			if(mUnits[j].time < small && mUnits[j].time != 0) {
				small = mUnits[j].time;
				currentPID = mUnits[j].pid;
			}
		}
		
		//For loop goes through memory looking for first process.
		bool = 0;												//Set bool to 0. This lets us know when we're done with a process.
		for(j = 0; j < MEMORYAVAIL; j++) {
			
			if(mUnits[j].pid == currentPID) {					//If PID matches what we're looking for, decrement time on that task.
				mUnits[j].time--;
				
				if(mUnits[j].time == 0) {						//If decrementing time causes task's time to go to 0...
					mUnits[j].pid = -1;							//Set its process ID to -1 to show no ID.
					turnaround = countTime - mUnits[j].arrival;	//Set turnaround, how long it took for this process to complete (including wait time).
					bool = 1;									//Set bool to 1 to show that process has completed.
				}
			}
		}
		
		if(bool == 1) {											//If bool is 1 and therefore a process has completed...
			totalTime += turnaround;							//Add turnaround time to the totalTime.
		}
	}
	
	return totalTime;											//Return totalTime, the total turnaround times from all the processes.
}

/***************************************************************************
* int RRq1(struct task stream[])
* Author: David Garvin
* Date: 3 May 2020
* Description: "Round Robin quantum = 1" scheduling algorithm.
*
* Parameters:
* 	stream		I/P		struct task		Task stream.
* 	FCFS		O/P 	int			 	Total time it takes to complete each process.
**************************************************************************/
int RRq1(struct task stream[]) {
	int i, j, index;											//Index variables.
	int bool;													//Boolean variable.
	int totalTime = 0, currentPID = 0;							//totalTime is time it takes to complete each process. currentPID is current process ID we're looking at.
	int turnaround = 0, countTime = 0;							//turnaround temporarily holds turnaround time for completed task. countTime is how long we've been going.
	struct memory mUnits[MEMORYAVAIL];							//Memory units.
	
	//Initializes memory units.
	for(i = 0; i < MEMORYAVAIL; i++) {
		mUnits[i].time = 0;
		mUnits[i].pid = -1;
	}
	
	//While loop places tasks until we're full. This allows second while loop to not end early because of an empty memory.
	i = 0;
	while(placeMemory(countTime, stream[i], mUnits) == 0) {
		i++;
	}
	
	//While loop places tasks until we're full, runs FCFS, then places more tasks. This continues until the memory is again empty.
	while(notEmpty(mUnits) == 0) {
		
		//While loop places tasks until we're full.
		while(placeMemory(countTime, stream[i], mUnits) == 0 && i < STREAMSIZE) {
			i++;
		}
		countTime++;											//Increment countTime. This is our total time transpired.
		
		//For loop goes through memory looking for first process.
		bool = 0;												//Set bool to 0. This lets us know when we're done with a process.
		for(j = 0; j < MEMORYAVAIL; j++) {
			
			if(mUnits[j].pid == currentPID) {					//If PID matches what we're looking for, decrement time on that task.
				mUnits[j].time--;
				
				if(mUnits[j].time == 0) {						//If decrementing time causes task's time to go to 0...
					mUnits[j].pid = -1;							//Set its process ID to -1 to show no ID.
					turnaround = countTime - mUnits[j].arrival;	//Set turnaround, how long it took for this process to complete (including wait time).
					bool = 1;									//Set bool to 1 to show that process has completed.
				}
				
				index = j;										//Set index so that we can find our next process ID later.
			}
		}
		
		if(bool == 1) {											//If bool is 1 and therefore a process has completed...
			totalTime += turnaround;							//Add turnaround time to the totalTime.
		}
		
		//For loop that checks for the next process ID. If nothing else is found then we are on the last process and we can keep the same PID.
		for(j = 0; j < MEMORYAVAIL; j++) {
			
			index++;											//Increase the index that we are checking.
			if(index == MEMORYAVAIL) {							//If we reach the end of memory, we go back to the start.
				index = 0;
			}
			
			if(mUnits[index].pid != -1) {						//Ignores process IDs of -1. Assigns new PID and breaks out of for loop.
				currentPID = mUnits[index].pid;
				break;
			}
		}		
	}
	
	return totalTime;											//Return totalTime, the total turnaround times from all the processes.
}



/***************************************************************************
* int main(int argc, char *argv[])
* Author: David Garvin
* Date: 3 May 2020
* Description: Generates task stream and uses four differnt scheduling algorithms
*					and compares their results.
*
* Parameters:
* 	argc	I/P		int				Number of arguments on the command line.
* 	argv	I/P 	char *[] 		Arguments on the command line.
*										Filenames that should be given in the order
*											of Q matrix, A matrix, and Available vector.
* 	main	O/P 	int				Status code (not currently used).
**************************************************************************/
int main(int argc, char *argv[]) {
	int i, j;															//'i' and 'j' are indexes.
	int serviceTime, timeTemp;											//serviceTime holds total time for process duration of that task stream. timeTemp holds the return from the algorithm temporarily.
	int loadingTime = 0;												//loadingTime holds value for the "progress bar" so the user knows that the program is thinking.
	double trFCFS = 0, trSPN = 0, trSRT = 0, trRR = 0;					//Turnaround times for all algorithms.
	double rTrFCFS = 0, rTrSPN = 0, rTrSRT = 0, rTrRR = 0;				//Relative turnaround times for all algorithms.
	struct task stream[STREAMSIZE];										//An array of strutures containing 'time' and 'size' integers.
	
	//For loop runs 1000 experiments.
	for(i = 0; i < 1000; i++) {
		
		serviceTime = 0;												//Reset service time to 0 so that it can get a new total count.
		
		//For loop sets a new stream with values between 1-16 for time and size. Also assigns process ID equal to j.
		for(j = 0; j < STREAMSIZE; j++) {
			stream[j].size = normal(4, 4);
			
			if(stream[j].size < 1) {
				stream[j].size = 1;
			}
			
			stream[j].time = normal(4, 4);
			
			if(stream[j].time < 1) {
				stream[j].time = 1;
			}
			
			serviceTime += stream[j].time;
			
			stream[j].pid = j;
		}
		
		timeTemp = FCFS(stream);										//This block gets time from FCFS, adds its turnaround and relative turnaround times to their respective totals.
		trFCFS += (double)timeTemp / (double)STREAMSIZE;
		rTrFCFS += (double)timeTemp / (double)serviceTime;
		
		timeTemp = SPN(stream);											//This block gets time from SPN, adds its turnaround and relative turnaround times to their respective totals.
		trSPN += (double)timeTemp / (double)STREAMSIZE;
		rTrSPN += (double)timeTemp / (double)serviceTime;
		
		timeTemp = SRT(stream);											//This block gets time from SRT, adds its turnaround and relative turnaround times to their respective totals.
		trSRT += (double)timeTemp / (double)STREAMSIZE;
		rTrSRT += (double)timeTemp / (double)serviceTime;
		
		timeTemp = RRq1(stream);										//This block gets time from RRq1, adds its turnaround and relative turnaround times to their respective totals.
		trRR += (double)timeTemp / (double)STREAMSIZE;
		rTrRR += (double)timeTemp / (double)serviceTime;
		
		if(i % 100 == 0) {												//Prints out a loading time so that the user knows that the program is working.
			loadingTime = loadingTime + 10;
			printf("\nLoading: %d%%", loadingTime);
		}
	}
	
	//Following block prints out our results divided by the number of experiments for an average.
	printf("\n\nFCFS:\tTurnaround Time = %.3f,\tRelative Turnaround = %.3f\n", trFCFS / 1000, rTrFCFS / 1000);
	printf("\nSPN:\tTurnaround Time = %.3f,\tRelative Turnaround = %.3f\n", trSPN / 1000, rTrSPN / 1000);
	printf("\nSRT:\tTurnaround Time = %.3f,\tRelative Turnaround = %.3f\n", trSRT / 1000, rTrSRT / 1000);
	printf("\nRRq1:\tTurnaround Time = %.3f,\tRelative Turnaround = %.3f\n", trRR / 1000, rTrRR / 1000);
	
	return 0;															//Ends program.
}