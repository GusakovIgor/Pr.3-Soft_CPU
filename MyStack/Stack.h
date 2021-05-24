#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum errors   { HASH_PROTECTION_ERROR = -7, 
				CANARY_STACK_PROTECTION_ERROR, 
				CANARY_ARRAY_PROTECTION_ERROR, 
                CONSTRUCTION_ERROR, MEMORY_ERROR, SIZE_ERROR, 
                CHECK_IF_EMPTY_ERROR, FILLING_ERROR, I_AM_OK };
                                             
typedef unsigned long long can_type;
#define CANARY_SIZE sizeof(can_type)

// Array Canaries
#define CANARY_1L ((can_type*)((char*)stk->array - CANARY_SIZE))
#define CANARY_1R ((can_type*)((char*)stk->array + stk->capacity*sizeof(double)))

// Hole Stack Canaries
#define CANARY_2L ((can_type*)((char*)stk - CANARY_SIZE))
#define CANARY_2R ((can_type*)((char*)stk + sizeof(MyStack)))

#define MIN_CAPACITY 1

struct MyStack;

//Logs
void LogsClear ();
int  StackOK   (MyStack* stk);
void ASSERT_OK (MyStack* stk, char* func);
void StackDump (MyStack* stk, char* func);

long long int HashCounter (const void* begin, const void* end);
void HashMaker   (MyStack* stk);
int  HashCompare (MyStack* stk);

//Unit_Tests
void Unit_Tests (int argc, const char* argv[]);
void Help_Print ();
void Test_StackPush ();
void Test_StackPop  ();
void Test_StackTop  ();
void Test_StackExpansion ();
void Test_StackFree ();

//Stack
MyStack* StackConstruct (size_t capacity);
MyStack* StackDestruct  (MyStack* stk);

void StackExpansion (MyStack* stk);
void StackFree      (MyStack* stk);

void   StackClear (MyStack* stk);
void   StackPush  (MyStack* stk, double elem);
double StackPop   (MyStack* stk);
double StackTop   (MyStack* stk);

struct MyStack
{
    long long int hash;
    bool is_empty;
    size_t size;
    size_t capacity;
    double* array;
};