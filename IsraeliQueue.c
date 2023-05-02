#include "IsraeliQueue.h"
#include <stdlib.h>
#include <stdbool.h>
#ifndef PROVIDED_MATH_H
#define PROVIDED_MATH_H
#include <math.h>
#endif

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

int* createFriendsEnemiesArr(IsraeliQueue israeliQueue, void* person);
bool inUsed(void** used, void* person);
void** updateUsedArray(void** used, int size);
int getTotalLengths(IsraeliQueue* israeliQueues);
void setAllFriendshipFunctions(IsraeliQueue* israeliQueues, IsraeliQueue newIsraeliQueue);
void setFriendshipThreshold(IsraeliQueue* israeliQueues, IsraeliQueue newIsraeliQueue);
void setRivalryThreshold(IsraeliQueue* israeliQueues, IsraeliQueue newIsraeliQueue);
int getMaxLength(IsraeliQueue* israeliQueue);
Node getNodeAtPosition(Node start, int position);

int getFunctionsArrLength(FriendshipFunction* friendshipFunctions){
    int i = 0;
    while(*(i+friendshipFunctions)){
        i++;
    }
    return i;
}


//create arr with markers for each position's relation to person:
//friend => arr[i] = 1
//enemy => arr[i] = -1
//neutral => arr[i] = 0
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
            if (sum / getFunctionsArrLength(israeliQueue->friendshipFunctions)
            < israeliQueue->rivalryThreshold){
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
    Node queue = malloc(sizeof(queue));
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

    Node queueStart = malloc(sizeof(queueStart));
    queueStart->prev = malloc(sizeof(queueStart->prev));
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
        return ISRAELIQUEUE_ALLOC_FAILED;
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
    free(israeliQueue->friendshipFunctions);
    newFunctions[n] = friendshipFunction;
    israeliQueue->friendshipFunctions = newFunctions;
    free(temp);
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue israeliQueue, int friendshipThreshold){
    if (!israeliQueue) {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    israeliQueue->friendshipThreshold = friendshipThreshold;
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue israeliQueue, int rivalryThreshold){
    if (!israeliQueue) return ISRAELIQUEUE_BAD_PARAM;
    israeliQueue->rivalryThreshold = rivalryThreshold;
    return ISRAELIQUEUE_SUCCESS;
}

//TODO: possibly use this instead of .length
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
            free(temp);
            return true;
        }
        temp = temp->next;
    }
    free(temp);
    return false;
}

void* IsraeliQueueDequeue(IsraeliQueue israeliQueue){
    if(!israeliQueue || israeliQueue->length == 0){
        return NULL;
    }
    Node last = israeliQueue->queueEnd;
    last->prev->next = NULL;
    israeliQueue->queueEnd = last->prev;
    israeliQueue->length--;
    return last;
}

//TODO: minimize lines
IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue israeliQueue, void * person){
    Node newPerson = malloc(sizeof(newPerson));
    if (!newPerson){
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    if (!israeliQueue || !person){
        return ISRAELIQUEUE_BAD_PARAM;
    }
    int* friendsEnemiesArr = createFriendsEnemiesArr(israeliQueue, person);
    int length = israeliQueue->length;
    Node current = israeliQueue->queueEnd;
    int index = length - 1;
    if (index < 0){
        newPerson->value = person;
        newPerson->enemiesBlocked=0;
        newPerson->friendsAdded=0;
        newPerson->next = NULL;
        newPerson->prev = NULL;
        israeliQueue->queueStart = newPerson;
        israeliQueue->queueEnd = newPerson;
        israeliQueue->length++;
        return ISRAELIQUEUE_SUCCESS;
    }
    bool first = true;
    while(current && index >= 0){
        if (friendsEnemiesArr[index] == 1 && current->friendsAdded < FRIEND_QUOTA){
            Node optionalPosition = current;
            if (index != length - 1){
                first = false;
            }
            while(index >= 0 && friendsEnemiesArr[index] != -1 ||
                (index >= 0 && friendsEnemiesArr[index] == -1 && current->enemiesBlocked >= RIVAL_QUOTA)){
                index--;
                current = current->prev;
            }
            if (index < 0){
                newPerson->value = person;
                newPerson->enemiesBlocked=0;
                newPerson->friendsAdded=0;
                newPerson->next = optionalPosition->next;
                newPerson->prev = optionalPosition;
                optionalPosition->next = newPerson;
                optionalPosition->friendsAdded++;
                if (first){
                    israeliQueue->queueEnd = newPerson;
                }
                return ISRAELIQUEUE_SUCCESS;
            } else{
                current->enemiesBlocked++;
                current = current->prev;
                index--;
            }
        } else{
            current = current->prev;
            index--;
        }
    }//TODO: may be irrelevant
    if (index <= 0){
        newPerson->next = israeliQueue->queueStart;
        newPerson->prev = NULL;
        newPerson->value = person;
        newPerson->enemiesBlocked = 0;
        newPerson->friendsAdded = 0;
        israeliQueue->queueStart->prev = newPerson;
        israeliQueue->queueStart = newPerson;
        return ISRAELIQUEUE_SUCCESS;
    }
}

IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue israeliQueue){
    if (!israeliQueue){
        return ISRAELIQUEUE_BAD_PARAM;
    }
    void** used = malloc(sizeof(void*));
    if (!used){
        free(used);
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    int index = 0;
    Node current = israeliQueue->queueStart;
    while (current){
        if (!inUsed(used, current->value)) {
            current->next->prev = current->prev;
            IsraeliQueueEnqueue(israeliQueue, current->value);
            used[index] = current->next;
            used = updateUsedArray(used, index+1);
            if (!used){
                free(used);
                free(current);
                return ISRAELIQUEUE_ALLOC_FAILED;
            }
            index++;
        }
        current = current->next;
    }
    return ISRAELIQUEUE_SUCCESS;
}


//TODO: add friendship functions now
IsraeliQueue IsraeliQueueMerge(IsraeliQueue* israeliQueues,ComparisonFunction comparisonFunction){
    if (!israeliQueues || !comparisonFunction){
        return NULL;
    }
    int maxLength = getMaxLength(israeliQueues);
    IsraeliQueue newIsraeliQueue = malloc(sizeof(newIsraeliQueue));
    newIsraeliQueue->length = getTotalLengths(israeliQueues);
    setAllFriendshipFunctions(israeliQueues, newIsraeliQueue);
    setFriendshipThreshold(israeliQueues, newIsraeliQueue);
    setRivalryThreshold(israeliQueues, newIsraeliQueue);
    newIsraeliQueue->comparisonFunction = comparisonFunction;
    bool first = true;
    Node current, newNode;
    for (int i = 0; i < maxLength; ++i) {
        int currentIndex = 0;
        while(*(currentIndex + israeliQueues)){
            if (i < israeliQueues[currentIndex]->length) {
                if (first) {
                    first = false;
                    current = getNodeAtPosition(israeliQueues[currentIndex]->queueStart, i);
                    newIsraeliQueue->queueStart = current;
                    current = current->next;
                } else{
                    newNode = getNodeAtPosition(israeliQueues[currentIndex]->queueStart, i);
                    newNode->prev = current;
                    current->next = newNode;
                    current = current->next;
                }
            }
            currentIndex++;
        }
    }
    free(current);
    free(newNode);
    newIsraeliQueue->queueEnd = getNodeAtPosition(newIsraeliQueue->queueStart, newIsraeliQueue->length - 1);
    return newIsraeliQueue;
}

Node getNodeAtPosition(Node start, int position){
    Node current = start;
    int currentPosition = 0;
    while(currentPosition != position){
        current = current->next;
        currentPosition++;
    }
    return current;
}

int getMaxLength(IsraeliQueue* israeliQueues){
    int max = -1;
    int index = 0;
    while(*(israeliQueues + index)){
        max = israeliQueues[index]->length > max ? israeliQueues[index]->length : max;
        index++;
    }
    return max;
}

int getTotalLengths(IsraeliQueue* israeliQueues){
    int sum = 0;
    int index = 0;
    while(*(israeliQueues + index)){
        sum += israeliQueues[index]->length;
        index++;
    }
    return sum;
}

void setAllFriendshipFunctions(IsraeliQueue* israeliQueues, IsraeliQueue newIsraeliQueue){
    int i = 0;
    while (*(i + israeliQueues)){
        int j = 0;
        while (*(j + israeliQueues[i]->friendshipFunctions)){
            IsraeliQueueAddFriendshipMeasure(newIsraeliQueue,
                                             israeliQueues[i]->friendshipFunctions[j]);
            j++;
        }
        i++;
    }
}

void setFriendshipThreshold(IsraeliQueue* israeliQueues,IsraeliQueue newIsraeliQueue){
    int sum = 0, index = 0;
    while (*(israeliQueues+index)){
        sum+= israeliQueues[index]->friendshipThreshold;
        index++;
    }
    IsraeliQueueUpdateFriendshipThreshold(newIsraeliQueue, sum / index);
}

void setRivalryThreshold(IsraeliQueue* israeliQueues,IsraeliQueue newIsraeliQueue){
    int mult = 1, index = 0;
    while(*(index + israeliQueues)){
        mult *= israeliQueues[index]->rivalryThreshold;
        index++;
    }
    IsraeliQueueUpdateRivalryThreshold(newIsraeliQueue, pow(mult, 1./index));
}

void** updateUsedArray(void** used, int size){
    void** newUsed = malloc((size+1)* sizeof(newUsed));
    if (!newUsed){
        free(newUsed);
        return NULL;
    }
    void** temp = used;
    int n = 0;
    while(temp){
        newUsed[n] = temp[n];
        temp++;
        n++;
    }
    free(temp);
    free(used);
    return newUsed;
}


//true - person in used
//TODO: implement
bool inUsed(void** used, void* person){

}

