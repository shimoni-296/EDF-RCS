// Implementation file: sched.c file
// ----------------- HEADER INCLUSION -----------------
#include"sched.h"		  // For custom function declarations
#include<stdio.h>     // For standard I/O
#include<stdlib.h>    // For malloc(), free()
#include<math.h>      // For sqrt(), ceil()
#include<float.h>     // For FLT_MAX
#include<time.h>      // For ctime()

// --------------- FUNCTION DEFINITIONS ---------------

// Function to create a Node for priority queue
Node* newNode(int d, float p) {
   Node* temp = (Node*)malloc(sizeof(Node));
   temp->data = d;
   temp->priority = p;
   temp->next = NULL;
   return temp;
}

// Function to get first node value from priority queue
int peek(Node** head) {
   Node* tmp = (*head)->next;
   if(tmp==NULL){
     return -1;
   }
   return tmp->data;
}

// Function to remove Node from priority queue
void pop(Node** head, int data) {
   Node* temp = (*head)->next;
   Node* prev = *head;
   if((temp)==NULL)  return;
   while(temp!=NULL){
     if(temp->data == data){
       (prev)->next = (temp)->next;
       free(temp);
       break;
     }
     temp = temp->next;
     prev = prev->next;
   }
}

// Function to push node into priority queue
void push(Node** head, int d, float p) {
   Node* start = (*head);
   Node* temp = newNode(d, p);
   if ((*head)->priority > p) {
      temp->next = *head;
      (*head) = temp;
   } else {
      while (start->next != NULL &&
      start->next->priority <= p) {
         start = start->next;
      }
      // Either at the ends of the list
      // or at required position
      temp->next = start->next;
      start->next = temp;
   }
}

// Function to check the queue is empty
int isEmpty(Node** head) {
  Node* temp = (*head);
  if(temp->next!=NULL){
    return 0;
  }
  else{
    return 1;
  }
}

// Function to create a Node for Linked List
L_Node* new_L_Node(float s, float e, int t, int j) {
   L_Node* temp = (L_Node*)malloc(sizeof(L_Node));
   temp->st_t = s;
   temp->end_t = e;
   temp->task_id = t;
   temp->job_id = j;
   temp->next = NULL;
   return temp;
}

// Function to push Node into Linked List
void push_L_Node(L_Node** head, float s, float e, int t, int j) {
   L_Node* start = (*head);
   L_Node* temp = new_L_Node(s, e, t, j);

   while(start->next!=NULL) start = start->next;
   start->next = temp;
}

// Function to remove Linked List nodes from memory
void clear_L_List(L_Node** head){
  L_Node* temp = (*head)->next;
  L_Node* prev = *head;
  if((temp)==NULL)  return;
  while(temp!=NULL){
    prev = temp->next;
    free(temp);
    temp = prev;
  }
}

// Function to write Schedule to file
void printToFile(L_Node** head){
  L_Node* temp = (*head)->next;

  FILE *fptr;
  char* path = "Schedule.txt";
  fptr = fopen(path,"w");
  if(fptr == NULL){
    printf("Error in creating file!");
    return;
  }

  int prev_job = temp->job_id;
  int prev_job_tid = temp->task_id;
  float prev_job_st = temp->st_t;
  float prev_job_end = temp->end_t;

  while(temp!=NULL){
    if( prev_job_tid==temp->task_id && prev_job==temp->job_id){ // Same entry
      prev_job_end = temp->end_t;
    }else{ // Different entry
      if(prev_job == -1){
        fprintf(fptr, "%.2f - %.2f -> IDLE \n",
        prev_job_st, prev_job_end);
      }else{
        fprintf(fptr, "%.2f - %.2f -> T%d_J%d \n",
        prev_job_st, prev_job_end, prev_job_tid, prev_job );
      }
      // Update var
      prev_job = temp->job_id;
      prev_job_tid = temp->task_id;
      prev_job_st = temp->st_t;
      prev_job_end = temp->end_t;
    }
    temp = temp->next;
  }
  if(prev_job == -1){
    fprintf(fptr, "%.2f - %.2f -> IDLE \n",
    prev_job_st, prev_job_end);
  }else{
    fprintf(fptr, "%.2f - %.2f -> T%d_J%d \n",
    prev_job_st, prev_job_end, prev_job_tid, prev_job );
  }

  fclose(fptr);
}

// Function to print priority queue at a given time
void printQueue(Node** head, struct job* j_head, int lfd){
  Node* temp = (*head)->next;
  fprintf(lfd, "\n Start of Queue:-\n");
  while(temp!=NULL){
    fprintf(lfd, " J%d (%.2f / %.2f)", j_head[temp->data].j_id, j_head[temp->data].rem_time,
            j_head[temp->data].abs_deadline);
    temp = temp->next;
  }
  fprintf(lfd, "\n End of Queue ------\n\n");
}

// Function to get Total no of Periodic Task from file
int getNoOfPeriodicTasks(int fd){
	int no_of_tasks;
	fscanf(fd, "%d", &no_of_tasks);
	if(no_of_tasks<0)	return 0;
	return no_of_tasks;
}

// Function to round up float value
float round_func(float var)
{
    // 37.66666 * 100 =3766.66
    // 3766.66 + .5 =3767.16    for rounding off value
    // then type cast to int so value is 3767
    // then divided by 100 so the value converted into 37.67
    float value = (int)(var * 100 + .5);
    return (float)value / 100;
}

// Function to create a struct array for Periodic Tasks
struct periodicTask* readPeriodicTasks(int fd, int noPeriodicTasks){
	struct periodicTask* array = malloc(noPeriodicTasks * sizeof(struct periodicTask));
	for(int i=0;	i<noPeriodicTasks;	i++){
		float a,b,c,d;
    fscanf(fd, "%f %f %f %f", &a, &b, &c, &d);
    array[i].phase = round_func(a);
    array[i].period = round_func(b);
    array[i].wcet = round_func(c);
    array[i].rel_d = round_func(d);
	}
	return array;
}

// DEBUG function to check whether file reading is done correctly or not
void printPeriodicData(struct periodicTask* t_head, int noPeriodicTasks){
	printf("\n START OF DATA:- \n");
	printf("\n A P E D\n");
	for(int i=0;	i<noPeriodicTasks;	i++){
		printf(" %.2f %.2f %.2f %.2f\n", t_head[i].phase, t_head[i].period, t_head[i].wcet, t_head[i].rel_d);
	}
	printf("\n END OF DATA!! \n");
}

// Function to calculate GCD of two numbers
long long gcd(long long a,long long b){
	if(b==0)	return a;
	return	gcd(b,a%b);
}

// Function to calculate hyperperiod of periodic tasks
float calculateLCM(struct periodicTask* t_head, int noPeriodicTasks){
  int precision = 2;
  long long period[noPeriodicTasks];
  for(int i=0;i<noPeriodicTasks;i++){
    float val = t_head[i].period;
    for(int j=0;j<precision;j++)  val*=10.0;
    period[i] = val;
  }
	long long lcm = period[0];
	for(int i=1;i<noPeriodicTasks;i++){
		lcm = (period[i] * lcm) / (gcd(period[i], lcm));
	}
  float ans = (float)lcm;
  for(int j=0;j<precision;j++)  ans/=10;
	return ans;
}

// Function to get random float value between given range
float float_rand( float min, float max )
{
    float scale = rand() / (float) RAND_MAX; // [0, 1.0]
    return min + scale * ( max - min );      // [min, max]
}

// Function to get number of jobs for each task
void getTaskJobs(int* arr, struct periodicTask* t_head, int noPeriodicTasks, int time_upper_bound){
  for(int i=0;i<noPeriodicTasks;i++){
		arr[i] = (time_upper_bound - t_head[i].phase) / t_head[i].period;
	}
}

// Function to get utilization of task set
float getUtil(struct periodicTask* t_head, int noPeriodicTasks){
  float util = 0.0;
  for(int i=0;i<noPeriodicTasks;i++){
    util += t_head[i].wcet / (float)t_head[i].period;
  }
  return util;
}

// Function to get first in-phase time for the given task-set
float getInPhaseTime(struct periodicTask* t_head, int noPeriodicTasks){

  float task_time[noPeriodicTasks];
  for(int i=0;i<noPeriodicTasks;i++)  task_time[i]=t_head[i].phase;

  float inPhaseTime = 0;
  int pos;
  while(1){
    // Check if all task time are same
    short cond = 1;
    pos=0;
    for(int i=1;i<noPeriodicTasks;i++){
      if(task_time[i]!=task_time[pos]){
        cond=0; break;
      }
    }
    inPhaseTime = task_time[pos];
    if(cond==1){  break;  }
    // Get task id which has least time of all
    for(int i=1;i<noPeriodicTasks;i++){
      if( task_time[pos] > task_time[i] ){
        pos = i;
      }else if( task_time[pos] == task_time[i] ){
        if(t_head[i].period < t_head[pos].period){
          pos = i;
        }
      }
    }
    task_time[pos]+=t_head[pos].period;
  }
  return inPhaseTime;
}

// Function to get total number of jobs in a hyperperiod
int getNoOfJobs(int* arr, int noPeriodicTasks){
  int cnt=0;
  for(int i=0;i<noPeriodicTasks;i++)	cnt+=arr[i];
  return cnt;
}

// Function to initialize all job objects
void init_jobs(struct periodicTask* t_head, struct job* j_head, int* task_jobs_size, int noPeriodicTasks){
  int cnt=0;
	for(int i=0;i<noPeriodicTasks;i++){
		int j_lim = task_jobs_size[i];
		for(int j=0;j<j_lim;j++){
			j_head[cnt].task_id = i+1;
			j_head[cnt].j_id = cnt+1; // j+1
      j_head[cnt].job_id = j+1; // j+1
			j_head[cnt].status = 0;
			j_head[cnt].arrival_time = t_head[i].phase + t_head[i].period*j;
      j_head[cnt].period = t_head[i].period;
			j_head[cnt].abs_deadline = j_head[cnt].arrival_time + t_head[i].rel_d;
      j_head[cnt].priority = -j_head[cnt].abs_deadline;
			j_head[cnt].wcet = t_head[i].wcet;
      j_head[cnt].act_wcet = round_func(t_head[i].wcet * float_rand(0.5,1.0));
      j_head[cnt].rem_time = j_head[cnt].act_wcet;  // wcet / act_wcet
      j_head[cnt].first_st_time = -1;  // wcet / act_wcet
			cnt++;
		}
	}
}

// Function to print Job List
void printJobList(int* arr, int total_jobs, struct job* j_head, int lfd){
  fprintf(lfd,"\n\n \t\t Job List \n");
  fprintf(lfd," Job\t   AT    ET    ACT_ET    D ");
  for(int i=0;i<total_jobs;i++){
     fprintf(lfd,"\nJ_%d (T_%d)  %.2f  %.2f  %.2f    %.2f",
       j_head[arr[i]].j_id , j_head[arr[i]].task_id,
       j_head[arr[i]].arrival_time, j_head[arr[i]].wcet,
       j_head[arr[i]].act_wcet, j_head[arr[i]].abs_deadline );
  }
}

// Function to sort by indexes and store it into array
// Sort by ascending by arrival - In case of tie sort according to order of deadline
// Buuble sort - O(n^2) - Where n = Number of jobs
void sort_by_arrival_bubble(int* arr, int total_jobs, struct job* j_head){

  for(int i=0;i<total_jobs-1;i++){

    for(int j=i+1;j<total_jobs;j++){

      // Swap if condition satisfied
      if(j_head[arr[j]].arrival_time < j_head[arr[i]].arrival_time){
        arr[i] = arr[i]^arr[j];
        arr[j] = arr[i]^arr[j];
        arr[i] = arr[i]^arr[j];
      }else if(j_head[arr[i]].arrival_time == j_head[arr[j]].arrival_time){
        if(j_head[arr[i]].abs_deadline > j_head[arr[j]].abs_deadline){
          arr[i] = arr[i]^arr[j];
          arr[j] = arr[j]^arr[i];
          arr[i] = arr[i]^arr[j];
        }
      }
    }
  }

  // End of sorting
}

// Function to calculate Slack time of a job J at time t
float findSlack(int job_id, float t, struct job* j_head){
  return ( j_head[job_id].abs_deadline - t - j_head[job_id].rem_time  );
}


// This function computes the maximum time period up to which each of the queued
// jobs can be deferred; the minimum of these deferral times is the period for which the
// current job can be continued.
float extension_time(Node* readyQ, int Curr, float t, struct job* j_head){
  Node* pq_i = readyQ;
  Node* pq_k;
  pq_i = pq_i->next;
  int pq_size = 0;
  while(pq_i!=NULL){
    if(pq_i->data == Curr) break;
    pq_size++;
    pq_i = pq_i->next;
  }
  if(pq_size==0)  return 0;
  pq_i = readyQ;
  pq_i = pq_i->next;
  float job_ext_arr[pq_size];
  for(int i=0;i<pq_size;i++){
    float slack_val = findSlack(pq_i->data, t, j_head);
    float summation_part = 0.0;
    pq_k = readyQ->next;
    for(int j=0;j<i;j++){
      if(i==j){
        // Do nothing
      }else{
        float deadline_diff = j_head[pq_i->data].abs_deadline -j_head[pq_k->data].abs_deadline;
        summation_part += ( j_head[pq_k->data].rem_time +
                            ceil( deadline_diff / j_head[pq_k->data].period ) * j_head[pq_k->data].act_wcet );
      }
      pq_k = pq_k->next;
    }
    job_ext_arr[i] = slack_val - summation_part;
    pq_i = pq_i->next;
  }

  float ans = job_ext_arr[0];
  for(int i=1;i<pq_size;i++){
    if(ans > job_ext_arr[i])  ans = job_ext_arr[i];
  }
  ans -= round_func(0.3);
  if(ans<round_func(0.0)) ans = round_func(0.0);
  return ans;
}

// Function to update the Job status
int updateJobStats(struct job* j_head, int Curr, float t){
  j_head[Curr].status = 1;
  j_head[Curr].fin_time = t;
  j_head[Curr].rem_time = 0.0;
  if( j_head[Curr].fin_time > j_head[Curr].abs_deadline ){
    printf("\n A job missed deadline! Check log.txt to debug!");
    return -1;
  }
  return 0;
}

// Function to update the Job first schedule time
void updateJobTime(struct job* j_head, int Curr, float t){
  if(j_head[Curr].first_st_time==-1){
    j_head[Curr].first_st_time=t;
  }
}

// Function to add jobs to priority queue
short addJobs(Node* readyQ, int* idx, float t, struct job* j_head, int* arr, int limit){
  while(t >= j_head[ arr[*idx] ].arrival_time){
    push(&readyQ, arr[*idx], j_head[ arr[*idx] ].abs_deadline );
    *idx = *idx + 1;  // Increment counter
    if(*idx == limit){ // No new jobs to come now
      return 1;
    }
  }
  return 0;
}

// Function to simulate EDF_RCS algorithm for given task set
void edf_rcs_schedule(int* arr, int noPeriodicTasks,int total_jobs, struct job* j_head, float hyperPeriod, int lfd){
  fprintf(lfd, "\n\n ***** Start of Schedule! *****\n");

  /** ---- Scheduler Initilizaiton -------- */
  // Create head pointer for ready queue ordered by priority
  Node* readyQ = newNode(-1, 0);  // First head as priority 0 (which any job can't have)
  int arr_pos = 0;

  // Create head for Linked List scheduling struct
  L_Node* sched = new_L_Node(0,0,0,0);

  float t = 0.0;            // Current time t
  float next_t = 0.0;       // Next decision point time for scheduler
  int Curr, J;              // Variable to hold job pointer
  short no_more_jobs = 0;   // Check if more jobs are coming or not
  short dec_type=0;         // Decision Type for scheduler
  float extensionTime_Curr; // Deffered time calculated for a job
  int def_switch = 0;       // Whether a Job was deffered or not
  float def_switch_time;    // Time till which job can be deferred from time t
  int schedule_res;         // Whether a job missed it deadline or not
  float preemption_cost = 0.2;
  float sched_cost = 0.1;
  float extension_time_cost = 0.05;
  int noOfPreEmption = 0;		// Number of pre-emption happening during schedule

  // Add all jobs at time t = 0 (For initialization)
  no_more_jobs = addJobs(readyQ, &arr_pos, 0.0, j_head, arr, total_jobs);
  
  /** ---- END of Initilization ------------ */

  /** ------- Main Scheduler logic ---------- */
  while(1){
    t = next_t;
    t+=sched_cost;
    if(no_more_jobs==1 && isEmpty(&readyQ)==1){ // Break condition
      break;
    }
    printQueue(&readyQ, j_head, lfd);
    if(isEmpty(&readyQ)==0){
      updateJobTime(j_head, peek(&readyQ), t);
    }

    if(def_switch==0) Curr = peek(&readyQ);

    /**
      Find out next decision point
    */
    // CASE: IDLE job condition checking
    if( Curr == -1 ){
      next_t = round_func(j_head[arr[arr_pos]].arrival_time);
      dec_type = 3;
    }
    // CASE: There is a deffered switch
    else if(def_switch==1){
      if( no_more_jobs==0  ){
        // Whether current job finishes first or new job arrives first
        if( round_func(t+j_head[Curr].rem_time) == round_func(def_switch_time)
           && round_func(j_head[arr[arr_pos]].arrival_time) == round_func(def_switch_time) ){
            next_t = round_func(t+j_head[Curr].rem_time);
            dec_type = 1;
        }
        else{
          if(def_switch_time < (t+j_head[Curr].rem_time)
            && def_switch_time < (j_head[arr[arr_pos]].arrival_time)){
            next_t = def_switch_time;
            dec_type = 4;
          }
          else if(t+j_head[Curr].rem_time < (def_switch_time)
            && t+j_head[Curr].rem_time < (j_head[arr[arr_pos]].arrival_time)){
            next_t = t+j_head[Curr].rem_time;
            dec_type = 1;
          }
          else if(j_head[arr[arr_pos]].arrival_time < (def_switch_time)
            && j_head[arr[arr_pos]].arrival_time < (t+j_head[Curr].rem_time)){
            next_t = j_head[arr[arr_pos]].arrival_time;
            dec_type = 2;
          }
          else{
            if(round_func(t+j_head[Curr].rem_time) == round_func(j_head[arr[arr_pos]].arrival_time)){
              next_t = t+j_head[Curr].rem_time;
              dec_type = 1;
            }else{
              next_t = t+j_head[Curr].rem_time;
              dec_type = 2;
            }
          }
        }

      }else{ // No more jobs incoming
        if( def_switch_time < t+j_head[Curr].rem_time ){
          next_t = def_switch_time;
          dec_type = 4;
        }else if( round_func(def_switch_time) == round_func(t+j_head[Curr].rem_time) ){
          next_t = def_switch_time;
          dec_type = 1;
        }else{
          next_t = t+j_head[Curr].rem_time;
          dec_type = 0;
        }
      }
    }
    // CASE: Departure + Arrival at same time
    else if( no_more_jobs==0 &&
      (round_func(t+j_head[Curr].rem_time) == round_func(j_head[arr[arr_pos]].arrival_time)) ){
      next_t = round_func(t + j_head[Curr].rem_time);
      dec_type = 1;
    }
    // CASE: Departure occurs before arrival
    else if( no_more_jobs==0 && t+j_head[Curr].rem_time < j_head[arr[arr_pos]].arrival_time ){
      next_t = round_func(t + j_head[Curr].rem_time);
      dec_type = 0;
    }
    // CASE: Arrival before departure
    else if( no_more_jobs==0 && t+j_head[Curr].rem_time > j_head[arr[arr_pos]].arrival_time ){
      next_t = round_func(j_head[arr[arr_pos]].arrival_time);
      dec_type = 2;
    }
    // CASE: No more jobs to arrive
    else if( no_more_jobs == 1 ){
      next_t = round_func(t + j_head[Curr].rem_time);
      dec_type = 0;
    }

    fprintf(lfd, "\n Current time t:%.2f Next time:%.2f", t, next_t);


    /**
        Do operation according to decision type
    **/
    // CASE: Current departure
    if(dec_type==0){
      schedule_res = updateJobStats(j_head, Curr, next_t);
      if(schedule_res==-1)  return;
      push_L_Node(&sched, t, next_t, j_head[Curr].task_id, j_head[Curr].job_id);
      pop(&readyQ, Curr); // Remove the finished job from ready queue
      def_switch = 0;
    }
    // CASE: Current departure + New job arrivals
    else if(dec_type==1){
      schedule_res = updateJobStats(j_head, Curr, next_t);
      if(schedule_res==-1)  return;
      push_L_Node(&sched, t, next_t, j_head[Curr].task_id, j_head[Curr].job_id);
      pop(&readyQ, Curr); // Remove the finished job from ready queue
      if(next_t == j_head[arr[arr_pos]].arrival_time){
        no_more_jobs = addJobs(readyQ, &arr_pos, next_t, j_head, arr, total_jobs); // Add jobs
      }
      def_switch = 0;
    }
    // CASE: New job arrives before Curr can depart
    else if(dec_type==2){
      j_head[Curr].rem_time -= (next_t - t); // Update remaining time for Current job
      push_L_Node(&sched, t, next_t, j_head[Curr].task_id, j_head[Curr].job_id);
      no_more_jobs = addJobs(readyQ, &arr_pos, next_t, j_head, arr, total_jobs); // Add jobs
      J = peek(&readyQ);
      if( Curr == J ){
        // Do nothing (Continue with Curr)
      }
      else{
        next_t += round_func(extension_time_cost);
        extensionTime_Curr = round_func(extension_time(readyQ, Curr, next_t, j_head));
        if(extensionTime_Curr == 0.0f){
          // Pre-emption happens HERE
          push_L_Node(&sched, t, next_t-round_func(extension_time_cost),
            j_head[Curr].task_id, j_head[Curr].job_id);
          def_switch = 0;
          /** Add Pre-emption overhead  */
          next_t += round_func(preemption_cost);
          noOfPreEmption++;
        }
        else{
          def_switch = 1;
          def_switch_time = round_func(next_t+extensionTime_Curr);
        }
      }

    }
    // CASE: IDLE job
    else if(dec_type==3){
      push_L_Node(&sched, t, next_t, -1, -1);
      no_more_jobs = addJobs(readyQ, &arr_pos, next_t, j_head, arr, total_jobs); // Add jobs
      def_switch = 0;
    }
    // CASE: When Curr is extended and still new job arrives
    else{
      j_head[Curr].rem_time -= round_func(next_t - t); // Update remaining time for Current job
      push_L_Node(&sched, t, next_t, j_head[Curr].task_id, j_head[Curr].job_id);
      next_t += round_func(preemption_cost);
      noOfPreEmption++;
      def_switch = 0;
    }

  } // End of while loop

  if(t<hyperPeriod) push_L_Node(&sched, t, hyperPeriod, -1, -1);
  printToFile(&sched);

  clear_L_List(&sched);	// Clear Linked List
  free(sched);					// Free memory occupied by linked list head
  free(readyQ);					// Free memory occupied by priority queue
  fprintf(lfd, "\n\n ***** End of Schedule! *****\n");
  
  // Get Job Statistics
  generateStatistics(j_head,total_jobs,noPeriodicTasks, noOfPreEmption);

  /** ------- END of Main Scheduler logic ---------- */
}

// Function to write Scheduling Params to file
void generateStatistics(struct job* j_head, int total_jobs, int noPeriodicTasks, int cnt){
  FILE* sfd = fopen("Final_Statistics.txt", "w");
  if(sfd == NULL){
    printf("LOG: Fail to create Statistics file!");
    return;
  }

  // Logic to get Number of context switch
  int cntx_swth = 0;
  FILE* fp = fopen("Schedule.txt","r");
  if(fp==NULL){
    printf("Couldn't open Schedule.txt file!");
    return;
  }
  char c;
  // Extract characters from file and store in character c
  for (c = getc(fp); c != EOF; c = getc(fp))
    if (c == '\n') // Increment count if this character is newline
      cntx_swth = cntx_swth + 1;

  // Get Max Response Time and Min Response Time of each Task
  float max_res_arr[noPeriodicTasks];
  float min_res_arr[noPeriodicTasks];

  for(int i=0;i<noPeriodicTasks;i++){
    float max_res = 0;
    float min_res = FLT_MAX;
    for(int j=0;j<total_jobs;j++){
      if( ( j_head[j].task_id==i+1 ) && ( max_res < (j_head[j].fin_time - j_head[j].arrival_time) )){
        max_res = j_head[j].fin_time - j_head[j].arrival_time;
      }
      if( ( j_head[j].task_id==i+1 ) && ( min_res > (j_head[j].fin_time - j_head[j].arrival_time) )){
        min_res = j_head[j].fin_time - j_head[j].arrival_time;
      }
    }
    max_res_arr[i] = max_res;
    min_res_arr[i] = min_res;
  }

  // Get Average Response Time of each Task
  float avg_res_arr[noPeriodicTasks];
  for(int i=0;i<noPeriodicTasks;i++){
    int jb_cnt = 0;
    float sum_res = 0;
    for(int j=0;j<total_jobs;j++){
      if( j_head[j].task_id==i+1 ){
        sum_res += (j_head[j].fin_time - j_head[j].arrival_time);
        jb_cnt++;
      }
    }
    avg_res_arr[i] = sum_res / jb_cnt;
  }

  // Get Absolute Response Time Jitter of each task
  float arj_task[noPeriodicTasks];
  for(int i=0;i<noPeriodicTasks;i++){
    arj_task[i] = max_res_arr[i] - min_res_arr[i];
  }

  // Get Relative Response Time Jitter of each task
  float rrj_task[noPeriodicTasks];
  for(int i=0;i<noPeriodicTasks;i++){
    float rrj_max = 0;
    int init,p1,p2,j;
    for(j=0;j<total_jobs;j++){
      if(j_head[j].task_id==i+1){
        p1 = j; init = p1;  break;
      }
    }j++;
    for(;j<total_jobs;j++){
      if(j_head[j].task_id==i+1){
        p2 = j;
        if(rrj_max < (j_head[p2].fin_time - j_head[p1].fin_time) ){
          rrj_max = (j_head[p2].fin_time - j_head[p1].fin_time);
        }
        p1 = j;
      }
    }
    if(rrj_max < (j_head[p1].fin_time - j_head[init].fin_time) ){
      rrj_max = (j_head[p1].fin_time - j_head[init].fin_time);
    }
    rrj_task[i] = rrj_max;
  }

  // Get Max Waiting Time and Min Waiting Time of each Task
  float max_wt_arr[noPeriodicTasks];
  float min_wt_arr[noPeriodicTasks];

  for(int i=0;i<noPeriodicTasks;i++){
    float max_wt = 0;
    float min_wt = FLT_MAX;
    for(int j=0;j<total_jobs;j++){
      if( ( j_head[j].task_id==i+1 ) && ( max_wt < (j_head[j].first_st_time - j_head[j].arrival_time) )){
        max_wt = j_head[j].first_st_time - j_head[j].arrival_time;
      }
      if( ( j_head[j].task_id==i+1 ) && ( min_wt > (j_head[j].first_st_time - j_head[j].arrival_time) )){
        min_wt = j_head[j].first_st_time - j_head[j].arrival_time;
      }
    }
    max_wt_arr[i] = max_wt;
    min_wt_arr[i] = min_wt;
  }

  // Get Average Waiting Time of each Task
  float avg_wt_arr[noPeriodicTasks];
  for(int i=0;i<noPeriodicTasks;i++){
    int jb_cnt = 0;
    float sum_res = 0;
    for(int j=0;j<total_jobs;j++){
      if( j_head[j].task_id==i+1 ){
        sum_res += (j_head[j].first_st_time - j_head[j].arrival_time);
        jb_cnt++;
      }
    }
    avg_wt_arr[i] = sum_res / jb_cnt;
  }


  // ---------------- Output Code to File -----------------------------------

  fprintf(sfd,"\n ******************** SCHEDULING PARAMETER *****************************");
  fprintf(sfd,"\n Number of Context Switch: %d", cntx_swth);
  fprintf(sfd,"\n Number of Pre-emptions: %d", cnt);

  fprintf(sfd,"\n\n *************** RESPONSE TIME JITTER ***************\n");
  fprintf(sfd,"\n %4s   %28s   %28s ",
    "Task","Absolute Response Time Jitter", "Relative Response Time Jitter");
  for(int i=0;i<noPeriodicTasks;i++){
    fprintf(sfd,"\n %-5d %15.2f %32.2f",i+1, arj_task[i], rrj_task[i]);
  }

  fprintf(sfd,"\n\n *************** RESPONSE TIME ***************\n");
  fprintf(sfd,"\n %4s %10s %10s %10s","Task","MAX", "AVG", "MIN");
  for(int i=0;i<noPeriodicTasks;i++){
    fprintf(sfd,"\n %-5d %10.2f %10.2f %10.2f",i+1, max_res_arr[i], avg_res_arr[i], min_res_arr[i]);
  }

  fprintf(sfd,"\n\n *************** WAITING TIME ***************\n");
  fprintf(sfd,"\n %4s %10s %10s %10s","Task","MAX", "AVG", "MIN");
  for(int i=0;i<noPeriodicTasks;i++){
    fprintf(sfd,"\n %-5d %10.2f %10.2f %10.2f",i+1, max_wt_arr[i], avg_wt_arr[i], min_wt_arr[i]);
  }

  close(sfd);
}


// Driver function that runs the EDF_RCS scheduler 
int create_schedule(int fd){

  FILE* lfd = fopen("log.txt", "w");
  if(lfd==NULL){
    printf(" Log file creation failed!");
    return -1;
  }
  time_t curr_t;
  time(&curr_t);
  fprintf(lfd, "Program ran last on : %s", ctime(&curr_t));

  // Use current time as seed for random generator
  srand(time(0));

  // Load the data from file
  int noPeriodicTasks = getNoOfPeriodicTasks(fd);
  struct periodicTask* p_head;
	p_head = readPeriodicTasks(fd, noPeriodicTasks);
	fprintf(lfd, "\nLOG: Periodic Task Data read successfully!");
  
  float utilization = getUtil(p_head, noPeriodicTasks);
  if(utilization>1){
    //printf("\nLOG: Utilization over 100 percent! \nLOG: Terminating Program!\n");
    fprintf(lfd, "\nLOG: Utilization over 100 percent! \nLOG: Terminating Program!\n");
    free(p_head);
    close(fd);  // Close the file descriptor
  }
  fprintf(lfd, "\nLOG: Utilization is %.2f", utilization);

  float inPhaseTime = getInPhaseTime(p_head, noPeriodicTasks);
  fprintf(lfd, "\nLOG: Calculated In Phase Time: %.2f", inPhaseTime);

  float hyperPeriod = calculateLCM(p_head, noPeriodicTasks);
  fprintf(lfd, "\nLOG: Calculated Hyper-period: %.2f", hyperPeriod);

  float time_upper_bound; //min (3*hyperPeriod, inPhaseTime + hyperPeriod);
  if( (3*hyperPeriod) < (inPhaseTime + hyperPeriod) ) time_upper_bound = 3*hyperPeriod;
  else  time_upper_bound = inPhaseTime + hyperPeriod;
  fprintf(lfd, "\nLOG: Calculated upper bound (for scheduling): %.2f", time_upper_bound);

	// Get total number of jobs and initalize them
  int task_jobs_size[noPeriodicTasks];
  getTaskJobs(task_jobs_size, p_head, noPeriodicTasks, time_upper_bound);
  int total_jobs = getNoOfJobs(task_jobs_size, noPeriodicTasks);
  struct job* j_head = malloc(total_jobs * sizeof(struct job));
  init_jobs(p_head, j_head, task_jobs_size, noPeriodicTasks);
  fprintf(lfd, "\nLOG: Jobs initialization success!\n");
  
  // Sort the array to get Job List
  int sorted_job_list[total_jobs];
  for(int i=0;i<total_jobs;i++) sorted_job_list[i] = i;
  sort_by_arrival_bubble(sorted_job_list, total_jobs, j_head);
  printJobList(sorted_job_list, total_jobs, j_head, lfd);

	// Run the EDF_RCS algorithm 
  edf_rcs_schedule(sorted_job_list, noPeriodicTasks, total_jobs, j_head, time_upper_bound, lfd);

  free(j_head);	// Free memory occupied by Job structure objects
  free(p_head);	// Free memory occupied by periodic task structure objects
  close(fd);    // Close the file descriptor that read data
  fprintf(lfd, "\nLOG: Terminating program!\n");
  close(lfd);   // Close the file descriptor that wrote log
  return 0;
}
