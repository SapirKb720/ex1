#include "IsraeliQueue.h"
#include <math.h>

#define FRIEND 1
#define RIVAL -1

typedef struct node {
	Node next;
	void* student;
	int friends; //amount of friends he let them pass
	int rivals; //amount of rivals he prevented them to pass him in line
	bool improved; //required for the improve function

}*Node;

typedef struct IsraeliQueue_t {
	Node head;
	FriendshipFunction* friendshipFunctions;
	ComparisonFunction comparisonFunction;
	int friendship_th;
	int rivalry_th;
}*IsraeliQueue;

IsraeliQueue IsraeliQueueCreate(FriendshipFunction* friendshipFunctions, ComparisonFunction comparisonFunction,
	int friendship_th, int rivalry_th)
{
	IsraeliQueue q = malloc(sizeof(struct IsraeliQueue_t));
	q->head = NULL;
	q->friendshipFunctions = friendshipFunctions;
	q->comparisonFunction = comparisonFunction;
	q->friendship_th = friendship_th;
	q->rivalry_th = rivalry_th;
	return q;
}

IsraeliQueue IsraeliQueueClone(IsraeliQueue otherQ) {
	IsraeliQueue q = malloc(sizeof(struct IsraeliQueue_t));
	q->head = otherQ->head;
	q->friendshipFunctions = otherQ->friendshipFunctions;
	q->comparisonFunction = otherQ->comparisonFunction;
	q->friendship_th = otherQ->friendship_th;
	q->rivalry_th = otherQ->rivalry_th;
	return q;
}

void IsraeliQueueDestroy(IsraeliQueue queue) {
	//emptying node held in queue:
	while (queue->head) {
		Node toDelete = queue->head;
		queue->head = queue->head->next;
		free(toDelete);
	}
	free(queue->friendshipFunctions);
	free(queue);
}


/*
function finds length of given queue and returns it
*/
int FindQueueLength(Node head)
{
	Node q = head;
	int len = 0;
	while (q) {
		q = q->next;
		len++;
	}
	return len;
}

/*
function checks the relathionship betweeeb two given nodes
recieves the two nodes, the friendship functions and friendship and rivalry thresholds
returns -1 if rivals, 1 if friends or a neutral 0
*/
int CheckRelationship(Node n1, Node n2, FriendshipFunction* friendshipFunctions, int friendship_th, int rivalry_th)
{
	int sumFunctions = 0, numFunctions = 0, currResult = 0, index=0;
	if (n2->friends < FRIEND_QUOTA)
	{
		while (friendshipFunctions[index])
		{
			numFunctions++;
			currResult = (*friendshipFunctions)(n1->student, n2->student); 
			if (currResult > friendship_th)
			{
				n2->friends++;
				return FRIEND;
			}
			sumFunctions += currResult;
			index++;
		}
	}

	if (n2->friends < RIVAL_QUOTA)
	{
		if ((sumFunctions / numFunctions) < rivalry_th)
		{
			n2->rivals++;
			return RIVAL;
		}
	}

	return 0;
}

IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue queue, void* item)
{
	if (!item || !queue) {
		return ISRAELIQUEUE_BAD_PARAM;
	}
	int len = FindQueueLength(queue->head);
	int j = 0, friendIndex = len;
	int* friendRivalArray = malloc(sizeof(int) * len);
	Node curr = queue->head;
	Node newNode = NULL;
	if (newNode = malloc(sizeof(Node)) == NULL) {
		return ISRAELIQUEUE_ALLOC_FAILED;
	}
	newNode->friends = 0;
	newNode->rivals = 0;
	newNode->improved = false;
	newNode->student = item;

	if (!friendRivalArray) {
		return ISRAELIQUEUE_ALLOC_FAILED;
		free(newNode);
	}

	while (curr) {
		friendRivalArray[j] = CheckRelationship(newNode, curr, queue->friendship_th, queue->rivalry_th);
		j++;
		curr = curr->next;
	}

	for (int i = 0; i < len; i++) {
		if (friendIndex == len && friendRivalArray[i] == FRIEND) {
			friendIndex = i;
		}
		else if (friendRivalArray[i] == RIVAL) {
			friendIndex = len;
		}
	}
	j = 0;
	curr = queue->head;
	while (curr && j < friendIndex) {
		curr = curr->next;
	}

	newNode->next = curr->next;
	curr->next = newNode;

	return ISRAELIQUEUE_SUCCESS;
}

/*
function: counts num of functions in given friendship function array
input: an array of friendship function
output: length of given array
*/
int countFrienshipArrLen(FriendshipFunction* friendshipfunctions)
{
	int count = 0;
	FriendshipFunction* f = friendshipfunctions;
	while (*f != NULL)
	{
		count++;
		f++;
	}
	return count;
}

IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue queue, FriendshipFunction function)
{
	if (!function || !queue) {
		return ISRAELIQUEUE_BAD_PARAM;
	}
	int len = countFrienshipArrLen(queue->friendshipFunctions);
	FriendshipFunction* newFunctions = malloc(sizeof(FriendshipFunction) * (len + 1));
	if (!newFunctions)
	{
		return ISRAELIQUEUE_ALLOC_FAILED;
	}
	for (int i = 0; i < len; i++)
	{
		newFunctions[i] = queue->friendshipFunctions[i];
	}
	newFunctions[len] = function;
	newFunctions[len + 1] = NULL;
	free(queue->friendshipFunctions); //freeing current array
	queue->friendshipFunctions = newFunctions;

	return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue queue, int friendship_th)
{
	if (queue == NULL)
		return ISRAELIQUEUE_BAD_PARAM;
	queue->friendship_th = friendship_th;
	return ISRAELIQUEUE_SUCCESS;
}

int IsraeliQueueSize(IsraeliQueue queue)
{
	return FindQueueLength(queue);
}


bool IsraeliQueueContains(IsraeliQueue queue, void* item)
{
	Node q = queue;

	while (q) {
		q = q->next;
		if (q->student == item) {
			return true;
		}
	}
	return false;
}

int countImproved(IsraeliQueue queue)
{
	if (queue == NULL || queue->head == NULL)
		return 0;
	int counter = 0;
	int len = IsraeliQueueSize(queue);
	Node curr = queue->head;

	for (int i = 0; i < len; i++)
	{
		if (curr->improved)
			counter++;
		curr = curr->next;
	}
	return counter;
}

void* notImproved(IsraeliQueue queue)
{
	if (queue == NULL)
		return NULL;
	Node curr = queue->head;
	if (curr == NULL)
		return NULL;
	while (curr->next != NULL && !queue->head->improved)
	{
		curr = curr->next;
	}
}

IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue queue)
{
	
	if (queue == NULL)
		return ISRAELIQUEUE_BAD_PARAM;

	int len = IsraeliQueueSize(queue);
	int notImprovedYet = 0, currNotImproved = 0;
	Node curr = queue->head;
	Node temp = NULL;
	while (countImproved(queue) != len)
	{
		curr = queue->head;
		currNotImproved = 0;
		notImprovedYet = len - countImproved(queue);
		if (notImprovedYet == 1 && !curr->improved)
		{
			queue->head = queue->head->next;
			IsraeliQueueEnqueue(queue, curr->student);
			free(curr);
		}
		while (!curr->next->improved && currNotImproved < notImprovedYet)
		{
			if (!curr->next->improved) {
				currNotImproved++;
			}
			curr = curr->next;
		}
		temp = curr->next;
		curr->next = curr->next->next;
		IsraeliQueueEnqueue(queue, temp->student);
		free(temp);
	}

	return ISRAELIQUEUE_SUCCESS;

}

/*
function: counts num of queues in given queue array
input: an array of queues
output: length of given array
*/
int countQueuesArray(IsraeliQueue* queue)
{
	int count = 0;
	IsraeliQueue* q = queue;
	while (*q != NULL)
	{
		count++;
		q++;
	}
	return count;
}

/*
function: counts total amount of friendship functions in all queues in given queue array
input:  an array of israeli queues
output: total amount of friendship functions in all queues in given queue array
*/
int countFriendshipFunctions(IsraeliQueue* queue)
{
	int count = 0;
	IsraeliQueue* q = queue;
	while (*q != NULL)
	{
		count += countFrienshipArrLen((*q));
		q++;
	}
	return count;
}

/*
function: merges all friendship functions of queues in given queue array into one big friendshipFunctions array
input: an array of queues and the array of friendshipfunctions to merge into
output: none
*/
void mergeFriendshipFunctions(IsraeliQueue* queue, FriendshipFunction* friendshipFunctions)
{
	int i = 0;
	IsraeliQueue* currQueue = queue;
	FriendshipFunction* currFunc = friendshipFunctions;
	while (*currQueue != NULL)
	{
		currFunc = (*currQueue)->friendshipFunctions;
		while (*currFunc != NULL)
		{
			*(friendshipFunctions + i) = *currFunc;
			i++;
			currFunc++;
		}
		currQueue++;
	}
	*(friendshipFunctions + i) = NULL; //putting null at the end
}

/*
function: mergers all queues in given israeliqueue array into one queue
iput: an array of queues, new queue to merge into
output: none
*/
void mergeQueues(IsraeliQueue* queue, IsraeliQueue newQueue)
{
	int numOfQueues = countQueuesArray(queue);
	Node* nodes = malloc(sizeof(Node) * numOfQueues);
	bool isAllNull = false;

	while (!isAllNull) {
		isAllNull = true;
		for (int i = 0; i < numOfQueues; i++) {
			if ((*(nodes + i)) != NULL) {
				isAllNull = false;
				IsraeliQueueEnqueue(queue, *(nodes + i));
				*(nodes + i) = (*(nodes + i))->next;
			}
		}
	}
}

IsraeliQueue IsraeliQueueMerge(IsraeliQueue* queue, ComparisonFunction comparisonFunction)
{
	int numOfQueues = countQueuesArray(queue);
	int friendshipSum = 0, rivaltyMultiple = 1;
	int friendshipArrayTotal = countFriendshipFunctions(queue);
	FriendshipFunction* newFriendshipFunctions = malloc(sizeof(FriendshipFunction) * (friendshipArrayTotal + 1));
	if (newFriendshipFunctions == NULL) { //ERROR!!
		return NULL;
	}
	mergeFriendshipFunctions(queue, newFriendshipFunctions);

	for (int i = 0; i < numOfQueues; i++) {
		friendshipSum += (*(queue + i))->friendship_th;
		rivaltyMultiple *= abs((*(queue + i))->rivalry_th);
	}

	int friendshipAvg = friendshipSum / numOfQueues;
	//geometrical average calculating (powering by 1/num, then rounding up to an int:  )
	int rivaltyAvg = (int)(ceil(pow((double)(rivaltyMultiple), 1 / (double)(numOfQueues))));

	IsraeliQueue newQueue = IsraeliQueueCreate(newFriendshipFunctions, comparisonFunction, friendshipAvg, rivaltyAvg);
	mergeQueues(queue, newQueue); 
	return newQueue;

}

