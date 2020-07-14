// Header file : sched.h
#ifndef _sched_h
#define _sched_h

// Structure for priority Queue Node
typedef struct node {
   int data;            // Job ID index to get data from Job structure
   float priority;      // Priority value as deadline(J)
   struct node* next;   // Pointer to next node in queue
} Node;

// Structure for Periodic Task
struct periodicTask {
	float phase;					// Phi_i - Arrival time for first instance
	float period;					// P_i   - Period
	float wcet;		  			// e_i   - Worst Case Execution Time
	float rel_d;	  			// D_i   - Deadline
};


// Structure for denoting a Job
struct job {
	int task_id;           // Respective Task ID
	int j_id;              // Job ID (For sorting)
  int job_id;            // Actual Job ID
  short status;          // Scheduled or not. 0 -> Not completed , 1-> Completed
	float arrival_time;    // Arrival time of Job (Arrival jitter is zero)
  float period;          // Period of job (Period of task it belongs to)
	float abs_deadline;    // Absolute deadline for the job (D_i)
  float fin_time;        // Time when job finished executing
  float priority;        // Priority of job (Job level fixed)
	float wcet;            // Worst case execution time of job
  float act_wcet;        // Relaxed wcet time using rand float value -> [0.5 - 1.0] of wcet
  float rem_time;        // Execution time of job remaining
  float first_st_time;   // Time instance when Job was first scheduled
};

// Structure for SCHEDULING
typedef struct L_node {
   float st_t;    // Start time
   float end_t;   // End time
   int task_id;   // Task ID
   int job_id;    // Job ID
   struct L_node* next;   // Pointer to next node in Linked List
} L_Node;

/**
Function to create a Node for priority queue
Arguments to the function:
  d : Job ID index
	p : Priority value (Job deadline value)
*/
Node* newNode(int d, float p);

/**
Function to get first node value from priority queue
Arguments to the function:
  **head : Pointer to head pointer of priority queue
*/
int peek(Node** head);

/**
Function to remove Node from priority queue
Arguments to the function:
  **head : Pointer to head pointer of priority queue
	data   : Priority value (Job deadline value)
*/
void pop(Node** head, int data);

/**
Function to push node into priority queue
Arguments to the function:
	**head : Pointer to head pointer of priority queue
  d			 : Job ID index
	p			 : Priority value (Job deadline value)
*/
void push(Node** head, int d, float p);

/**
Function to check whether priority queue is Empty or not
Arguments to the function:
  **head : Pointer to head pointer of priority queue
*/
int isEmpty(Node** head);

/**
Function to create a Node for Linked List
Arguments to the function:
  s: Start time for this node
	e: End time for this node
	t: Task ID corresponding to job
	j: Job ID
*/
L_Node* new_L_Node(float s, float e, int t, int j);

/**
Function to push Node into Linked List
Arguments to the function:
	**head	: Pointer to head pointer of Linked List
  s				: Start time for this node
	e				: End time for this node
	t				: Task ID corresponding to job
	j				: Job ID
*/
void push_L_Node(L_Node** head, float s, float e, int t, int j);

/**
Function to remove Linked List nodes from memory
Arguments to the function:
	**head	: Pointer to head pointer of Linked List
*/
void clear_L_List(L_Node** head);

/**
Function to print Schedule to file
Arguments to the function:
	**head	: Pointer to head pointer of Linked List
*/
void printToFile(L_Node** head);

/**
Function to print priority queue at a given time
Arguments to the function:
	**head	: Pointer to head pointer of priority queue
	*j_head : Pointer to Job struct array
	lfd			: FILE pointer for log.txt		
*/
void printQueue(Node** head, struct job* j_head, int lfd);

/**
Function to get Total no of Periodic Task from file
Arguments to the function:
  fd: File descriptor to file containing Periodic Task Set
*/
int getNoOfPeriodicTasks(int fd);

/**
Function to round float values to 2 decimal points after point
Arguments to the function:
  var: Float value to be rounded
*/
float round_func(float var);

/**
Function to create a struct array for Periodic Tasks
Arguments to the function:
  fd:              File descriptor to file containing Periodic Task Set
  noPeriodicTasks: Number of periodic tasks
*/
struct periodicTask* readPeriodicTasks(int fd, int noPeriodicTasks);

// DEBUG function to check whether file reading is done correctly or not
void printPeriodicData(struct periodicTask* t_head, int noPeriodicTasks);

// GCD function (Used indirectly to calculate LCM)
long long gcd(long long a,long long b);

/**
Function to calculate hyperperiod of periodic tasks
Arguments to the function:
  *t_head:         Pointer to head of the struct Periodic Tasks
  noPeriodicTasks: Number of periodic tasks
*/
float calculateLCM(struct periodicTask* t_head, int noPeriodicTasks);

/**
Function to get random float value between given range
Arguments to the function:
  min:  Minimum value of range [a,b] aka a
  max:  Maximum value of range [a,b] aka b
*/
float float_rand( float min, float max );

/**
Function to get number of jobs for each task
Arguments to the function:
  *arr							: Array that contains index sorted by arrival time for jobs
  *t_head						: Pointer to head of the struct Periodic Tasks
  noPeriodicTasks		: Number of periodic tasks
  time_upper_bound	: min(3*hyper-period, first in-phase time + hyper period)
*/
void getTaskJobs(int* arr, struct periodicTask* t_head, int noPeriodicTasks, int time_upper_bound);

/**
Function to get utilizzation for given task set
Arguments to the function:
  *t_head						: Pointer to head of the struct Periodic Tasks
  noPeriodicTasks		: Number of periodic tasks
*/
float getUtil(struct periodicTask* t_head, int noPeriodicTasks);

/**
Function to get first in-phase time for given task set
Arguments to the function:
	*t_head						: Pointer to head of the struct Periodic Tasks
  noPeriodicTasks		: Number of periodic tasks
*/
float getInPhaseTime(struct periodicTask* t_head, int noPeriodicTasks);

/**
Function to get total number of jobs in a hyperperiod
Arguments to the function:
  *arr            : Array that contains index sorted by arrival time for jobs
  noPeriodicTasks : Number of periodic tasks
*/
int getNoOfJobs(int* arr, int noPeriodicTasks);

/**
Function to initialize all job objects
Arguments to the function:
  *t_head:         Pointer to head of the struct Periodic Tasks
  *j_head:         Pointer to head of the struct job
  *task_jobs_size: Array containing number of jobs for each task
  noPeriodicTasks: Number of periodic tasks
*/
void init_jobs(struct periodicTask* t_head, struct job* j_head, int* task_jobs_size, int noPeriodicTasks);

/**
Function to print Jobs
Arguments to the function:
	*arr						: Array that contains index sorted by arrival time for jobs
	total_jobs      : Total number of jobs for given task set
	*j_head					: Pointer to head of the struct job
  lfd							: FILE pointer to log.txt file
*/
void printJobList(int* arr, int total_jobs, struct job* j_head, int lfd);

/**
Function to sort jobs by using Bubble Sort
Arguments to the function:
  *arr						: Array that contains index sorted by arrival time for jobs
	total_jobs      : Total number of jobs for given task set
	*j_head					: Pointer to head of the struct job
*/
void sort_by_arrival_bubble(int* arr, int total_jobs, struct job* j_head);

/**
Function to find slack for jobs at given time
Arguments to the function:
  job_id	: Job ID for which slack needs to be find out
	t				: Time at which slack needs to be calculated
	*j_head	: Pointer to head of the struct job
*/
float findSlack(int job_id, float t, struct job* j_head);

/**
Function to calculate extention time for Current Job executing (With overhead considerations)
Arguments to the function:
  *readyQ	: Pointer to priority queue 
	Curr		: Index value of currently running job
	t				: Time at which extention time needs to be calculated
	*j_head	: Pointer to head of the struct job
*/
float extension_time(Node* readyQ, int Curr, float t, struct job* j_head);

/**
Function to update Job parameters when it finishes execution
Arguments to the function:
  *j_head	: Pointer to head of the struct job
	Curr		: Index value of currently running job
	t				: Time at which job is finished
*/
int updateJobStats(struct job* j_head, int Curr, float t);

/**
Function to update the Job first schedule time
Arguments to the function:
  *j_head	: Pointer to head of the struct job
	Curr		: Index value of currently running job
	t				: Time at which job is first scheduled
*/
void updateJobTime(struct job* j_head, int Curr, float t);

/**
Function to add jobs to priority queue at given time (Used during scheduling)
Arguments to the function:
  *readyQ	: Pointer to priority queue 
	*idx		: Pointer to next arrival of job
	t				: Current time
	*j_head	: Pointer to head of the struct job
	*arr		: Array that contains index sorted by arrival time for jobs
	limit	  : Total number of jobs
*/
short addJobs(Node* readyQ, int* idx, float t, struct job* j_head, int* arr, int limit);

/**
Function to simulate EDF_RCS algorithm for given task set
Arguments to the function:
  *arr							: Array that contains index sorted by arrival time for jobs
	noPeriodicTasks		: Number of periodic tasks
	total_jobs      	: Total number of jobs for given task set
	*j_head						: Pointer to head of the struct job
	hyperPeriod				: Time till which scheduler needs to run
	lfd								: FILE pointer for log.txt
*/
void edf_rcs_schedule(int* arr, int noPeriodicTasks,int total_jobs, struct job* j_head, float hyperPeriod, int lfd);

/**
Function to generate Statistics report for the calculated schedule to file "Final_Statistics.txt"
Arguments to the function:
  *j_head         : Pointer to head of the struct job
  total_jobs      : Total number of jobs for given task set
  noPeriodicTasks : Number of periodic tasks
  cnt     				: Number of preemptions (Calculated during edf_rcs_schedule() call)
*/
void generateStatistics(struct job* j_head, int total_jobs, int noPeriodicTasks, int cnt);

/**
Driver function that uses all the above function and structures to generate schedule
Arguments to the function:
  fd: File descriptor to file containing Periodic Task Set
*/
int create_schedule(int fd);

#endif
