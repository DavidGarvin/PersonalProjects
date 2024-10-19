/*****************************************************************************
 * File: main.c
 * Author: David Garvin
 * Procedures:
 * main     -check the time before and after a fork() function
 *              and see how long the process will take,
 *                  this will print out all the differences
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

/*****************************************************************************
 * int main()
 * Author: David Garvin
 * Date 2 February 2020
 * Description: Checks the time before and after a fork()
 *              function and sees how long the process takes.
 *              It then prints all the differences.
 * 
 * Parameters:
 *      main    O/P int     Status code (not currently used)
 *****************************************************************************/

int main() {
    struct timeval startTV, endTV;      //Start and end timevalues.
    long startTime, endTime, totalTime; //Long variable for start and end, then the difference.
    int i, status;                      //Int variable for index i and status.
    pid_t pid;                          //Holds PID value.

    for(i = 0; i < 1050; i++) {
        gettimeofday(&startTV, NULL);   //This line and the following line gets us the value for start time in microseconds.
        startTime = startTV.tv_usec;
        pid = fork();                   //Forks and gets PID value.
        if(pid == 0) {                  //If child, exit.
            _exit(1);
        }
        pid = wait(&status);
        gettimeofday(&endTV, NULL);     //This line and the following line gets us the value for end time in microseconds.
        endTime = endTV.tv_usec;
        totalTime = endTime - startTime;//Difference between start and end times.
        
        printf("%lu\n", totalTime);     //Prints difference.

        sleep(0.01);                    //A short sleep for stability.
    }
    
    return 0;
}