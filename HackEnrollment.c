#include "HackEnrollment.h"


struct Student{
    char id[9];
    int totalCredit;
    double gpa;
    char* firstName;
    char* surName;
    char* city;
    char* department;

};


struct Hacker{
    char id[9];
    int totalCredit;
    double gpa;
    char* firstName;
    char* surName;
    char* city;
    char* department;
    char** desiredCourses;
    char* friendsIds[9];
    char* rivalsIds[9];
};

struct Course {
    char* courseNumber;
    int size;
};

struct EnrollmentSystem{
    Student* students;
    Course* courses;
};

static Student* readStudents(FILE* students);
static Course* readCourses(FILE* courses);
static Student createStudent(char* id, char* totalCredit,
                             char* gpa, char* firstName, char* surName, char* city, char* department);

EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers){
    EnrollmentSystem sys;
    sys.students = readStudents(students);
    sys.courses = readCourses(courses);

    return sys;
}

static Student* readStudents(FILE* students){
    char* output;
    char result = (char)fgetc(students);
    while (result != EOF){
        ungetc(result, students);
        char* id, *totalCredit, *gpa, *firstName, *surName, *city, *department;
        fscanf(students, "%ms", &id);
        fscanf(students, "%ms", &totalCredit);
        fscanf(students, "%ms", &gpa);
        fscanf(students, "%ms", &firstName);
        fscanf(students, "%ms", &surName);
        fscanf(students, "%ms", &city);
        fscanf(students, "%ms", &department);
        Student student = createStudent(id, totalCredit, gpa, firstName, surName, city, department);
        result = (char)fgetc(students);
    }
}

static Course* readCourses(FILE* courses){

}

static Student createStudent(char* id, char* totalCredit,
                             char* gpa, char* firstName, char* surName, char* city, char* department){

}