COMPILER = gcc
TOOL_PATH = tool
EX_PATH = /new_home/courses/mtm/public/2223b/ex1
OBJECT_FILES = main.o IsraeliQueue.o HackerEnrollement.o
EXECUTEC = main
EXECUTABLE_NAME = HackEnrollment
FLAGS = -std=c99 -Im -I$(EX_PATH) -I$(TOOL_PATH) -Wall -pedantic-errors -g -Werror -DNDEBUG
COMPILE = $(COMPILER) &(FLAGS) -c &(TOOL_PATH)/$*.c -o $@

$(EXECUTEC) : $(OBJECT_FILES)
	$(COMPILER) $(FLAGS) $(OBJECT_FILES) -o $(EXECUTABLE_NAME)


IsraeliQueue.o : IsraeliQueue.c &(EX_PATH)/IsraeliQueue.h
	gcc -c IsraeliQueue.c

HackerEnrollment.o : $(TOOL_PATH)/HackerEnrollment.c $(TOOL_PATH)/HackerEnrollment.h $(EX_PATH)/IsraeliQueue.h
	$(COMPILE)

main.o : $(TOOL_PATH)/main.c IsraeliQueue.h $(TOOL_PATH)/HackerEnrollment.h
	$(COMPILE)

clean:
	rm -f $(OBJECT_FILES) &(EXECUTABLE_NAME)