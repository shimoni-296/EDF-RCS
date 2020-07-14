## CONTENTS OF README FILE
--------------------------
 * General Info
 * Project Structure
 * How To Compile
 * How To Use


# GENERAL INFO
--------------
 Objective:
		Earliest Deadline First with Reduced Context Switch (EDF_RCS) priority scheduling algorithm implemented for scheduling periodic task-set.
 
 
# PROJECT STRUCTURE
-------------------
+ sched.h                    - Header File that contains declaration of custom functions required for implementation
+ sched.c                    - C file with definition of functions defined in header file
+ scheduler.c                - Driver C program that runs the main function
- inputFile.txt              - Input file to executable that contains Periodic Task-Set data
* Makefile                   - Makefile for project ( Creates executable )
* README.md                  - ReadME file
* Schedule.txt       				 - Text file with generated schedule
* Final_Statistics.txt       - Text file with all the statistics for the generated final schedule
* log.txt										 - Text file that logs execution of the program


# HOW TO COMPILE
----------------
 Make sure that you have the executable permission enabled for all files.
 If not, you can make the files executable by running chmod +777 for all project files.

 To compile the project, simply run the "make" command by opening the terminal where Makefile is located.
 The makefile executes the following command and creates scheduler executable:
    gcc -o scheduler scheduler.c sched.c -w -lm
 Where,
 -w  flag is to suppress warnings (can be ignored)
 -lm flag is to link math library (libm.so)


# HOW TO USE
------------
 To run the program, open terminal where executable file is located and execute the following:
                ./scheduler "inputFile.txt"   (Without "" quotes)
 Note: File which contains periodic task data with different filename can be given as well.
       Replace the inputFile.txt with the filename in that scenario.

 Input file requirement(s):
 - First line of the input file denotes a single integer N which tells total number of tasks in file
 - Remaining N lines follow space seperated tuple structure (Phi, P, E, D).
   Where,
    Phi - Phase
    P   - Period 
    E   - Worst Case Execution Time (WCET)
    D   - Relative deadline
 - Each value in tuple is taken as floating point value with precision upto 2 decimal points.
   (E.g., 2.554 is taken/rounded as 2.55 and 2.555 is taken/rounded as 2.56)


