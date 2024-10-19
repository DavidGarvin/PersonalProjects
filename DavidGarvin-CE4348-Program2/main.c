/***************************************************************************
* File: main.c
* Author: David Garvin
* Procedures:
* main - creates five child processes, creates message queue, calls
*			functions to send/receive messages, keeps count and declares
*				losers and the winner
*				
* parentProcess - if the queue is empty, sends a message
* childProcess - if the queue has something in it, receives a message
***************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>		//For O_* constants
#include <sys/stat.h>	//For mode constants
#include <sys/wait.h>
#include <sys/types.h>

struct mq_attr attr;		//Initialize message gueue attributes structure.

/***************************************************************************
* int parentProcess(mqd_t mq, struct mq_attr attr)
* Author: David Garvin
* Date: 1 March 2020
* Description: Checks if queue is empty. If so, sends a message
*				into the queue. If message is sent, returns 0. Otherwise
*					returns -1.
*
* Parameters:
* 	mq		I/P		mqd_t			Message queue description
* 	attr	I/P 	struct mq_attr 	Message queue attributes
* 	main	O/P 	int				Status code (0 if successful, -1 if not)
**************************************************************************/

int parentProcess(mqd_t mq, struct mq_attr attr) {
	mq_getattr(mq, &attr);		//Get attributes.
	if(attr.mq_curmsgs == 0) {	//If message gueue is empty, send message and return 0. Else return -1.
		mq_send(mq, "", 0, 0);
		return 0;
	}
	return -1;
}

/***************************************************************************
* int childProcess(mqd_t mq, struct mq_attr attr)
* Author: David Garvin
* Date: 1 March 2020
* Description: Checks if queue has a message. If so, receives a message
*				from the queue. If message is received, returns 0. Otherwise
*					returns -1.
*
* Parameters:
* 	mq		I/P		mqd_t			Message queue description
* 	attr	I/P 	struct mq_attr 	Message queue attributes
* 	main	O/P 	int				Status code (0 if successful, -1 if not)
**************************************************************************/

int childProcess(mqd_t mq, struct mq_attr attr) {
	char * buffer;						//Set buffer. Won't be used.
	mq_getattr(mq, &attr);				//Get attributes.
	
	if(attr.mq_curmsgs != 0) {			//If message queue is not empty, receive message and return 0. Else return -1.
		mq_receive(mq, buffer, 1, NULL);
		return 0;
	}
	return -1;
}

/***************************************************************************
* int main(int argc, char *argv[])
* Author: David Garvin
* Date: 1 March 2020
* Description: creates five child processes, creates message queue, calls
*				functions to send/receive messages, keeps count and declares
*					losers and the winner.
*
* Parameters:
* argc		I/P		int				The number of arguments on the command line
* argv		I/P		char *[]		The arguments on the command line
* main		O/P		int				Status code (not currently used)
**************************************************************************/

int main(int argc, char *argv[]) {
	mqd_t mq;								//Message queue description.
	pid_t p = 1;							//PID. Initialized as 1 so the for loop later knows it's the parent.
	volatile int count = 0;					//Count for the children to keep count of messages received.
	
	attr.mq_maxmsg = 10;					//Message queue attributes set. Max messages = 10, message size = 1, no change to flag, start current message count at 0.
	attr.mq_msgsize = 1;
	attr.mq_flags = 0;
	attr.mq_curmsgs = 0;
	
	mq = mq_open("/dlg170430",				//Create new message queue. 0777 allows everything to read, write, and execute.
		O_RDWR | O_CREAT, 0777, &attr);
	
	printf("FROM LOSER TO WINNER\n\n");		//Message to set up output before the fork().
	
	
	int i;
	for(i = 0; i < 5; i++) {				//Fork() 5 children processes. If process is a child, no fork().
		if(p != 0) {
			p = fork();
		}
	}

	
	if(p != 0) {							//If parent process, set messages to 0 and keep sending until we reach 5000.
		int messages = 0;
		while(messages < 5000) {
			if(parentProcess(mq, attr) == 0) {
				messages++;
			}
		}
	}
	else {									//Else child process, increment count if receives message.
		while(count < 1000) {
			if(childProcess(mq, attr) == 0) {
				count++;
			}
		}
	}
	
	
	sleep(1);								//Sleep for one second. Helps lower possible errors.
	if(p != 0) {							//If parent process, delete message queue.
		mq_unlink("/dlg170430");
	}
	else {									//Else child process. Sleep for one second. Print PID. Close link to message queue. Exit child process.
		sleep(1);
		printf("%d\n", getpid());
		mq_close(mq);
		_exit(0);
	}
	sleep(3);								//Sleep for three seconds. Helps lower possible errors.
	
	wait(NULL);								//Waits for children processes to finish.
	return 0;								//End program.
}