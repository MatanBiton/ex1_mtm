#include "IsraeliQueue.h"
#include <stdlib.h>

typedef struct Node_t
{
    struct Node_t *prev;
    struct Node_t *next;
    void* value;
} * Node;

struct IsraeliQueue_t{
    FriendshipFunction* friendshipFunctions;
    Node queueStart;
    Node queueEnd;
    ComparisonFunction comparisonFunction;
    int friendshipThreshold;
    int rivalryThreshold;
    int length;
};

IsraeliQueue IsraeliQueueCreate(FriendshipFunction * friendshipFunction, ComparisonFunction comparisonFunction,
                                int friendshipThreshold, int rivalryThreshold){
    if (!friendshipFunction || !comparisonFunction) return NULL;
    IsraeliQueue israeliQueue = malloc(sizeof(*israeliQueue));
    if (!israeliQueue) {
        free(israeliQueue);
        return NULL;
    }
    israeliQueue->friendshipFunctions = friendshipFunction;
    israeliQueue->comparisonFunction = comparisonFunction;
    Node queue = NULL;
    queue->next = NULL;
    queue->value = NULL;
    israeliQueue->queueStart = queue;
    israeliQueue->queueEnd = queue;
    israeliQueue->friendshipThreshold = friendshipThreshold;
    israeliQueue->rivalryThreshold = rivalryThreshold;
    israeliQueue->length = 0;
    return israeliQueue;
}

IsraeliQueue IsraeliQueueClone(IsraeliQueue q){
    if (!q) return NULL;
    IsraeliQueue newIsraeliQueue = malloc(sizeof(*newIsraeliQueue));
    if (!newIsraeliQueue){
        free(newIsraeliQueue);
        return NULL;
    }
    newIsraeliQueue->friendshipThreshold = q->friendshipThreshold;
    newIsraeliQueue->rivalryThreshold = q->rivalryThreshold;
    Node newQueue = NULL;
    Node first = newQueue;
    Node temp = q->queueStart;
    bool isStart = true;
    while(temp){
        if (isStart) {
            newQueue->value = temp->value;
            newQueue->next = temp->next;
            isStart = false;

        } else {
            newQueue->value = temp->value;
            newQueue->next = temp->next;
            newQueue->prev = temp->prev;
        }
        newQueue = (Node) newQueue->next;
        temp = (Node) temp->next;
    }
    newQueue = first;
    newIsraeliQueue->queueStart = newQueue;
    newIsraeliQueue->queueEnd = q->queueEnd;
    FriendshipFunction* newFriendshipFunctions = malloc(q->length* sizeof(FriendshipFunction));
    if (!newFriendshipFunctions){
        free(newFriendshipFunctions);
        return NULL;
    }
    newIsraeliQueue->friendshipFunctions = newFriendshipFunctions;
    int i = 0;
    while (*(q->friendshipFunctions + i)){
        newIsraeliQueue->friendshipFunctions[i] = q->friendshipFunctions[i];
        i++;
    }
    newIsraeliQueue->length = q->length;
    newIsraeliQueue->comparisonFunction = q->comparisonFunction;
    return newIsraeliQueue;
}

void IsraeliQueueDestroy(IsraeliQueue israeliQueue){
    free(israeliQueue->friendshipFunctions);
    Node next = israeliQueue->queueStart;
    for (int i = 0; i < israeliQueue->length; ++i) {
        Node temp = next->next;
        free(next);
        next = temp;
    }
    free(israeliQueue->queueEnd);
}

//TODO: add error
IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue israeliQueue, FriendshipFunction friendshipFunction){
    int n = 0;
    FriendshipFunction* temp = israeliQueue->friendshipFunctions;
    while(temp){
        n++;
        temp+= 1;
    }
    FriendshipFunction * newFunctions = malloc((n + 1)*sizeof(FriendshipFunction));
    int i = 0;
    temp = israeliQueue->friendshipFunctions;
    while(temp){
        newFunctions[i] = temp[i];
        temp+=1;
        i++;
    }
    temp = israeliQueue->friendshipFunctions;
    newFunctions[i] = friendshipFunction;
    israeliQueue->friendshipFunctions = newFunctions;
    free(temp);

    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue israeliQueue, int friendshipThreshold){
    if (!israeliQueue) return ISRAELIQUEUE_BAD_PARAM;
    israeliQueue->friendshipThreshold = friendshipThreshold;
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue israeliQueue, int rivalryThreshold){
    if (!israeliQueue) return ISRAELIQUEUE_BAD_PARAM;
    israeliQueue->rivalryThreshold = rivalryThreshold;
    return ISRAELIQUEUE_SUCCESS;
}

int IsraeliQueueSize(IsraeliQueue israeliQueue){
    if(!israeliQueue){
        return 0;
    }
    return israeliQueue->length;
}

bool IsraeliQueueContains(IsraeliQueue israeliQueue, void * item){
    if (!israeliQueue || !item){
        return false;
    }
    Node temp = israeliQueue->queueStart;
    for (int i = 0; i < israeliQueue->length; ++i) {
        if(israeliQueue->comparisonFunction(temp->value, item)){
            return true;
        }
        temp = temp->next;
    }
    return false;
}

void* IsraeliQueueDequeue(IsraeliQueue israeliQueue){
    if (!israeliQueue || !israeliQueue->queueStart || !israeliQueue->queueEnd){
        return NULL;
    }
    Node temp = israeliQueue->queueEnd;
    Node prev = temp->prev;
    prev->next = NULL;
    void* value = temp->value;
    free(temp);
    return value;
}
