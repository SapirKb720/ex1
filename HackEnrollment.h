#ifndef HACK_ENTROLLMENT_H
#define HACK_ENTROLLMENT_H

#include <stdbool.h>
#include <stdlib.h>
#include "IsraeliQueue.h"
#include <stdio.h>
#include <string.h>

#define ID_LENGTH 10
#define ID_STAGE 0
#define CREDITS_STAGE 1
#define GPA_STAGE 2
#define NAME_STAGE 3
#define SURNAME_STAGE 4
#define CITY_STAGE 5
#define DEPARTMENT_STAGE 6
#define NUM_STAGES 7

#define HACKER_STAGE_ID 0
#define HACKER_STAGE_COURSES 1
#define HACKER_STAGE_FRIENDS 2
#define HACKER_STAGE_RIVALS 3
#define NUM_HACKER_STAGES 4

#define FAILED 0

#define FRIENDS 20
#define RIVALS -20
#define NOT_FRIENDS_NOT_RIVALS 0
#define NOT_FOUND -1

#define NUM_WANTED_COURSES 2
#define ONE_COURSE 1

typedef struct student* Student;
typedef struct hacker* Hacker; 
typedef struct enrollmentSystem* EnrollmentSystem; 
typedef struct node* Node;

EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers);

EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues);

void hackEnrollment(EnrollmentSystem sys, FILE* out);

/*
* function: compares whethere two pointers are the same
* does so by comparing tthe addresses they point to
* input: two pointers to two students
* output: whethere the two students are the same
*/
int comparisonFunc(void* student1, void* student2);

void updateIsCaseSensitive(EnrollmentSystem sys, bool isCaseSensitive);

#endif //HACK_ENTROLLMENT_H

