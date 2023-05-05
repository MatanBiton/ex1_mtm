#ifndef EX1_MTM_HACKENROLLMENT_H
#define EX1_MTM_HACKENROLLMENT_H

#include <stdlib.h>
#include <stdio.h>

typedef struct EnrollmentSystem EnrollmentSystem;
typedef struct Student Student;
typedef struct Hacker Hacker;
typedef struct Course Course;

EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers);

EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues);

void hackEnrollment(EnrollmentSystem sys, FILE* out);

#endif //EX1_MTM_HACKENROLLMENT_H
