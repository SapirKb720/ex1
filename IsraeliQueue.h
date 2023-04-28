#ifndef PROVIDED_ISRAELIQUEUE_H
#define PROVIDED_ISRAELIQUEUE_H

#include <stdbool.h>
#include <stdlib.h>

#define FRIEND_QUOTA 5
#define RIVAL_QUOTA 3

#define FRIEND 1
#define RIVAL -1

typedef struct node {
	Node next;
}*Node;

typedef int (*FriendshipFunction)(void*, void*);
typedef int (*ComparisonFunction)(void*, void*);

typedef struct IsraeliQueue_t {
	Node head;
	FriendshipFunction* friendshipFunctions;
	ComparisonFunction comparisonFunction;
	int friendship_th;
	int rivalry_th;
}*IsraeliQueue;

typedef enum { ISRAELIQUEUE_SUCCESS, ISRAELIQUEUE_ALLOC_FAILED, ISRAELIQUEUE_BAD_PARAM, ISRAELI_QUEUE_ERROR } IsraeliQueueError;

/**Error clarification:
 * ISRAELIQUEUE_SUCCESS: Indicates the function has completed its task successfully with no errors.
 * ISRAELIQUEUE_ALLOC_FAILED: Indicates memory allocation failed during the execution of the function.
 * ISRAELIQUEUE_BAD_PARAM: Indicates an illegal parameter was passed.
 * ISRAELI_QUEUE_ERROR: Indicates any error beyond the above.
 * */

/**Creates a new IsraeliQueue_t object with the provided friendship functions, a NULL-terminated array,
 * comparison function, friendship threshold and rivalry threshold. Returns a pointer
 * to the new object. In case of failure, return NULL.*/
IsraeliQueue IsraeliQueueCreate(FriendshipFunction * friendshipFunctions, ComparisonFunction comparisonFunction,
									int friendship_th, int rivalry_th);

/**Returns a new queue with the same elements as the parameter. If the parameter is NULL or any error occured during
 * the execution of the function, NULL is returned.*/
IsraeliQueue IsraeliQueueClone(IsraeliQueue otherQ);

/**@param IsraeliQueue: an IsraeliQueue created by IsraeliQueueCreate
 *
 * Deallocates all memory allocated by IsraeliQueueCreate for the object pointed to by
 * the parameter.*/
void IsraeliQueueDestroy(IsraeliQueue queue);

/**@param IsraeliQueue: an IsraeliQueue in which to insert the item.
 * @param item: an item to enqueue
 *
 * Places the item in the foremost position accessible to it.*/
IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue queue, void* item);

/**@param IsraeliQueue: an IsraeliQueue to which the function is to be added
 * @param FriendshipFunction: a FriendshipFunction to be recognized by the IsraeliQueue
 * going forward.
 *
 * Makes the IsraeliQueue provided recognize the FriendshipFunction provided.*/
IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue queue, FriendshipFunction function);

/**@param IsraeliQueue: an IsraeliQueue whose friendship threshold is to be modified
 * @param friendship_threshold: a new friendship threshold for the IsraeliQueue*/
IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue queue, int friendship_th);

/**@param IsraeliQueue: an IsraeliQueue whose rivalry threshold is to be modified
 * @param friendship_threshold: a new rivalry threshold for the IsraeliQueue*/
IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue queue, int rivalry_th);

/**Returns the number of elements of the given queue. If the parameter is NULL, 0
 * is returned.*/
int IsraeliQueueSize(IsraeliQueue queue);

/**Removes and returns the foremost element of the provided queue. If the parameter
 * is NULL or a pointer to an empty queue, NULL is returned.*/
void* IsraeliQueueDequeue(IsraeliQueue queue);

/**@param item: an object comparable to the objects in the IsraeliQueue
 *
 * Returns whether the queue contains an element equal to item. If either
 * parameter is NULL, false is returned.*/
bool IsraeliQueueContains(IsraeliQueue queue, void* item);

/**Advances each item in the queue to the foremost position accessible to it,
 * from the back of the queue frontwards.*/
IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue queue);

/**@param q_arr: a NULL-terminated array of IsraeliQueues
 * @param ComparisonFunction: a comparison function for the merged queue
 *
 * Merges all queues in q_arr into a single new queue, with parameters the parameters described
 * in the exercise. Each queue in q_arr enqueues its head in the merged queue, then lets the next
 * one enqueue an item, in the order defined by q_arr. In the event of any error during execution, return NULL.*/
IsraeliQueue IsraeliQueueMerge(IsraeliQueue* queue,ComparisonFunction comparisonFunction);

/*
function finds length of given queue and returns it
*/
int FindQueueLength(Node head);

/*
function checks the relathionship betweeeb two given nodes
recieves the two nodes, the friendship functions and friendship and rivalry thresholds
returns -1 if rivals, 1 if friends or a neutral 0
*/
int CheckRelationship(Node n1, Node n2, FriendshipFunction* friendshipFunctions, int friendship_th, int rivalry_th);

#endif //PROVIDED_ISRAELIQUEUE_H