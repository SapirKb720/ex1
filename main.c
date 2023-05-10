#include "HackEnrollment.h"

#define NUM_WITH_FLAG 6
#define NUM_NO_FLAG 5

#define FLAG_INDEX 0
#define STUDENT_FILE_INDEX 1
#define COURSES_FILE_INDEX 2
#define HACKERS_FILE_INDEX 3
#define QUEUES_FILE_INDEX 4
#define OUT_FILE_INDEX 5

int main(int argc, char* argv[])
{
	int index_margin = -1;
	if (argc == NUM_WITH_FLAG) {
		index_margin = 0;
	}
	FILE* studentsFile = fopen(argv[STUDENT_FILE_INDEX + index_margin], "r");
	FILE* hackersFile = fopen(argv[HACKERS_FILE_INDEX + index_margin], "r");
	FILE* queuesFile = fopen(argv[QUEUES_FILE_INDEX + index_margin], "r");
	FILE* coursesFile = fopen(argv[COURSES_FILE_INDEX + index_margin], "r");
	FILE* outFile = fopen(argv[OUT_FILE_INDEX + index_margin], "w");

	EnrollmentSystem sys= createEnrollment(studentsFile,coursesFile,hackersFile);
	if (argc == NUM_WITH_FLAG){
		updateIsCaseSensitive(sys,strcmp(argv[FLAG_INDEX], "-i") == 0);
	}
	sys = readEnrollment(sys, queuesFile);
	hackEnrollment(sys, outFile);

	fclose(studentsFile);
	fclose(hackersFile);
	fclose(queuesFile);
	fclose(coursesFile);
	fclose(outFile);

	return 0;
}