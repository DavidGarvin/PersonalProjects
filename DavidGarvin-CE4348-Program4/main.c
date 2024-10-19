/***************************************************************************
* File: main.c
* Author: David Garvin
* Procedures:
* main - creats 1000 streams of 1000 elements each and runs them through
*			First Fit, Best Fit, Worst Fit, and Next Fit and average how long
*				each would take to go through the same stream
*				
* uniform - uniform distribution (Author: Professor Goodrum)
* normal - normal distribution (Author: Professor Goodrum)
* firstFit - finds first available space in the memory for the next
*			task in the stream
* bestFit - finds closest fitting available space in the memory for
*			the next task in the stream
* worstFit - finds the least closest fitting available space in the memory
*			for the next task in the stream
* nextFit - finds the first available space in the memory, while searching
*			from where the last task was placed, for the next task in the stream
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int STREAMSIZE = 1000;			//Global variable setting the size of the stream.
int MEMORYAVAIL = 56;			//Global variable setting how many units are in the memory.

struct data {					//Structure for each element in the stream. Has two integer values for time and size.
	int time;					//Time is how long the task will take to complete.
	int size;					//Size is how many memory units the task will take.
};

/***************************************************************************
* int uniform(int lo, int hi)
* Author: Professor Goodrum
* Date: Copied 19 April 2020
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
* Date: Copied 19 April 2020
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
* int firstFit(struct data stream)
* Author: David Garvin
* Date: 19 April 2020
* Description: Finds first available space in the memory for the next
*			task in the stream
*
* Parameters:
* 	stream		I/P		struct data		Stream of data with a size and a time integer values.
* 	firstFit	O/P 	int			 	Total time transpired to go through the stream.
**************************************************************************/
int firstFit(struct data stream[]) {
	int i, j, k;													//'i', 'j', and 'k' are indexes.
	int placed = 0, bool = 0, totalTime = 0;						//'placed' and 'bool' are booleans. 'totalTime' is the time transpired.
	int mUnits[MEMORYAVAIL];										//Memory units. The amount is set by the global variable MEMORYAVAIL.
	
	for(i = 0; i < MEMORYAVAIL; i++) {								//Initialize the memory units.
		mUnits[i] = 0;
	}
	
	//For loop that goes through all the tasks in the stream.
	for(i = 0; i < STREAMSIZE; i++) {
		placed = 0;													//'placed' is set to 0 to start the loop.
		
		//For loop that goes through the memory unit looking for open space.
		for(j = 0; j <= (MEMORYAVAIL - stream[i].size); j++) {
			bool = 0;												//'bool' is set to 0 to start the loop.
			
			//For loop that starts at the current 'j' index and checks enough units for our task.
			for(k = j; k < (stream[i].size + j); k++) {
				
				if(mUnits[k] != 0) {								//Checks if current unit is not 0. If so we mark 'bool' as 1 and break out of For loop.
					bool = 1;
					break;
				}
			}
			
			if(bool == 0) {											//Checks if 'bool' is 0. If so we know that we have room or else it would be 1.
				 
				 //For loop that starts at the current 'j' index and checks enough units for our task. Assigns time to each unit and marked 'placed' boolean as 1.
				 for(k = j; k < (stream[i].size + j); k++) {
					 mUnits[k] = stream[i].time;
					 placed = 1;
				 }
			}
			
			if(placed == 1) {										//If we successfully placed the task, break out of For loop.
				break;
			}
		}
		
		if(bool != 0 && placed == 0) {								//If we didn't place the task decrement all times, increment 'totalTime', and decrement 'i'.
			
			//For loop that goes through the memory units.
			for(j = 0; j < MEMORYAVAIL; j++) {
				
				if(mUnits[j] > 0) {									//Decrements if unit's time is above 0.
					mUnits[j] = mUnits[j] - 1;
				}
			}
			
			totalTime++;											//Since we couldn't place the task, increment 'totalTime'.
			i--;													//Decrement 'i' so we go through this task again.
		}
	}
	
	int largest = 0;												//'largest' holds value of highest remaining time left in the memory.
	//For loop goes through the memory, checking all of the time.
	for(i = 0; i < MEMORYAVAIL; i++) {
		if(mUnits[i] > largest) {
			largest = mUnits[i];
		}
	}
	totalTime += largest;											//Adds largest amount of time left to the 'totalTime'.
	
	return totalTime;
}

/***************************************************************************
* int bestFit(struct data stream)
* Author: David Garvin
* Date: 19 April 2020
* Description: Finds closest fitting available space in the memory for
*			the next task in the stream
*
* Parameters:
* 	stream		I/P		struct data		Stream of data with a size and a time integer values.
* 	bestFit		O/P 	int			 	Total time transpired to go through the stream.
**************************************************************************/
int bestFit(struct data stream[]) {
	int i, j, index;												//'i' and 'j' are indexes. 'index' holds starting index for where we're putting the task.
	int count, small, totalTime = 0;								//'count' is our count of how many 0's in a row. 'small' is our tightest fit so far. 'totalTime' is time transpired.
	int mUnits[MEMORYAVAIL];										//Memory units. The amount is set by the global variable MEMORYAVAIL.
	
	//For loop initializes memory units.
	for(i = 0; i < MEMORYAVAIL; i++) {
		mUnits[i] = 0;
	}
	
	//For loop that goes through all the tasks in the stream.
	for(i = 0; i < STREAMSIZE; i++) {
		index = -1;													//'index' is set to -1 so that we know if we haven't placed a task.
		count = 0;													//'count' is set to 0 to start the For loop.
		small = 100;												//'small' is set to 100 so that any value to start will be smaller.
		
		//For loop that goes through memory units.
		for(j = 0; j < MEMORYAVAIL; j++) {
			
			if(mUnits[j] == 0) {									//If current unit is 0, increment 'count'.
				count++;
			}
			
			if(mUnits[j] != 0) {									//If current unit is not 0, compare our 'count'.
				
				if(count >= stream[i].size && count < small) {		//If 'count' is big enough to fit task and is smaller than current 'small', make it the new 'small' and set 'index' to beginning of where we are placing task.
					small = count;
					index = j - count;
				}
				count = 0;											//Reset 'count' to 0.
			}
			
			if(j == (MEMORYAVAIL - 1)) {							//If current unit is at the end of the memory units array, compare our 'count'.
				
				if(count >= stream[i].size && count < small) {		//If 'count' is big enough to fit task and is smaller than current 'small', make it the new 'small' and set 'index' to beginning of where we are placing task.
					small = count;
					index = j - count + 1;
				}
				count = 0;											//Reset 'count' to 0.
			}
		}
		
		if(index != -1) {											//If 'index' is not -1 then we have placed the task.
			
			//For loop that goes from 'index' and checks as many units as our task size. Assigns task time to them.
			for(j = index; j < (stream[i].size + index); j++) {
				mUnits[j] = stream[i].time;
			}
		}
		else {														//If 'index' is -1 then we have not placed the task.
			
			//For loop that goes through the memory units and decrements all of those over 0.
			for(j = 0; j < MEMORYAVAIL; j++) {
				
				if(mUnits[j] > 0) {
					mUnits[j] = mUnits[j] - 1;
				}
			}
			totalTime++;											//Increment 'totalTime' since we couldn't place task.
			i--;													//Decrement 'i' so we go through this task again.
		}
	}
	
	int largest = 0;												//'largest' holds value of highest remaining time left in the memory.
	//For loop goes through the memory, checking all of the time.
	for(i = 0; i < MEMORYAVAIL; i++) {
		if(mUnits[i] > largest) {
			largest = mUnits[i];
		}
	}
	totalTime += largest;											//Adds largest amount of time left to the 'totalTime'.
	
	return totalTime;
}

/***************************************************************************
* int worstFit(struct data stream)
* Author: David Garvin
* Date: 19 April 2020
* Description: Finds the least closest fitting available space in the memory
*			for the next task in the stream
*
* Parameters:
* 	stream		I/P		struct data		Stream of data with a size and a time integer values.
* 	worstFit	O/P 	int			 	Total time transpired to go through the stream.
**************************************************************************/
int worstFit(struct data stream[]) {
	int i, j, index;												//'i' and 'j' are indexes. 'index' holds starting index for where we're putting the task.
	int count, large, totalTime = 0;								//'count' is our count of how many 0's in a row. 'large' is our loosest fit so far. 'totalTime' is time transpired.
	int mUnits[MEMORYAVAIL];										//Memory units. The amount is set by the global variable MEMORYAVAIL.
	
	//For loop initializes memory units.
	for(i = 0; i < MEMORYAVAIL; i++) {
		mUnits[i] = 0;
	}
	
	//For loop that goes through all the tasks in the stream.
	for(i = 0; i < STREAMSIZE; i++) {
		index = -1;													//'index' is set to -1 so that we know if we haven't placed a task.
		count = 0;													//'count' is set to 0 to start the For loop.
		large = 0;													//'large' is set to 0 so that any value to start will be larger.
		
		//For loop that goes through memory units.
		for(j = 0; j < MEMORYAVAIL; j++) {
			
			if(mUnits[j] == 0) {									//If current unit is 0, increment 'count'.
				count++;
			}
			
			if(mUnits[j] != 0) {									//If current unit is not 0, compare our 'count'.
				
				if(count >= stream[i].size && count > large) {		//If 'count' is big enough to fit task and is larger than current 'large', make it the new 'large' and set 'index' to beginning of where we are placing task.
					large = count;
					index = j - count;
				}
				count = 0;											//Reset 'count' to 0.
			}
			
			if(j == (MEMORYAVAIL - 1)) {							//If current unit is at the end of the memory units array, compare our 'count'.
				
				if(count >= stream[i].size && count > large) {		//If 'count' is big enough to fit task and is larger than current 'large', make it the new 'large' and set 'index' to beginning of where we are placing task.
					large = count;
					index = j - count + 1;
				}
				count = 0;											//Reset 'count' to 0.
			}
		}
		
		if(index != -1) {											//If 'index' is not -1 then we have placed the task.
			
			//For loop that goes from 'index' and checks as many units as our task size. Assigns task time to them.
			for(j = index; j < (stream[i].size + index); j++) {
				mUnits[j] = stream[i].time;
			}
		}
		else {														//If 'index' is -1 then we have not placed the task.
			
			//For loop that goes through the memory units and decrements all of those over 0.
			for(j = 0; j < MEMORYAVAIL; j++) {
				
				if(mUnits[j] > 0) {
					mUnits[j] = mUnits[j] - 1;
				}
			}
			totalTime++;											//Increment 'totalTime' since we couldn't place task.
			i--;													//Decrement 'i' so we go through this task again.
		}
	}
	
	int largest = 0;												//'largest' holds value of highest remaining time left in the memory.
	//For loop goes through the memory, checking all of the time.
	for(i = 0; i < MEMORYAVAIL; i++) {
		if(mUnits[i] > largest) {
			largest = mUnits[i];
		}
	}
	totalTime += largest;											//Adds largest amount of time left to the 'totalTime'.
	
	return totalTime;
}

/***************************************************************************
* int nextFit(struct data stream)
* Author: David Garvin
* Date: 4/19/2020
* Description: Finds the first available space in the memory, while searching
*			from where the last task was placed, for the next task in the stream
*
* Parameters:
* 	stream		I/P		struct data		Stream of data with a size and a time integer values.
* 	nextFit		O/P 	int			 	Total time transpired to go through the stream.
**************************************************************************/
int nextFit(struct data stream[]) {
	int i, j, index, count;											//'i' and 'j' are indexes. 'index' holds starting index for where we're putting the task. 'count' is our count of how many 0's in a row.
	int checkCounter, pointer = 0, totalTime = 0;					//'checkCounter' keeps count of how many loops we have, since we can start and end in the middle of the array. 'pointer' points to end of where we put last task. 'totalTime' is time transpired.
	int mUnits[MEMORYAVAIL];										//Memory units. The amount is set by the global variable MEMORYAVAIL.
	
	//For loop initializes memory units.
	for(i = 0; i < MEMORYAVAIL; i++) {
		mUnits[i] = 0;
	}
	
	//For loop that goes through all the tasks in the stream.
	for(i = 0; i < STREAMSIZE; i++) {
		index = -1;													//'index' is set to -1 so that we know if we haven't placed a task.
		count = 0;													//'count' is set to 0 to start the For loop.
		checkCounter = 0;											//'checkCounter' is set to 0 to start the For loop.
		
		//For loop that goes through memory units. Starts from pointer and loops around the start of the array by using 'checkCounter'.
		for(j = pointer; checkCounter < MEMORYAVAIL; j++) {
			
			if(mUnits[j] == 0) {									//If current unit is 0, increment 'count'.
				count++;
			}
			
			if(mUnits[j] != 0) {									//If current unit is not 0, compare our 'count'.
				
				if(count >= stream[i].size) {						//If 'count' is big enough to fit task, set 'index' to beginning of where we are placing task, set 'pointer' to the end of the task we are placing, and break out of For loop.
					index = j - count;
					pointer = index + stream[i].size;
					break;
				}
				count = 0;											//Reset 'count' to 0.
			}
			
			if(j == (MEMORYAVAIL - 1)) {							//If current unit is at the end of the memory units array, compare our 'count'.
				
				if(count >= stream[i].size) {						//If 'count' is big enough to fit task, set 'index' to beginning of where we are placing task, set 'pointer' to the end of the task we are placing, and break out of For loop.
					index = j - count + 1;
					pointer = index + stream[i].size;
					if(pointer >= MEMORYAVAIL) {					//If 'pointer' has reached the end or beyond of memory, set it back to 0.
						pointer = 0;
					}
					break;
				}
				count = 0;											//Reset 'count' to 0.
				j = -1;												//Reset 'j' to -1 so we can start at the beginning of the array.
			}
			
			if(j == (pointer - 1)) {								//If we have looped around and ended at the pointer.
				
				if(count >= stream[i].size) {						//If 'count' is big enough to fit task, set 'index' to beginning of where we are placing task, set 'pointer' to the end of the task we are placing, and break out of For loop.
					index = j - count + 1;
					pointer = index + stream[i].size;
					break;
				}
			}
			
			checkCounter++;											//Increment 'checkCounter' so that we know when we have made one pass through.
		}
		
		if(index != -1) {											//If 'index' is not -1 then we have placed the task.
			
			//For loop that goes from 'index' and checks as many units as our task size. Assigns task time to them.
			for(j = index; j < (stream[i].size + index); j++) {
				mUnits[j] = stream[i].time;
			}
		}
		else {														//If 'index' is -1 then we have not placed the task.
			
			//For loop that goes through the memory units and decrements all of those over 0.
			for(j = 0; j < MEMORYAVAIL; j++) {
				
				if(mUnits[j] > 0) {
					mUnits[j] = mUnits[j] - 1;
				}
			}
			totalTime++;											//Increment 'totalTime' since we couldn't place task.
			i--;													//Decrement 'i' so we go through this task again.
		}
	}

	int largest = 0;												//'largest' holds value of highest remaining time left in the memory.
	//For loop goes through the memory, checking all of the time.
	for(i = 0; i < MEMORYAVAIL; i++) {
		if(mUnits[i] > largest) {
			largest = mUnits[i];
		}
	}
	totalTime += largest;											//Adds largest amount of time left to the 'totalTime'.
	
	return totalTime;
}

/***************************************************************************
* int main(int argc, char *argv[])
* Author: David Garvin
* Date: 8 March 100100
* Description: Reads integers representing processes and resources from files,
*				writes them to arrays, and then determines if a deadlock
*					would happen or not.
*
* Parameters:
* 	argc	I/P		int				Number of arguments on the command line.
* 	argv	I/P 	char *[] 		Arguments on the command line.
*										Filenames that should be given in the order
*											of Q matrix, A matrix, and Available vector.
* 	main	O/P 	int				Status code (not currently used).
**************************************************************************/
int main(int argc, char *argv[]) {
	int i, j, time;														//'i' and 'j' are indexes. 'time' temporarily holds current time passed from functions.
	int fFitTime = 0, bFitTime = 0, wFitTime = 0, nFitTime = 0;			//These are the total times from each time we've ran the function. They will be used for average.
	int fFitLo = 10000, bFitLo = 10000, wFitLo = 10000, nFitLo = 10000;	//These hold the lowest times from each of the algorithms. Set to 10000 since our values will be in the 1000-1500 range.
	int fFitHi = 0, bFitHi = 0, wFitHi = 0, nFitHi = 0;					//These hold the highest times from each of the algorithms. Set to 0.
	struct data stream[STREAMSIZE];										//An array of strutures containing 'time' and 'size' integers.
	
	//For loop runs 1000 experiments.
	for(i = 0; i < 1000; i++) {
		
		//For loop sets a new stream with values between 1-16 for time and size.
		for(j = 0; j < STREAMSIZE; j++) {
			stream[j].size = normal(4, 4);
			
			if(stream[j].size < 1) {
				stream[j].size = 1;
			}
			
			stream[j].time = normal(4, 4);
			
			if(stream[j].time < 1) {
				stream[j].time = 1;
			}
		}
		
		time = firstFit(stream);										//This block gets our lowest, highest, and total time for First Fit.
		if(time > fFitHi) {
			fFitHi = time;
		}
		if(time < fFitLo) {
			fFitLo = time;
		}
		fFitTime += time;
		
		time = bestFit(stream);											//This block gets our lowest, highest, and total time for Best Fit.
		if(time > bFitHi) {
			bFitHi = time;
		}
		if(time < bFitLo) {
			bFitLo = time;
		}
		bFitTime += time;
		
		time = worstFit(stream);										//This block gets our lowest, highest, and total time for Worst Fit.
		if(time > wFitHi) {
			wFitHi = time;
		}
		if(time < wFitLo) {
			wFitLo = time;
		}
		wFitTime += time;
		
		time = nextFit(stream);											//This block gets our lowest, highest, and total time for Next Fit.
		if(time > nFitHi) {
			nFitHi = time;
		}
		if(time < nFitLo) {
			nFitLo = time;
		}
		nFitTime += time;
	}
	
	printf("\nFirst Fit:\tRange is %d to %d,\tAverage is %d\n", fFitLo, fFitHi, (fFitTime / 1000));	//This block prints out our ranges and our average times.	
	printf("Best Fit:\tRange is %d to %d,\tAverage is %d\n", bFitLo, bFitHi, (bFitTime / 1000));
	printf("Worst Fit:\tRange is %d to %d,\tAverage is %d\n", wFitLo, wFitHi, (wFitTime / 1000));
	printf("Next Fit:\tRange is %d to %d,\tAverage is %d\n", nFitLo, nFitHi, (nFitTime / 1000));
}