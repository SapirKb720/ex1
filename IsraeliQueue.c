#include "IsraeliQueue.h"

IsraeliQueue IsraeliQueueCreate(FriendshipFunction* friendshipFunctions, ComparisonFunction comparisonFunction,
	int friendship_th, int rivalry_th)
{
	IsraeliQueue q = malloc(sizeof(*IsraliQueue));
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
	free(queue->friendshipFunctions)
	free(queue);
}

int FindQueueLength(Node head)
{
	int len = 0;
	while (head) {
		heah = head->next;
		len++;
	}
	return len;
}

int CheckRelationship(Node n1, Node n2, FriendshipFunction* friendshipFunctions, int friendship_th, int rivalry_th)
{
	int sumFunctions = 0, numFunctions = 0, currResult=0;
	while (friendshipFunctions)
	{
		numFunctions++;
		currResult = (*friendshipFunctions)(n1, n2); //does it work like that???????????????????????????????????????
		if (currResult > friendship_th)
		{
			return FRIEND;
		}
		sumFunctions += currResult;
	}

	if ((sumFunctions / numFunctions) < rivalry_th)
	{
		return RIVAL;
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

	if (!friendRivalArray){
		return ISRAELIQUEUE_ALLOC_FAILED;
	}

	while (curr){
		friendRivalArray[j] = CheckRelationship(item, curr, queue->friendship_th, queue->rivalry_th);
		j++;
		curr = curr->next;
	}

	for (int i = 0; i < len; i++){
		if (friendIndex == len && friendRivalArray[i] == FRIEND){
			friendIndex = i;
		}
		else if (friendRivalArray[i] == RIVAL){
			friendIndex = len;
		}
	}
	j = 0;
	curr = queue->head;
	while (curr && j < friendIndex){
		curr = curr->next;
	}
	
	item->next = curr->next;
	curr->next = item;

	return ISRAELIQUEUE_SUCCESS;
}

int countFunctions(FriendshipFunction* functions) {
	int i = 0;
	if (!funtions)
	{
		return 0;
	}
	while (functions[i])
	{
		i++;
	}
	return i;
}

IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue queue, FriendshipFunction function)
{
	if (!function || !queue) {
		return ISRAELIQUEUE_BAD_PARAM;
	}
	int len = countFunctions(queue->friendshipFunctions);
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
	newFunctions[len + 1]  NULL;
	free(queue->friendshipFunctions); //freeing current array
	queue->friendshipFunctions = newFunctions;

	return ISRAELIQUEUE_SUCCESS;
}
