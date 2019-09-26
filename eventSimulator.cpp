//Aryanna Anderson
#include <queue>
#include <iostream>
#include <string>
#include <deque>
#include <list>
#include <random>
#include <ctime>
#include <cstdlib>
#include <cstddef>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;




	//enumeration that holds the states of the events, so I can access them by number
	typedef enum {Job_Arrival, CPU_Enter, CPU_Finish, Disk1_Enter, Disk1_Finish, Disk2_Enter, Disk2_Finish,Job_Exit, Simulation_End} eventType;


	//struct for process. each process contains the time at which they occur, their state, and their job number
	struct process{
    	int time;
	eventType state;
	int id;
	};


	//struct that I use to compare each process with one another so I can organize them in the priority queue
	//If a process has a lower time it has a higher priority. If they have same time, the one with lower job number goes first, and if they have same job number the one with lower state goes first
	struct compare{
    	bool operator()(const process & a, const process & b){
	if(a.time==b.time){
		return a.state>b.state;
        }else if(a.time==b.time && a.state==b.state){
		return a.id>b.id;
	}else{
            	return a.time>b.time;
            	}
	}
    	};


	//prototypes for event state handling functions
	void processArrival(int min, int max);
	int cpuEnter(int min, int max); //returns an integer which is the time it takes for the process to use the CPU. Need this value for statistical results
	void cpuFinish(int prob);
	int disk1Enter(int min, int max); //returns an integer which is the time it takes for the process to use disk 1. Need this value for statistical results
	void disk1Finish();
	int disk2Enter(int min, int max); //returns an integer which is the time it takes for the process to use disk 2. Need this value for statistical results
	void disk2Finish();
	void simulationEnd();

	//prototypes for random number generator and probability select functions
	int random_gen(int min, int max);
	bool prob_select(int percent);
	void jobExit();


	//declarations for queues. One is a priority queue for the processes, while the other three are FIFO queues for each separate component
	//made global so all handler functions can use them
	std::priority_queue<process,vector<process>,compare> eventQueue;
	std::queue<process> cpuQueue;
	std::queue<process> disk1Queue;
	std::queue<process> disk2Queue;

	//global variables for state of components. Boolean used to tell if the component is idle or busy
	//made global so all handler functions can use them
	bool disk1IsBusy=false;
	bool disk2IsBusy=false;
	bool CPUIsBusy=false;


	int main(){
	//reading variables from config.txt
	ifstream inputFile("config.txt"); //opening config file
	int i=0; //variable used to keep track of variables by number
	int array[12]; //array in which I put the values of each variable
	int value;
	string constant;
	//reading through config file and putting values in array
	while (inputFile >> constant >> value){
	array[i]=value;
	i++;
	}
	//taking array values and placing them with their variables
	int SEED=array[0];
    	int INIT_TIME=array[1];
    	int FIN_TIME=array[2];
    	int ARRIVE_MIN=array[3];
    	int ARRIVE_MAX=array[4];
    	int QUIT_PROB=array[5];
	int CPU_MIN=array[6];
    	int CPU_MAX=array[7];
    	int DISK1_MIN=array[8];
    	int DISK1_MAX=array[9];
	int DISK2_MIN=array[10];
	int DISK2_MAX=array[11];

	//opening log.txt file to write to it
	ofstream myFile;
	myFile.open("log.txt");
	//writing values from config file to log file
	myFile<<"the SEED is "<<SEED<<endl;
	myFile<<"the intial start of simulation time is "<<INIT_TIME<<endl;
	myFile<<"the finish time for simulation is "<<FIN_TIME<<endl;
	myFile<<"the minimum job arrivak time is "<<ARRIVE_MIN<<endl;
	myFile<<"the maximum job arrival time is "<<ARRIVE_MAX<<endl;
	myFile<<"the quit probability is "<<QUIT_PROB<<endl;
	myFile<<"the minimum CPU processing time is "<<CPU_MIN<<endl;
	myFile<<"the maximum CPU processing time is "<<CPU_MAX<<endl;
	myFile<<"the minimum disk 1 processing time is "<<DISK1_MIN<<endl;
	myFile<<"the maximum disk 1 processing time is "<<DISK1_MAX<<endl;
	myFile<<"the minimum disk 2 processing time is "<<DISK2_MIN<<endl;
	myFile<<"the maximum disk 2 processing time is "<<DISK2_MAX<<endl;

	//variables declarion for maximum queue sizes
	int maxEventQueueLength=0;
	int maxCPUQueueLength=0;
	int maxDisk1QueueLength=0;
	int maxDisk2QueueLength=0;

	//variables declaration for total queue size (which I use to help find average queue sizes)
	int totalEventQueueSize=0;
	int totalCPUQueueSize=0;
	int totalDisk1QueueSize=0;
	int totalDisk2QueueSize=0;

	//variables for maximum component time
	int maxCPUTime=0;
	int maxDisk1Time=0;
	int maxDisk2Time=0;

	//variables for total component time (which I use to help find average component time)
	double totalCPUTime=0.0;
	double totalDisk1Time=0.0;
	double totalDisk2Time=0.0;

	//variables to keep track of the number of jobs that go through each component
	int CPUJobs=0;
	int disk1Jobs=0;
	int disk2Jobs=0;

	//variables to keep track of how many cycles we make through while loop (which I use to help find average queue sizes)
	int cycles=0;

	//variables to help keep track of component time (which I use to help find average component time as well as maximum component time)
	int CPUTime=0;
	int disk1Time=0;
	int disk2Time=0;

	//variables for average component time
	double avgCPUTime;
	double avgDisk1Time;
	double avgDisk2Time;

	//variables for component utilization
	double CPUUtilization;
	double disk1Utilization;
	double disk2Utilization;

	//variables for average queue size
	double avgEventQueueSize=0;
	double avgCPUQueueSize=0;
	double avgDisk1QueueSize=0;
	double avgDisk2QueueSize=0;

	//variables for component throughput
	float CPUThroughput;
	float disk1Throughput;
	float disk2Throughput;

	//initializing srand with the seed from the config file
	srand(SEED);

	//declaring time as the IN_TIME also found in config file
	int time= INIT_TIME;

	//creating first job arrival process
	process first_job={time,Job_Arrival, 1};

	//creating process that ends simulation at FIN_TIME, which is also found from config file
    	process finish= {FIN_TIME,Simulation_End,0};

	//pushing both starting and finishing processes on to priority queue
    	eventQueue.push(first_job);
    	eventQueue.push(finish);

	//big while loop for simulation of system
	//while the priority queue isn't empty and the time is not up it will take job from top of priority queue and depending on the state of the process it will handle it
	while(!eventQueue.empty()&&(time < FIN_TIME)){
		cycles++; //incrementing cycles each time we go through while loop, to show how many times the while loop is used
    		process cur_job = eventQueue.top();
		myFile<<"at time "<<cur_job.time<<" job "<<cur_job.id<<" "; //writing to file the time and job number
        	switch (cur_job.state){ //switch case for each process state. Depending on state of process is what handler function the event will go to
                	case Job_Arrival: myFile<<"arrived."<<endl; processArrival(ARRIVE_MIN, ARRIVE_MAX);break;
                	case CPU_Enter: myFile<<"entered CPU."<<endl; CPUTime=cpuEnter(CPU_MIN,CPU_MAX); totalCPUTime+=CPUTime; break; //adds CPUTime which is time it takes for process to use the CPU to totalCPUTime
                	case CPU_Finish: myFile<<"finished at CPU."<<endl; cpuFinish(QUIT_PROB); CPUJobs++; break; //increments how many jobs have been processed by CPU
                	case Disk1_Enter: myFile<<"entered disk 1."<<endl; disk1Time=disk1Enter(DISK1_MIN,DISK1_MAX); totalDisk1Time+=disk1Time; break; //adds disk1Time, which is time it takes for process to use the disk to total disk1Time
                	case Disk1_Finish: myFile<<"finished at disk 1."<<endl; disk1Finish(); disk1Jobs++; break; //increments amount of jobs that have been processed by disk 1
			case Disk2_Enter: myFile<<"entered disk 2."<<endl; disk2Time=disk2Enter(DISK2_MIN,DISK2_MAX); totalDisk2Time+=disk2Time; break; //adds disk2Time, which is time it takes for process to use the disk to total disk2Time
			case Disk2_Finish: myFile<<"finished at disk 2."<<endl; disk2Finish(); disk2Jobs++; break; //increments amount of jobs that have been processed by disk 2
			case Job_Exit: myFile<<"exited program."<<endl; jobExit(); break;
                	case Simulation_End: myFile<<"ended simulation."<<endl;
		//closing log file
		myFile.close();
		//opening stat file to write statistics to it
		ofstream myFile;
        		myFile.open("stats.txt");
				//writing statistics, befor exiting out of program
				myFile<<"************Maximum queue sizes***************"<<endl;
				myFile<<"max event queue size is "<<maxEventQueueLength<<endl;
				myFile<<"max cpu queue size is "<<maxCPUQueueLength<<endl;
				myFile<<"max disk1 queue size is "<<maxDisk1QueueLength<<endl;
				myFile<<"max disk 2 queue size is "<<maxDisk2QueueLength<<endl;
				myFile<<"***************************************"<<endl;
				myFile<<"************Average queue sizes***************"<<endl;
				myFile<<"average event queue size is "<<avgEventQueueSize<<endl;
				myFile<<"average CPU queue size is "<<avgCPUQueueSize<<endl;
				myFile<<"average disk 1 queue size is "<<avgDisk1QueueSize<<endl;
				myFile<<"average disk 2 queue size is "<<avgDisk2QueueSize<<endl;
				myFile<<"***************************************"<<endl;
				myFile<<"************Maximum component response times***************"<<endl;
				myFile<<"max CPU response time is "<<maxCPUTime<<endl;
				myFile<<"max disk 1 response time is "<<maxDisk1Time<<endl;
				myFile<<"max disk 2 response time is "<<maxDisk2Time<<endl;
				myFile<<"***************************************"<<endl;
				myFile<<"************Average component response times***************"<<endl;
				myFile<<"average cpu response time is "<<avgCPUTime<<endl;
				myFile<<"average disk 1 response time is "<<avgDisk1Time<<endl;
				myFile<<"average disk 2 response time is "<<avgDisk2Time<<endl;
				myFile<<"***************************************"<<endl;
				myFile<<"************Component throughputs***************"<<endl;
				myFile<<"cpu throughput is "<<CPUThroughput<<" jobs per unit of time."<<endl;
				myFile<<"disk 1 throughput is "<<disk1Throughput<<" jobs per unit of time."<<endl;
				myFile<<"disk 2 throughput is "<<disk2Throughput<<" jobs per unit of time."<<endl;
				myFile<<"***************************************"<<endl;
				myFile<<"************Component Utilization***************"<<endl;
				myFile<<"CPU utilization is "<<CPUUtilization<<endl;
				myFile<<"disk 1 utilization is "<<disk1Utilization<<endl;
				myFile<<"disk 2 utilization is "<<disk2Utilization<<endl;
				//closing stat file
				myFile.close();
			simulationEnd(); break;
               }

	//code to find maximum component time
	//after every iteration through while loop it will compare component time with the maximum component time, and if it is bigger it will replace it
	if(CPUTime>maxCPUTime)
		maxCPUTime=CPUTime;
	if(disk2Time>maxDisk2Time)
		maxDisk2Time=disk2Time;
	if (disk1Time>maxDisk1Time)
		maxDisk1Time=disk1Time;

	//code to find average component time
	//takes total component time which keeps being updated as the while loop gets executed and divides it by the number of jobs the component processed
	avgCPUTime= (totalCPUTime/CPUJobs);
	avgDisk1Time= (totalDisk1Time/disk1Jobs);
	avgDisk2Time= (totalDisk2Time/disk2Jobs);

	//code to find utilization of component
	//takes total time the component was in use and divides it by total time of simulation
	CPUUtilization=totalCPUTime/FIN_TIME;
	disk1Utilization=totalDisk1Time/FIN_TIME;
	disk2Utilization=totalDisk2Time/FIN_TIME;

	//code to find the maximum queue size
	//after each iteratin through while loop it will compare queue size with maximum queue size, and if it is bigger it will replace it
	if(eventQueue.size()>maxEventQueueLength)
		maxEventQueueLength=eventQueue.size();
	if(cpuQueue.size()>maxCPUQueueLength)
		maxCPUQueueLength=cpuQueue.size();
	if(disk1Queue.size()>maxDisk1QueueLength)
		maxDisk1QueueLength=disk1Queue.size();
	if(disk2Queue.size()>maxDisk2QueueLength)
	maxDisk2QueueLength=disk2Queue.size();

	//adds queue size after each iteration through while loop to total queue size
	totalEventQueueSize+=eventQueue.size();
	totalCPUQueueSize+=cpuQueue.size();
	totalDisk1QueueSize+=disk1Queue.size();
	totalDisk2QueueSize+=disk2Queue.size();

	//code to find average queue size
	//takes total queue size and divides it by how many cycles that were made through while loop
	avgEventQueueSize= ((float)totalEventQueueSize/cycles);
	avgCPUQueueSize= ((float)totalCPUQueueSize/cycles);
	avgDisk1QueueSize= ((float)totalDisk1QueueSize/cycles);
	avgDisk2QueueSize= ((float)totalDisk2QueueSize/cycles);

	//code for component throughput
	//divides number of jobs the component process by the total time of the simulation
	CPUThroughput= ((float)CPUJobs/FIN_TIME);
	disk1Throughput= ((float)disk1Jobs/FIN_TIME);
	disk2Throughput= ((float)disk2Jobs/FIN_TIME);
        }
	}


	//random number generator
	//if min is greater than max switches the numbers
	//returns a random integer between the min and the max given
	int random_gen(int min,int max){
	if(max<min){
		int tmp=min;
		min=max;
		max=tmp;
		}
        return min + rand() % (max+1 - min);
    	}


	//probability select function
	//generates random number between 0 and 100 percent
	//if probability given  is less than or equal to that number returns true
	//otherwise returns false
    	bool prob_select(int percent){
	if(percent<0 ||percent>100){
		cout<<"error"<<endl;
		return -1;
	}else{
    	int tmp=random_gen(0,100);
    	if (tmp<=percent)
		return true;
    	else
        	return false;
		}
    	}


	//handler function for a process arriving to system
	//event is taken off the top of the queue
	//checks cpu. if it is idle and the queue is empty a new cpu enter process is created with same time and job number and pushed on to priority queue
	//if either of those things are false the current job is pushed on to the cpu queue
	//next a new job arrival process must be created
	//a random number between arrive min and max is generated to describe when this process will arrive
	//this number is added to the current time of process
	//a new job arrival process is created with this new time and an incremented job number
	//this process is pushed on to the priority queue
	void processArrival(int min, int max){
	process cur_job=eventQueue.top();
	int i=eventQueue.top().id;
	eventQueue.pop();
	if(CPUIsBusy==false && cpuQueue.empty()){
		process new_job={cur_job.time,CPU_Enter,i};
		eventQueue.push(new_job);
	}else if(CPUIsBusy==true || !cpuQueue.empty()){
		cpuQueue.push(cur_job);
	}
   	int arrival_time=random_gen(min, max);
    	int time= cur_job.time+arrival_time;
    	process job_arrival={time, Job_Arrival, ++i};
   	eventQueue.push(job_arrival);
    	}


	//handler function for process entering cpu
	//the event is taken off the top of the priority queue
	//the cpu boolean is changed to busy
	//a random time between cpu min and max is generated to describe how long the process used the cpu
	//this time is added to current time of the process
	//a new cpu finished process is created with this new time and same job number
	//this process is pushed on to the event queue
	//the number describing how long processed used cpu is returned to main for statistical purposes
	int cpuEnter(int min, int max){
	process cur_job=eventQueue.top();
	eventQueue.pop();
	CPUIsBusy=true;
	int finish_time= random_gen(min, max);
	int new_time= cur_job.time +finish_time;
	process new_job={new_time, CPU_Finish, cur_job.id};
	eventQueue.push(new_job);
	return finish_time;
	}


	//handler function for process finishing using the CPU
	//the event is taken off the top of the priority queue
	// the cpu boolean is changed to idle
	//the prob_select function is used to determine if job will exit system or go to disk
	//if the function returns true a new job_exit process is created with same time and job number and is pushed on to priority queue
	//if function returns false we must figure out what disk to send it to
	//first check disk 1. if it is not busy and the queue is empty create a new disk 1 enter process with same time and number and push on to priority queue
	//if either of those things aren't true check disk 2. if disk 2 is not busy and queue is empty create a new disk 2 enter process and push on to priority queue
	//if both of those are false must push current process on to one of the queues(shortest one)
	//compare queue sizes. Whichever queue is shortest is where job is pushed on to
	//if they are equal use prob_select function to randomly decide which queue to put it on
	//next must put a new job on to the cpu
	//if the cpu queue is not empty, take job off front of queue and create a new cpu enter process with same time and number and push on to priority queue
	void cpuFinish(int QUIT_PROB){
	process cur_job=eventQueue.top();
	eventQueue.pop();
	CPUIsBusy=false;
	if(prob_select(QUIT_PROB)==true){
		process new_job={cur_job.time,Job_Exit, cur_job.id};
		eventQueue.push(new_job);
	}else if(disk1IsBusy==false && disk1Queue.empty()){
		process new_job={cur_job.time,Disk1_Enter,cur_job.id};
		eventQueue.push(new_job);
	}else if(disk2IsBusy==false && disk2Queue.empty()){
		process new_job={cur_job.time,Disk2_Enter,cur_job.id};
		eventQueue.push(new_job);
	}else if (disk1Queue.size()<disk2Queue.size()){
		disk1Queue.push(cur_job);
	}else if(disk1Queue.size()>disk2Queue.size()){
		disk2Queue.push(cur_job);
	}else if(prob_select(50)==true){
		disk1Queue.push(cur_job);
	}else{
		disk2Queue.push(cur_job);
	}
	if(!cpuQueue.empty()){
		process job=cpuQueue.front();
		cpuQueue.pop();
		process new_job={cur_job.time,CPU_Enter,job.id};
		eventQueue.push(new_job);
		}
	}


	//handler function for process entering disk 1
	//the process is taken off the top of the priority queue
	//the disk 1 boolean is changed to busy
	//a random time between the disk 1 min and max is generated to describe how long process used the disk
	//this time is added to the current time of the process
	//a new disk 1 finished process is created with this new time and same job number
	//this process is pushed on to the priority queue
	//the number describing how long the process used disk 1 is returned to main, for statistical purposes
  	int disk1Enter(int min, int max){
	process cur_job=eventQueue.top();
	eventQueue.pop();
	disk1IsBusy=true;
	int finish_time= random_gen(min, max);
	int new_time= cur_job.time+finish_time;
	process new_job= {new_time, Disk1_Finish,cur_job.id};
	eventQueue.push(new_job);
	return finish_time;
	}


	//handler function for process entering disk 2
	//the process is taken off the top of the priority queue
	//the disk 2 boolean is changed to busy
	//a random time between the disk 2 min and max is generated to describe how long process used the disk
	//this time is added to the current time of the process
	//a new disk 2 finished process is created with this new time and same job number
	//this process is pushed on to the priority queue
	//the number describing how long the process used disk 2 is returned to main, for statistical purposes
 	int disk2Enter(int min, int max){
	process cur_job=eventQueue.top();
	eventQueue.pop();
	disk2IsBusy=true;
	int finish_time= random_gen(min, max);
	int new_time=cur_job.time+finish_time;
	process new_job= {new_time, Disk2_Finish,cur_job.id};
	eventQueue.push(new_job);
	return finish_time;
	}


	//handler function for process finishing at disk 1
	//the process is taken off the top of the priority queue
	//the disk 1 boolean is changed to idle
	//the cpu component is checked. if the CPU is busy or the queue has a process in it, the current process is pushed on to the cpu queue
	//If the queue is empty and the CPU is free a new CPU enter process is created with same time and job number and pushed on to priority queue
	//next a new process must be taken off the disk 1 queue and put on to the disk
	//if the disk queue is not empty, the job is taken off the front of the queue, a new disk 1 enter process is created with same time and job number, and is pushed on to priority queue
    	void disk1Finish(){
	process cur_job=eventQueue.top();
	eventQueue.pop();
	disk1IsBusy=false;
	if(CPUIsBusy==true || !cpuQueue.empty()){
        	cpuQueue.push(cur_job);
    	}else if(CPUIsBusy==false && cpuQueue.empty()){
        	process new_job={cur_job.time,CPU_Enter,cur_job.id};
		eventQueue.push(new_job);
	}
	if(!disk1Queue.empty()){
        	process job=disk1Queue.front();
        	disk1Queue.pop();
        	process new_job={cur_job.time,Disk1_Enter,job.id};
        	eventQueue.push(new_job);
        	}
        }


	//handler function for process finishing at disk 2
	//the process is taken off the top of the priority queue
	//the disk 2 boolean is changed to idle
	//the cpu component is checked. if the CPU is busy or the queue has a process in it, the current process is pushed on to the cpu queue
	//If the queue is empty and the CPU is free a new CPU enter process is created with same time and job number and pushed on to priority queue
	//next a new process must be taken off the disk 2 queue and put on to the disk
	//if the disk queue is not empty, the job is taken off the front of the queue, a new disk 2 enter process is created with same time and job number, and is pushed on to priority queue
	void disk2Finish(){
	process cur_job=eventQueue.top();
	eventQueue.pop();
    	disk2IsBusy=false;
	if(CPUIsBusy==true || !cpuQueue.empty()){
        	cpuQueue.push(cur_job);
    	}else if(CPUIsBusy==false && cpuQueue.empty()){
        	process new_job={cur_job.time,CPU_Enter,cur_job.id};
		eventQueue.push(new_job);
	}
	if(!disk2Queue.empty()){
        	process job=disk2Queue.front();
        	disk2Queue.pop();
        	process new_job={cur_job.time,Disk2_Enter,job.id};
        	eventQueue.push(new_job);
        	}
        }


	//handler function for job exiting state
	//pops the job off the priority queue then returns to main
	void jobExit(){
	eventQueue.pop();
	}


	//handler function for ending simulation
	//prints out that the simulation is over and exits from program
	void simulationEnd(){
	cout<<"Event simulation is now over"<<endl;
	exit(0);
	}

