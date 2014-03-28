/*
 * File: forker.c
 * Author: Leslie Minor
 * Project: CSCI 3753 Programming Assignment 3
 * Create Date: 2014/03/28
 * Description:
 * 	This file contains a program for forking and running the provided 
 *  programs, for the purpose of generating required data for assignment.
 * Usage:
 * ./forker schedulingPolicy numberOfChildren CPULoadType
 */

 //usage of other files (we'll just use defaults)
 // rw.c - bytestocopy blocksize inputfile outputfile (adds to output)
 // pi-sched.c - iterations schedulingpolicy

int main(int argc, char* argv[]){
	int policy;
	int children;
	int i;
	pid_t pid;
	// Validate input, because apparently I don't trust myself. Dark times.
	if(argc != 4){
		fprintf(stderr, "Correct usage: schedulingPolicy 
				numberOfChildren CPULoadType\n");
		exit(EXIT_FAILURE);
	}

	if(!strcmp(argv[1], "SCHED_OTHER")){
    	policy = SCHED_OTHER;
	}
	else if(!strcmp(argv[1], "SCHED_FIFO")){
    	policy = SCHED_FIFO;
	}
	else if(!strcmp(argv[1], "SCHED_RR")){
    	policy = SCHED_RR;
	}
	else{
    	fprintf(stderr, "Unhandeled scheduling policy, %s\n", argv[1]);
    	exit(EXIT_FAILURE);
	}

	children = atoi(argv[2]);
	if(children < 1){
		fprintf(stderr, "Invalid number of children, %d\n", children);
		exit(EXIT_FAILURE);
	}

	if(strcmp(argv[3], "pi")){
		if(strcmp(argv[3], "rw")){
			if(strcmp(argv[3], "mix")){
				fprintf(stderr, "Invalid program name, %s\n", arvg[3]);
				exit(EXIT_FAILURE);
			}
		}
	}

	//set scheduling policy

	/* Set process to max priority for given scheduler */
    param.sched_priority = sched_get_priority_max(policy);
    
    /* Set new scheduler policy */
    fprintf(stdout, "Current Scheduling Policy: %d\n", sched_getscheduler(0));
    fprintf(stdout, "Setting Scheduling Policy to: %d\n", policy);
    if(sched_setscheduler(0, policy, &param)){
		perror("Error setting scheduler policy");
		exit(EXIT_FAILURE);
    }
    fprintf(stdout, "New Scheduling Policy: %d\n", sched_getscheduler(0));
	
	//fork the children, in the eye
	for(i=0; i < children; ++i){
		if((pid = fork()) == -1){
			fprintf(stderr, "Forking failed\n");
			exit(EXIT_FAILURE);
		}
		//run the program specified 
		//(with default arguments - this should be ok)
		if(0 == pid){
			execl(argv[3], argv[3], NULL);
			exit(EXIT_SUCCESS);
		}
		//parent keep on keepin on
	}


	//wait for children
	for(i=0; i < children; ++i){
		pid = wait(NULL);
		if(pid == -1)
			fprintf(stderr, "Wait error \n");
	}

	exit(EXIT_SUCCESS);
}

