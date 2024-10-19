/***************************************************************************
* File: main.c
* Author: David Garvin
* Procedures:
* main - reads integers representing processes and resources,
*			writes them to arrays, and then determines if a deadlock
*				would happen or not
*				
* checkChar - depending on character in the input will output an integer
*				so that the array knows what to do
* compW - compares a row from Q and the vector from W to see if the row from
*			Q is smaller than or equal to the vector from W
***************************************************************************/

#include <stdio.h>

/***************************************************************************
* int checkChar(char c)
* Author: David Garvin
* Date: 8 March 100100
* Description: Checks what character has been given to it from fgetc().
*				If 0 or 1, return integer of equal value. If \n, return
*					-1 so that the program knows to go to the next line.
*						If -2, end of file and the while loop in main() can end.
*							If -3, this is a character we do not care about
*								(whitespace) and we will ignore it in main().
*
* Parameters:
* 	c			I/P		char			Character to check
* 	checkChar	O/P 	int			 	Int 0 or 1 for char 0 or 1.
*										Int -1 for char \n. Int -2 for
*										char EOF (end of file). Int -3
*										for any other char.
**************************************************************************/

int checkChar(char c) {
	if(c == '0') {
		return 0;
	}
	if(c == '1') {
		return 1;
	}
	if(c == '\n') {
		return -1;
	}
	if(c == EOF) {
		return -2;
	}
	
	return -3;
}

/***************************************************************************
* int compW(int W[], int Q[100][100], int Qcol, int Qrow)
* Author: David Garvin
* Date: 8 March 100100
* Description: Compares vector W to a row from matrix Q. If row is greater than
*				vector, return -1. Otherwise return 0.
*
* Parameters:
* 	W		I/P		int[]			Vector W.
* 	Q		I/P 	int[100][100] 	Matrix Q.
* 	maxCol	I/P 	int				Amount of columns actually used in matrix.
*	Qrow	I/P		int				The row number in the matrix to compare.
*	compW	O/P		int				-1 if row in Q matrix is greater than
*										vector W.
**************************************************************************/

int compW(int W[], int Q[100][100], int maxCol, int Qrow) {
	int i;
	
	for(i = 0; i < maxCol; i++) {	//If any column of row from matrix Q is larger than the same column from vector W, return -1.
		if (Q[i][Qrow] > W[i]) {
			return -1;
		}
	}
	
	return 0;						//If no column of row from matrix Q is larger then return 0.
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
	FILE *fptr;						//File pointer for reading from files.
	int i = 0, j = 0, temp;			//Value that will be used throughout the code. i and j for arrays and temp to hold values temporarily.
	int maxCol = 0, maxRow = 0;		//After we go through the first array we will know the max rows and columns.
	
	char *qfile, *afile, *availVecfile; //Holds the names of the files for input.
	
	//If we are given 3 file names, use them. If not, set them to default names.
	if(argc != 4) {
		qfile = "q.txt";
		afile = "a.txt";
		availVecfile = "availvec.txt";
	}
	else {
		qfile = argv[1];
		afile = argv[2];
		availVecfile = argv[3];
	}
	
	//The following block of code reads the Request matrix Q and writes to Q array.
	//In addition, while we are guessing at an array size of 100x100 here, this first array will give us the size for future arrays.
	int q[100][100];
	if((fptr = fopen(qfile, "r")) == NULL) {		//If file open fails, give error and shut program down.
		printf("Error opening Q file.");
		return 0;
	}
	while(temp != -2) {								//While loop while there is something left in the file. Value from checkChar function.
		temp = checkChar(fgetc(fptr));				//Use checkChar function to return integer value.
		
		if(temp == -1) {							//If next value is NEXTLINE then adjust indexes accordingly.
			i = 0;
			j++;
		}
		if(temp == 0 || temp == 1) {				//If next value is 0 or 1 then assign to current index in array.
			q[i][j] = temp;
			i++;
		}
	}
	maxCol = i;										//Assign maximum Columns and Rows depending on result from first array.
	maxRow = j + 1;									//		Bring reused variables back to 0 and close file.
	temp = 0;
	i = 0;
	j = 0;
	fclose(fptr);
	
	//The following block of code reads the Allocation matrix A and writes to A array.
	int a[maxCol][maxRow];							//Makes matrix size match Q.
	if((fptr = fopen(afile, "r")) == NULL) {		//If file open fails, give error and shut program down.
		printf("Error opening A file.");
		return 0;
	}
	while(temp != -2) {								//While loop while there is something left in the file. Value from checkChar function.
		temp = checkChar(fgetc(fptr));				//Use checkChar function to return integer value.
		
		if(temp == -1) {							//If next value is NEXTLINE then adjust indexes accordingly.
			i = 0;
			j++;
		}
		if(temp == 0 || temp == 1) {				//If next value is 0 or 1 then assign to current index in array.
			a[i][j] = temp;
			i++;
		}
	}
	temp = 0;										//Bring reused variables back to 0 and close file.
	i = 0;
	j = 0;
	fclose(fptr);
	
	//The following block of code reads the Available vector and writes to availVec array.
	int availVec[maxCol];								//Makes array match width of matrices Q and A.
	if((fptr = fopen(availVecfile, "r")) == NULL) {		//If file open fails, give error and shut program down.
		printf("Error opening Available Vector file.");
		return 0;
	}
	while(temp != -2) {									//While loop while there is something left in the file. Value from checkChar function.
		temp = checkChar(fgetc(fptr));					//Use checkChar function to return integer value.

		if(temp == 0 || temp == 1) {					//If next value is 0 or 1 then assign to current index in array.
			availVec[i] = temp;
			i++;
		}
	}
	fclose(fptr);										//Close file.
	
	//The following block of code adds the integers from each column in the A array and the same column from the availVec array to get our resource vector values.
	int resVec[maxCol];
	for(i = 0; i < maxCol; i++) {
		resVec[i] = 0;
		for(j = 0; j < maxRow; j++) {
			resVec[i] = resVec[i] + a[i][j];
		}
		resVec[i] = resVec[i] + availVec[i];
	}
	
	//The following block of code prints out our initial values to show the user that everything is being read correctly.
	printf("\nRequest matrix Q\n");
	for(j = 0; j < maxRow; j++) {
		for(i = 0; i < maxCol; i++) {
			printf("%d", q[i][j]);
		}
		printf("\n");
	}
	printf("\nAllocation matrix A\n");
	for(j = 0; j < maxRow; j++) {
		for(i = 0; i < maxCol; i++) {
			printf("%d", a[i][j]);
		}
		printf("\n");
	}
	printf("\nAvailable vector\n");
	for(i = 0; i < maxCol; i++) {
		printf("%d", availVec[i]);
	}
	printf("\n\nResource vector\n");
	for(i = 0; i < maxCol; i++) {
		printf("%d", resVec[i]);
	}
	printf("\n\n");
	
	
	int marked[maxRow];			//Array to keep up with which rows are marked. 1 = marked, 0 = unmarked.
	
	//Marks all rows in A matrix with all 0's. Temp variable is reused for this.
	for(j = 0; j < maxRow; j++) {
		temp = 0;
		for(i = 0; i < maxCol; i++) {
			temp = temp + a[i][j];
		}
		if(temp == 0) {
			marked[j] = 1;
		}
		else {
			marked[j] = 0;
		}
	}
	//Print out results after checking for A matrix rows that have all 0's. For user's benefit.
	printf("\nChecking rows for all 0's.\n");
	for(j = 0; j < maxRow; j++) {
		printf("P%d is ", j + 1);
		if(marked[j] == 1) {
			printf("marked.\n");
		}
		else {
			printf("unmarked.\n");
		}
	}
	
	//Copy values of Available vector to vector W.
	int w[maxCol];
	for(i = 0; i < maxCol; i++) {
		w[i] = availVec[i];
	}
	
	//Checks the Q matrix from the bottom up and compares the values found on each row to the values from vector W.
	j = maxRow;
	while(j > -1) {								//Will run until nothing stops it and index j tries to go into negative values.
		if(marked[j] == 1) {					//If row is already marked, move on.
			j--;
		}
		else if(compW(w, q, maxCol, j) == -1) {	//If row from Q is greater than vector W, move on.
			j--;
		}
		else {									//If row is unmarked and the row from Q is lesser than or equal to vector W,
			marked[j] = 1;						//	mark that row and add it to vector W.
			
			for(i = 0; i < maxCol; i++) {		//Adds row from Q to vector W by ORing each bit.
				w[i] = w[i] | q[i][j];
			}
			
			j = maxRow;							//After changing vector W we need to restart from the bottom.
		}
	}
	
	//Prints the final results. In other words, which Process is marked and unmarked.
	printf("\nFinal results.\n");
	for(j = 0; j < maxRow; j++) {
		printf("P%d is ", j + 1);
		if(marked[j] == 1) {
			printf("marked.\n");
		}
		else {
			printf("unmarked.\n");
		}
	}
	
	//If any rows are left unmarked then we have a deadlock and we print such. Otherwise, we tell the user that there is no deadlock.
	int isDeadlocked = 0;
	for(j = 0; j < maxRow; j++) {
		if(marked[j] == 0) {
			isDeadlocked = isDeadlocked + 1;
		}
	}
	if(isDeadlocked != 0) {
		printf("\n\nWe are deadlocked!\n\n");
	}
	else {
		printf("\n\nNo deadlock!\n\n");
	}
	
	return 0;									//End program.
}