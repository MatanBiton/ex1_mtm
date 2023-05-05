#include "HackEnrollment.h"
#include "IsraeliQueue.h"
#include <stdio.h>
#include <stdlib.h>


//5 if numbers are equal
int friendships_function1(void *item1, void *item2) {
    if (*((int *) item1) == *((int *) item2)) {
        return 5;
    }

    return 0;
}

// 5 if item1 -3 < item2< item1+3
int friendships_function2(void *item1, void *item2) {
    if (*((int *) item1) - 3 < *((int *) item2) && *((int *) item2) < *((int *) item1) + 3) {
        return 5;
    }

    return 0;
}

int comparison_function(void *item1, void *item2) {
    if (*((int *) item1) == *((int *) item2)) {
        return 1;
    }

    return 0;
}



int main() {
    FriendshipFunction* f1 = malloc(3*sizeof(FriendshipFunction));
    *f1 = friendships_function1;
    *(f1 + 1) = friendships_function2;
    *(f1 + 2) = NULL;

    ComparisonFunction f2 = comparison_function;

    IsraeliQueue q1 = IsraeliQueueCreate(f1, f2, 200, 100);
    IsraeliQueue q2 = IsraeliQueueCreate(f1, f2, 100, 100);

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 0;
    int e = 0;

    int* item1 = &a;
    int* item2 = &b;
    int* item3 = &c;
    int* item4 = &d;
    int* item5 = &e;

    IsraeliQueueEnqueue(q1, item1);
    IsraeliQueueEnqueue(q1, item2);
    IsraeliQueueEnqueue(q1, item3);
    IsraeliQueueEnqueue(q2, item4);
    IsraeliQueueEnqueue(q2, item5);

    IsraeliQueue qArr[3] = {q1, q2, NULL};

    IsraeliQueue q3 = IsraeliQueueMerge(qArr, f2);

    IsraeliQueueUpdateFriendshipThreshold(q1, 2);
    IsraeliQueueUpdateRivalryThreshold(q1, 5);

    int sizeQ1 = IsraeliQueueSize(q2);
    printf("%d", sizeQ1);






    /*FILE *studentsFile = fopen("C:\\Users\\Lior\\Programming\\untitled4\\students.txt", "r");

    Student *studentsArr = readStudents(studentsFile);

    Student student1 = studentsArr[0];
    Student student2 = studentsArr[1];
    Student student3 = studentsArr[2];
    if (studentsArr[3] == NULL) {
        printf("1");
    }

    fclose(studentsFile);

    FILE *coursesFile = fopen("C:\\Users\\Lior\\Programming\\untitled4\\courses.txt", "r");

    Course *coursesArr = readCourses(coursesFile);
    Course course1 = coursesArr[0];
    Course course2 = coursesArr[1];
    Course course3 = coursesArr[2];
    Course course4 = coursesArr[3];
    Course course5 = coursesArr[4];

    FILE *hackersFile = fopen("C:\\Users\\Lior\\Programming\\untitled4\\hackers.txt", "r");

    Student *hackersArr = readHackers(hackersFile);
    Student hacker1 = hackersArr[0];
    Student hacker2 = hackersArr[1];
    Student hacker3 = hackersArr[2];

    updateHackerDetails(studentsArr, hackersArr);
    int i =0 ;

    studentsFile = fopen("C:\\Users\\Lior\\Programming\\untitled4\\students.txt", "r");
    coursesFile = fopen("C:\\Users\\Lior\\Programming\\untitled4\\courses.txt", "r");
    hackersFile = fopen("C:\\Users\\Lior\\Programming\\untitled4\\hackers.txt", "r");
    FILE *queuesFile = fopen("C:\\Users\\Lior\\Programming\\untitled4\\queues.txt", "r");

    EnrollmentSystem enrollmentSystem = createEnrollment(studentsFile, coursesFile, hackersFile);

    enrollmentSystem = readEnrollment(enrollmentSystem, queuesFile);*/


}
