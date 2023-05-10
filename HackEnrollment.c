#include "HackEnrollment.h"

typedef struct node {
	Node next;
	void* student;
	int friends; //amount of friends he let them pass
	int rivals; //amount of rivals he prevented them to pass him in line
	bool improved; //required for the improve function

}*Node;

typedef struct student {
	char* studentId;
	int totalCredits;
	int GPA;
	char* name;
	char* surname;
	char* city;
	char* department;
	bool isCaseSensitive;

	void* hacker;
}*Student;

typedef struct hacker {
	char* studentId;
	char** courseNumbers;
	char** friendsId;
	char** rivalsId;
	int numCourses;
	int numFriends;
	int numRivals;
}*Hacker;

typedef struct course {
	char* courseNumber;
	int size;
}*Course;

typedef struct enrollmentSystem {
	Student* students;
	Course* courses;
	Hacker* hackers;
	IsraeliQueue* enrollmentQueues;
	int studentsNum;
	int coursesNum;
	int hackersNum;
}*EnrollmentSystem;

int findHacker(char* hackerId, IsraeliQueue queue)
{
	IsraeliQueue temp = IsraeliQueueClone(queue);
	Node curr = ((Node)(IsraeliQueueDequeue(temp)));
	int index = 0;
	while (temp != NULL && curr != NULL)
	{
		if (strcmp(((Student)(curr->student))->studentId, hackerId) == 0)
		{
			return index;
		}
		index++;
		curr = ((Node)(IsraeliQueueDequeue(temp)));
	}
	return NOT_FOUND;
}

int findHackerInStudentArr(char* hackerId, Student* students, int numStudents) {
	for (int i = 0; i < numStudents; i++) {
		if (strcmp(hackerId, students[i]->studentId) == 0) {
			return i;
		}
	}
	return NOT_FOUND;
}

void updateIsCaseSensitive(EnrollmentSystem sys, bool isCaseSensitive)
{
	for (int i = 0; i < sys->studentsNum; i++)
	{
		(sys->students)[i]->isCaseSensitive = isCaseSensitive;
	}
}

void updateStudent(char* currInput, int currStage, Student currStudent)
{
	switch (currStage) {
	case ID_STAGE:
		currStudent->studentId = currInput;
		break;
	case CREDITS_STAGE:
		currStudent->totalCredits = atoi(currInput);
		free(currInput);
		break;
	case GPA_STAGE:
		currStudent->GPA = atoi(currInput);
		free(currInput);
		break;
	case NAME_STAGE:
		currStudent->name = currInput;
		break;
	case SURNAME_STAGE:
		currStudent->surname = currInput;
		break;
	case CITY_STAGE:
		currStudent->city = currInput;
		break;
	case DEPARTMENT_STAGE:
		currStudent->department = currInput;
		break;
	}
}

void freeAllStudents(Student* studentsArray, int numStudents) {
	for (int i = 0; i < numStudents; i++) {
		free(studentsArray[i]->city);
		free(studentsArray[i]->name);
		free(studentsArray[i]->surname);
		free(studentsArray[i]->studentId);
		free(studentsArray[i]);
	}
	free(studentsArray);
}

Student* ParseStudentFile(FILE* students, EnrollmentSystem enrollmentSystem){
	Student* studentsArray = NULL, currStudent = malloc(sizeof(Student));
	int currSize = 0, currStage = 0, numStudents = 0;
	char inputChar = 0;
	char* currInput = NULL;
	bool notEmpty = false;
	while (inputChar != EOF) {
		inputChar = fgetc(students);
		if (inputChar == '\n' || inputChar == EOF) {
			if (!notEmpty) {
				free(currStudent);
				return NULL;
			}
			numStudents++;
			if ((studentsArray = realloc(studentsArray, numStudents)) == NULL) {
				free(currStudent);
				return NULL;
			}
			currStudent->hacker = NULL;
			updateStudent(currInput, currStage, currStudent);
			studentsArray[numStudents-1] = currStudent;
			if (inputChar == '\n') { //allocating space for new student
				currStudent = malloc(sizeof(Student));
				currStage = 0;
			}
		}
		else if (inputChar == ' ') {
			if ((currInput = realloc(currInput, currSize + 1)) == NULL) {
				free(currStudent);
				freeAllStudents(studentsArray,numStudents);
				return NULL;
			}
			currInput[currSize] = '\0';
			updateStudent(currInput, currStage, currStudent);
			currSize = 0;
			currInput = NULL;
			currStage++; //moving to next stage
		}
		else if (currInput != EOF){
			if (!notEmpty) {
				numStudents++;
				enrollmentSystem->studentsNum = numStudents;
				notEmpty = true;
			}
			currSize++;
			if ((currInput = realloc(currInput, currSize)) == NULL) {
				freeAllStudents(studentsArray, numStudents);
				return NULL;
			}
			currInput[currSize - 1] = inputChar;	
		}
	}
	return studentsArray;
}

bool updateHacker(char* currInput, int currStage, Hacker currhacker)
{
	switch (currStage) {
	case HACKER_STAGE_ID:
		currhacker->numCourses = 0;
		currhacker->numFriends = 0;
		currhacker->numRivals = 0;
		currhacker->studentId = currInput;
		break;
	case HACKER_STAGE_COURSES:
		currhacker->numCourses++;
		if (currhacker->courseNumbers = realloc(currhacker->courseNumbers, 
			sizeof(char*) * (currhacker->numCourses)) == NULL){
			return FAILED;
		}
		currhacker->courseNumbers[currhacker->numCourses - 1] = currInput;
		break;
	case HACKER_STAGE_FRIENDS:
		currhacker->numFriends++;
		if (currhacker->friendsId = realloc(currhacker->friendsId,
			sizeof(char*) * (currhacker->numFriends)) == NULL) {
			return FAILED;
		}
		currhacker->friendsId[currhacker->numFriends - 1] = currInput;
		break;
	case HACKER_STAGE_RIVALS:
		currhacker->numRivals++;
		if (currhacker->rivalsId = realloc(currhacker->rivalsId,
			sizeof(char*) * (currhacker->numRivals)) == NULL) {
			return FAILED;
		}
		currhacker->rivalsId[currhacker->numRivals - 1] = currInput;
		break;
	}
	return !FAILED;
}


void freeAllHackers(Hacker* hackersArray, int numHackers) {
	for (int i = 0; i < numHackers; i++) {
		if (hackersArray !=NULL && hackersArray[i] != NULL) {
			free(hackersArray[i]->studentId);
			for (int j = 0; j < hackersArray[i]->numCourses; j++) {
				if (hackersArray[i]->courseNumbers != NULL) {
					free(hackersArray[i]->courseNumbers[j]);
				}
			}
			free(hackersArray[i]->courseNumbers);
			for (int p = 0; p < hackersArray[i]->numFriends; p++) {
				if (hackersArray[i]->friendsId != NULL) {
					free(hackersArray[i]->friendsId[p]);
				}
			}
			free(hackersArray[i]->friendsId);
			for (int k = 0; k < hackersArray[i]->numRivals; k++) {
				if (hackersArray[i]->rivalsId != NULL) {
					free(hackersArray[i]->rivalsId[k]);
				}
			}
			free(hackersArray[i]->rivalsId);
			free(hackersArray[i]);
		}
	}
	free(hackersArray);
}

void updateStudentHackers(Hacker* hackers, Student* students,int numHackers, int numStudents) {
	int currIndex = NULL;
	for (int i = 0; i < numHackers; i++)
	{
		currIndex = findHackerInStudentArr(hackers[i]->studentId, students, numStudents);
		if (currIndex != NOT_FOUND) {
			students[currIndex]->hacker = hackers[i];
		}
	}
}

Hacker* parseHacker(FILE* hackers, EnrollmentSystem enrollmentSystem){
	Hacker* hackersArray = NULL, currHacker = malloc(sizeof(Hacker));
	int currSize = 0, currStage = 0, numHackers = 0;
	char inputChar = 0;
	char* currInput = NULL;
	bool notEmpty = false;
	while (inputChar != EOF) {
		inputChar = fgetc(hackers);
		if (inputChar == EOF || inputChar == '\n') {
			if (!notEmpty) {
				free(currHacker);
			}
			else if (updateHacker(currInput, currStage, currHacker) == FAILED) {
					free(currHacker);
					freeAllHackers(hackersArray, numHackers);
					return NULL;
			}
			else if ((hackersArray = realloc(hackersArray, numHackers+1)) == NULL) {
					free(currHacker);
					return NULL;
			}
			else{
				hackersArray[numHackers] = currHacker;
			}
			if (inputChar == '\n') {
				currStage = (currStage + 1) % NUM_HACKER_STAGES;
				if (currStage == HACKER_STAGE_ID) {
					currHacker = malloc(sizeof(Hacker));
					numHackers++;
					enrollmentSystem->hackersNum = numHackers;
				}
			}

		}
		else if (inputChar == ' ') {
			if ((currInput = realloc(currInput, currSize + 1)) == NULL) {
				freeAllHackers(hackersArray, numHackers);
				free(currHacker);
				return NULL;
			}
			currInput[currSize] = '\0';
			if (updateHacker(currInput, currStage, currHacker) == FAILED) {
				free(currHacker);
				freeAllHackers(hackersArray, numHackers);
				return NULL;
			}
			currSize = 0;
			currInput = NULL;

		}
		else if (inputChar != EOF) {
			if (!notEmpty) {
				numHackers++;
				notEmpty = true;
			}
			currSize++;
			if ((currInput = realloc(currInput, currSize)) == NULL) {
				freeAllStudents(hackersArray, numHackers);
				return NULL;
			}
			currInput[currSize - 1] = inputChar;
		}
	}
	return hackersArray;
}

void freeAllCourses(Course* arrayCourse, int numCourses) {
	if (arrayCourse != NULL) {
		for (int i = 0; i < numCourses; i++) {
			if (arrayCourse[i] != NULL) {
				free(arrayCourse[i]->courseNumber);
				free(arrayCourse[i]);
			}
		}
	}
	free(arrayCourse);
}

Course* ParseCourses(FILE* courses, EnrollmentSystem enrollmentSystem) {
	Course* coursesArray = NULL, currCourse = malloc(sizeof(Course));
	int currSize = 0, currStage = 0, numCourses = 0;
	char inputChar = 0;
	char* currInput = NULL;
	bool notEmpty = false;

	while (inputChar != EOF) {
		inputChar = fgetc(courses);
		if (inputChar == EOF || inputChar == '\n') {
			if (!notEmpty) {
				free(currCourse);
			}
			else if ((currInput = realloc(currInput, currSize + 1)) == NULL) {
					freeAllCourses(courses, numCourses);
					free(currCourse);
					return NULL;
			}
			currInput[currSize] = '\0';
			currCourse->courseNumber = currInput;
			if ((coursesArray = realloc(coursesArray, numCourses + 1)) == NULL) {
				free(currCourse);
				return NULL;
			}
			coursesArray[numCourses] = currCourse;
			if(inputChar == '\n'){
				currCourse = malloc(sizeof(Course));
				numCourses++;
				enrollmentSystem->coursesNum = numCourses;
			}
		}
		else if (inputChar == ' ') {
			if ((currInput = realloc(currInput, currSize + 1)) == NULL) {
				freeAllCourses(courses, numCourses);
				free(currCourse);
				return NULL;
			}
			currInput[currSize] = '\0';
			currCourse->courseNumber = currInput;
		}
		else if (inputChar != EOF) {
			if (!notEmpty) {
				numCourses++;
				notEmpty = true;
			}
			currSize++;
			if ((currInput = realloc(currInput, currSize)) == NULL) {
				freeAllStudents(coursesArray, numCourses);
				return NULL;
			}
			currInput[currSize - 1] = inputChar;
		}
	}
}

EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers)
{
	EnrollmentSystem enrollmentSystem = malloc(sizeof(EnrollmentSystem));
	Student* studentsArray = ParseStudentFile(students, enrollmentSystem);
	Course* coursesArray = ParseCourses(courses, enrollmentSystem);
	Hacker* hackersArray = parseHacker(hackers, enrollmentSystem);
	IsraeliQueue* enrollmentQueues = NULL;

	enrollmentSystem->courses = coursesArray;
	enrollmentSystem->enrollmentQueues = enrollmentQueues;
	enrollmentSystem->hackers = hackersArray;
	enrollmentSystem->students = studentsArray;

	updateStudentHackers(hackersArray, studentsArray, enrollmentSystem->hackersNum, enrollmentSystem->studentsNum);

	//allocating space for all enrollment queues:
	for (int i = 0; i < enrollmentSystem->coursesNum; i++) {
		enrollmentSystem->enrollmentQueues[i] = malloc(sizeof(IsraeliQueue));
	}

	return enrollmentSystem;
}

int friendshipFunction1(void* p1, void* p2)
{
	Student student1 = (Student)(p1); //new addition!!
	Student student2 = (Student)(p2);

	if (student1->hacker == NULL)
	{
		return NOT_FRIENDS_NOT_RIVALS;
	}

	char** friendsIds = ((Hacker)(student1->hacker))->friendsId;
	char** rivalsIds = ((Hacker)(student1->hacker))->rivalsId;
	int numFriends = ((Hacker)(student1->hacker))->numFriends;
	int numRivals = ((Hacker)(student1->hacker))->numRivals;
	char* studentID = student2->studentId;
	
	for (int i = 0; i < numFriends; i++)
	{
		if (strcmp(friendsIds[i], studentID) == 0)
			return  FRIENDS;
	}
	for (int i = 0; i < numRivals; i++)
	{
		if (strcmp(rivalsIds[i], studentID) == 0)
			return  RIVALS;
	}
	return NOT_FRIENDS_NOT_RIVALS;
}

char lower(char c)
{
	if (c < 'A' && c < 'Z')
		return c - 'A' + 'a';
}

int biggerLen(int len1, int len2)
{
	if (len1 > len2)
		return len1;
	return len2;
}

int sum(bool isCaseSensitive, int len, int lenHacker, int lenStudent, char* studentName, char* hackerName)
{
	int sum = 0;
	char cStudent = 0, cHacker = 0;

	for (int i = 0; i < len; i++) {
		if (i > lenHacker) {
			cHacker = 0;
			cStudent = studentName[i];
		}
		else if (i > lenStudent) {
			cStudent = 0;
			cHacker = hackerName[i];
		}
		else {
			cStudent = studentName[i];
			cHacker = hackerName[i];
		}

		if (isCaseSensitive)
		{
			lower(cStudent);
			lower(cHacker);
		}

		sum += abs(cStudent - cHacker);
	}

	return sum;
}

int friendshipFunction2(void* p1, void* p2, Student* students, int numStudents)
{
	Student student1 = (Student)(p1);
	Student student2 = (Student)(p2);
	char* student1FirstName = student1->name, * student1SurName = student1->surname;
	char* student2FirstName = student2->name, * student2SurName = student2->surname;

	int lenStudent1FirstName = strlen(student1FirstName), lenStudent1SurName = strlen(student1SurName);
	int lenStudent2FirstName = strlen(student2FirstName), lenStudent2SurName = strlen(student2SurName);
	int firstNameLen = 0, surNameLen = 0;
	int sumFirstName = 0, sumSurName = 0;
	bool isCaseSensitive = student1->isCaseSensitive;

	firstNameLen = biggerLen(lenStudent1FirstName, lenStudent2FirstName);
	surNameLen = biggerLen(lenStudent1SurName, lenStudent2SurName);
	
	sumFirstName = sum(isCaseSensitive, firstNameLen, lenStudent1FirstName, lenStudent2FirstName, student1FirstName, student2FirstName);
	sumSurName = sum(isCaseSensitive, surNameLen, lenStudent1SurName, lenStudent2SurName, student1SurName, student2SurName);

	return sumFirstName + sumSurName;
}

int friendshipFunction3(void* p1, void* p2)
{
	Node student1 = (Node)(p1);
	Node student2 = (Node)(p2);

	int id1 = atoi(((Student)(student1->student))->studentId);
	int id2 = atoi(((Student)(student2->student))->studentId);

	return abs(id1 - id2);

}

int findStudent(Student* students, char* studentId, int numStudents)
{
	for (int i = 0; i < numStudents; i++)
	{
		if (strcmp(studentId, students[i]->studentId) == 0)
			return i;
	}
	return NOT_FOUND;
}

int findCourse(Course* courses, char* numCourse, int numOfCourses)
{
	for (int i = 0; i < numOfCourses; i++)
	{
		if (strcmp(numCourse, courses[i]->courseNumber) == 0)
			return i;
	}
	return NOT_FOUND;
}

void EnqueueStudent(EnrollmentSystem sys, IsraeliQueue queue, char* id) {
	IsraeliQueueEnqueue(queue, sys->students[findStudent(sys->students, id, sys->studentsNum)]);
}

void ParseQueues(FILE* queues, EnrollmentSystem enrollmentSystem) {
	IsraeliQueue currQueue = NULL;
	int currSize = 0;
	char inputChar = 0;
	char* currInput = NULL;
	bool notEmpty = false, isNextCourseNum = true;

	while (inputChar != EOF) {
		inputChar = fgetc(queues);
		if (inputChar == EOF || inputChar == '\n') {
			isNextCourseNum = true;
			if (notEmpty) {
				if ((currInput = realloc(currInput, currSize + 1)) == NULL) {
					return;
				}
				currInput[currSize] = '\0';
				currSize = 0;
				EnqueueStudent(enrollmentSystem, currQueue, currInput);
			}
		}
		else if (inputChar == ' ') {
			if ((currInput = realloc(currInput, currSize + 1)) == NULL) {
				return;
			}
			currInput[currSize] = '\0';
			currSize = 0;
			if (isNextCourseNum) { //read the number till now
				isNextCourseNum = false;
				currQueue = enrollmentSystem->enrollmentQueues
					[findCourse(enrollmentSystem, inputChar, enrollmentSystem->coursesNum)];
			}
			else
			{
				EnqueueStudent(enrollmentSystem, currQueue, currInput);
			}
		}
		else if (inputChar != EOF) {
			notEmpty = true;
			currSize++;
			if ((currInput = realloc(currInput, currSize)) == NULL) {
				return;
			}
			currInput[currSize - 1] = inputChar;
		}


	}
}

int comparisonFunc(void* student1, void* student2)
{
	return student1 == student2;
}

EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues)
{
	ParseQueues(queues, sys);
	return sys;
}

void AddFriendshipToAllQueues(EnrollmentSystem sys) {
	for (int i = 0; i < sys->coursesNum; i++) {
		IsraeliQueueAddFriendshipMeasure(sys->enrollmentQueues[i], &friendshipFunction1);
		IsraeliQueueAddFriendshipMeasure(sys->enrollmentQueues[i], &friendshipFunction2);
		IsraeliQueueAddFriendshipMeasure(sys->enrollmentQueues[i], &friendshipFunction3);
	}
}

void WriteFailedMessege(FILE* out, char* id) {
	fprintf(out, "Cannot satisfy constraints for ");
	fprintf(out, id);
}

void writeToFile(FILE* out, EnrollmentSystem sys)
{
	IsraeliQueue currCourse = NULL;
	IsraeliQueue temp = NULL;
	Student currStudent = NULL;
	char* studentId = NULL;
	for (int i = 0; i < sys->coursesNum; i++)
	{
		fputs(((sys->courses)[i])->courseNumber, out);
		fputs(" ", out);
		currCourse = (sys->enrollmentQueues)[i];
		temp = IsraeliQueueClone(currCourse);
		currStudent =(Student)(((Node)(IsraeliQueueDequeue(temp)))->student);
		while(temp != NULL)
		{
			studentId = currStudent->studentId;
			fputs(studentId, out);
			fputs(" ", out);
			currStudent = (Student)(((Node)(IsraeliQueueDequeue(temp)))->student);
		}
		fputs("\n", out);
	}
}
void freeAll(EnrollmentSystem sys)
{
	freeAllCourses(sys->courses, sys->coursesNum);
	freeAllHackers(sys->hackers, sys->hackersNum);
	freeAllStudents(sys->students, sys->studentsNum);
	for (int i = 0; i < sys->coursesNum; i++)
	{
		IsraeliQueueDestroy((sys->enrollmentQueues)[i]);
		free((sys->enrollmentQueues)[i]);
	}
	free(sys->enrollmentQueues);
	free(sys->students);
	free(sys->hackers);
	free(sys->courses);
	free(sys);
}

void hackEnrollment(EnrollmentSystem sys, FILE* out)
{
	Course desiredCourse = NULL;
	IsraeliQueue currQueue = NULL;
	Hacker currHacker = NULL;
	int currIndex =0, location=0, currSize =0, currSuccessCount =0;
	AddFriendshipToAllQueues(sys);
	for (int i = 0; i < sys->hackersNum; i++)
	{
		currHacker = (sys->hackers)[i];
		currSuccessCount = 0;
		for (int j = 0; j < currHacker->numCourses; j++)
		{
			currIndex = findCourse(sys->courses, (currHacker->courseNumbers)[j], currHacker->courseNumbers[j]);
			desiredCourse = sys->courses[currIndex];
			currSize = desiredCourse->size;
			currQueue = sys->enrollmentQueues[currIndex];
			EnqueueStudent(sys, currQueue, currHacker->studentId);
			
			location = findHacker(currHacker->studentId, currQueue);
			if (location < currSize) {
				currSuccessCount++;
			}
		}
		if (currHacker->numCourses == ONE_COURSE && currSuccessCount ==0) { //didnt get the one course wanted - fail!
			WriteFailedMessege(out, currHacker->studentId);
			freeAll(sys);
			return;
		}
		else if (currHacker->numCourses > ONE_COURSE && currSuccessCount < NUM_WANTED_COURSES) { //failed to place current student
			WriteFailedMessege(out, currHacker->studentId);
			freeAll(sys);
			return;
		}


	}
	writeToFile(out, sys);
	freeAll(sys);
}


