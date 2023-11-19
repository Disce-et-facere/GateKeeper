#ifndef TAGHANDLER_H
#define TAGHANDLER_H
#include <stdbool.h>

typedef struct {
    char name[256];  // ex Gunnar Arbo 
    char idS[2];     // ex C = customer V = Visitor E = Employee
    int idD;         // ex 1,2,3,4 -> etc
    char pass[17];   // Random 16 char pass     
    int access;      // 0 = true 1 = false
} TAG;


// void something(TAG *tag); 

#endif // TAGHANDLER_H
