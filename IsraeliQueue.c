#include "IsraeliQueue.h"
#include <stdlib.h>
#include <stdbool.h>

int* createFriendsEnemiesArr(IsraeliQueue israeliQueue, void* person);

typedef struct Node_t
{
    struct Node_t *prev;
    struct Node_t *next;
    void* value;
    int friendsAdded;
    int enemiesBlocked;
} * Node;

struct IsraeliQueue_t{
    FriendshipFunction* friendshipFunctions;
    Node queueStart; //where people join the queue
    Node queueEnd; //where people leave the queue
    ComparisonFunction comparisonFunction;
    int friendshipThreshold;
    int rivalryThreshold;
    int length;
};

Node copyNode(Node node){
    Node newNode = NULL;
    newNode->next = node->next;
    newNode->prev = node->prev;
    newNode->next = node->next;
    newNode->friendsAdded = node->friendsAdded;
    newNode->enemiesBlocked = node->enemiesBlocked;
    return newNode;
}

int getFunctionsArrLength(FriendshipFunction* friendshipFunctions){
    int i = 0;
    while(*(i+friendshipFunctions)){
        i++;
    }
    return i;
}

int* createFriendsEnemiesArr(IsraeliQueue israeliQueue, void* person){
    int* arr = (int*) malloc(sizeof(int) * israeliQueue->length);
    for (int i = 0; i < israeliQueue->length; ++i) {
        arr[i] = 0;
    }
    Node current = israeliQueue->queueStart;
    for (int i = 0; i < israeliQueue->length; ++i) {
        int sum = 0;
        int k = 0;
        while(*(k + israeliQueue->friendshipFunctions)){
            int value = israeliQueue->friendshipFunctions[k](person, current->value);
            if(value > israeliQueue->friendshipThreshold){
                arr[i] = 1;
            } else{
                sum += value;
            }
        }
        if (arr[i] != 0){
            if (sum / getFunctionsArrLength(israeliQueue->friendshipFunctions) < israeliQueue->rivalryThreshold){
                arr[i] = -1;
            }
        }
        current = current->next;
    }
    return arr;
}


IsraeliQueue IsraeliQueueCreate(FriendshipFunction * friendshipFunction, ComparisonFunction comparisonFunction,
                                int friendshipThreshold, int rivalryThreshold){
    if (!friendshipFunction || !comparisonFunction) {
        return NULL;
    }
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
    queue->enemiesBlocked = 0;
    queue->friendsAdded = 0;
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

    newIsraeliQueue->length = q->length;

    Node queueStart = NULL;
    queueStart->prev = NULL;
    queueStart->value = q->queueStart->value;
    queueStart->friendsAdded = q->queueStart->friendsAdded;
    queueStart->enemiesBlocked = q->queueStart->enemiesBlocked;

    Node currentNode = queueStart;
    Node currentOriginalNode = q->queueStart->next;

    while (currentOriginalNode){
        Node temp = NULL;
        temp->value = currentOriginalNode->value;
        temp->friendsAdded = currentOriginalNode->friendsAdded;
        temp->enemiesBlocked = currentOriginalNode->enemiesBlocked;
        temp->prev = currentOriginalNode->prev;
        currentNode->next = temp;
        currentOriginalNode = currentOriginalNode->next;
        currentNode = currentNode->next;
    }

    Node queueEnd = NULL;
    queueEnd->value = q->queueEnd->value;
    queueEnd->enemiesBlocked = q->queueEnd->enemiesBlocked;
    queueEnd->friendsAdded = q->queueEnd->friendsAdded;
    queueEnd->prev = q->queueEnd->prev;
    queueEnd->next = NULL;

    newIsraeliQueue->queueStart = queueStart;
    newIsraeliQueue->queueEnd = queueEnd;

    int length = 0, i  = 0;
    while(*(i + q->friendshipFunctions)){
        length++;
        i++;
    }

    FriendshipFunction* newFriendShips = malloc(sizeof(newFriendShips) * (length));
    i = 0;
    while(*(i + q->friendshipFunctions)){
        newFriendShips[i] = q->friendshipFunctions[i];
        i++;
    }
    newIsraeliQueue->friendshipFunctions = newFriendShips;
    newIsraeliQueue->comparisonFunction = q->comparisonFunction;
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
    free(israeliQueue->queueEnd);
    free(israeliQueue);
}

//TODO: add error
IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue israeliQueue, FriendshipFunction friendshipFunction){
    if (!israeliQueue || !friendshipFunction){
        return ISRAELI_QUEUE_ERROR;
    }
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

}

IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue israeliQueue, void * person){
    if (!israeliQueue || !person){
        return ISRAELI_QUEUE_ERROR;
    }
    int* friendsEnemiesArr = createFriendsEnemiesArr(israeliQueue, person);
    int length = israeliQueue->length;

}

