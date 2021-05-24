#include "Stack.h"


//-------------------------------------------------------------------------------------------------------
//! StackConstruct function:
//!     1) Creating structure MyStack stk for our stack (in dinamic memory)
//!     2) Filling its fields
//!     3) Creating array (in dinamic memory) and shifting pointer forward on first element (everything before is canary)
//!         a) Array size is bigger than we need on two BYTES for canary protection
//!         b) Filling cannaries we need to make conversions
//!             b_1) double* to char*, otherwise memory distributor will index block of 8 bytes each
//!             b_2) after changing pointer convert it back to double*
//!     4) Fiiling canaries
//!     5) Clearing stack
//!
//! @param [in] capacity - initial capacity of array
//!
//! @return stk - pointer on MyStack structure
//-------------------------------------------------------------------------------------------------------
MyStack* StackConstruct (size_t capacity)
{
    MyStack* stk = (MyStack*) calloc (1*sizeof(MyStack) + 2*CANARY_SIZE, sizeof(char*));
    stk = (MyStack*)((char*)stk + CANARY_SIZE);
    assert (stk != NULL);
    
    *CANARY_2L = 0xBEEF;                  // Making second left canary BEEF
    *CANARY_2R = 0xBEEF;                  // And right too
    
    *stk = {0, true, 0, capacity, NULL};
    
    stk->array = (double*) calloc (capacity*sizeof(double) + 2*CANARY_SIZE, sizeof(char));     // Two extra bytes for canaries
    assert (stk->array != NULL);
    stk->array = (double*)((char*)stk->array + CANARY_SIZE);                                   // Shifting pointer on 1 BYTE right (to the array)
    
    *CANARY_1L = 0xC0FFEE;                  // Making left canary C0FFEE
    *CANARY_1R = 0xC0FFEE;                  // And right too

    StackClear (stk);
    
    HashMaker (stk);
    ASSERT_OK (stk, "StackConstruct");
    
    return stk;
}


//-------------------------------------------------------------------------------------------------------
//! StackDestruct function:
//!     1) Deleting arrray using pointer on Canary_1L
//!     2) Deleting stk
//!
//! @param [out] stk - stack
//!
//! @return NULL - means that there's no stack
//-------------------------------------------------------------------------------------------------------
MyStack* StackDestruct (MyStack* stk)
{
    ASSERT_OK (stk, "StackDestruct");
    
    free (CANARY_1L);
    free (CANARY_2L);

    return NULL;
}


//-------------------------------------------------------------------------------------------------------
//! StackClear function:
//!     1) Making all elements in stack NAN
//!     2) Making stk->size zero (cause we've deleted all elements in stack)
//!     3) And ofcourse making stk->is_empty true
//!
//! @param [out] stk - stack
//!
//! @return void
//-------------------------------------------------------------------------------------------------------
void StackClear (MyStack* stk)
{
    if (!stk->is_empty)
    {
        ASSERT_OK (stk, "StackClear");
    }
    
    for (int i = 0; i < stk->capacity; i++)
    {
        stk->array[i] = NAN;
    }
    
    stk->size = 0;
    stk->is_empty = true;
    
    HashMaker (stk);
    ASSERT_OK (stk, "StackClear");
}


//-------------------------------------------------------------------------------------------------------
//! StackPush function:
//!     1) Checks that stk is ok
//!     2) If element isn't deepest in stack and not highest, we simply increase size
//!        (and puts in stk->array[stk->size] new element, but it's in paragraph !!!)
//!     3) If element is deepest 
//!     4) And increases stk->size
//!
//! @param [out] stk  - stack
//! @param [in]  elem - value that we need to put in stack
//!
//! @return void
//-------------------------------------------------------------------------------------------------------
void StackPush (MyStack* stk, double elem)
{    
    (stk->size == 0 && isnan(stk->array[0])) ? stk->is_empty = true : stk->is_empty = false;
    
    HashMaker (stk);
    ASSERT_OK(stk, "StackPush");
    
    if (stk->is_empty)
    {
        stk->array[stk->size] = elem;
        stk->is_empty = false;
    }
    else
    {
        if (stk->size == stk->capacity - 1)
        {
            HashMaker (stk);
            StackExpansion(stk);
        }
        stk->size++;
        stk->array[stk->size] = elem;
    }
    
    HashMaker (stk);
    ASSERT_OK (stk, "StackPush");
}


//-------------------------------------------------------------------------------------------------------
//! StackPop function:
//!     1) Checks that stk is ok
//!     3) Saving value of the top element in temporary variable - top
//!     3) Then makes place of this element (in stk) NAN
//!     4) Decreses size only if it's > 0
//!     5) Then if size <= 0.25 * capacity and capacity >= 4:
//!        a) Capacity must be >= 4 cause in other cases new capacity will be 1 or 0,
//!           and that will cause conflicts with size
//!     6) Checks again, that stk is ok
//!
//! @param [out] stk  - stack
//!
//! @return top - value of the first element in stack
//-------------------------------------------------------------------------------------------------------
double StackPop (MyStack* stk)
{
    ASSERT_OK(stk, "StackPop");
    
    double top = 0;
    
    if (!stk->is_empty)
    {
        top = stk->array[stk->size];
        stk->array[stk->size] = NAN;
        assert (isnan(stk->array[stk->size]));
      
        if (stk->size == 0)     // ASSERT_OK helped to debug that
        {                       // there was no checking if we pop the last element in stack
            stk->is_empty = true;
        }
    }
    
    if (stk->size > 0) // ASSERT_OK helped to debug that
    {                  // there was no if, and stk->size sometimes was -1
        stk->size--;
    }
    
    if (stk->size <= stk->capacity / 4 && stk->capacity >= 4)
    {
        HashMaker (stk);
        StackFree (stk);
    }
    
    HashMaker (stk);
    ASSERT_OK(stk, "StackPop");
    
    return top;
}

//-------------------------------------------------------------------------------------------------------
//! StackTop function:
//!     1) Checks that stk is ok
//!     2) And returns value of top element
//!
//! @param [out] stk  - stack
//!
//! @return top - value of the first element in stack
//-------------------------------------------------------------------------------------------------------
double StackTop (MyStack* stk)
{
    ASSERT_OK (stk, "StackTop");
    
    return stk->array[stk->size];
}


//-------------------------------------------------------------------------------------------------------
//! StackExpansion function:
//!     1) It checks that stk is ok and that we correctly called this function
//!     2) Doubles memory for the stk->array using realloc
//!     3) It checks again that stk->array isn't NULL (that realloc worked well)
//!     4) Reorganizes memory to use Canaries (just like in StrackConstruct)
//!     5) Then making all new elements NANs
//!     6) And then doubles capacity of stk->array
//!     7) Filling Canaries (only after changing capacity!)
//!     8) Checks again if stk is ok
//!
//! @param [out] stk  - stack
//!
//! @return void
//-------------------------------------------------------------------------------------------------------
void StackExpansion (MyStack* stk)
{
    ASSERT_OK(stk, "StackExpansion");
    assert (stk->size == stk->capacity - 1);
    
    stk->array = (double*) realloc (CANARY_1L, 2*(stk->capacity)*sizeof(double) + 2*CANARY_SIZE);
    assert (stk->array != NULL);
    stk->array = (double*)((char*)stk->array + CANARY_SIZE);
    
    size_t new_size = 2*stk->capacity;
    for (int i = stk->capacity; i < new_size; i++)
    {
        stk->array[i] = NAN;
    }
    
    stk->capacity = new_size;
    
    // HashMaker (stk)
    // It seems to be a hack method
    // If we counting hash before restoring canaries ASSERT won't be OK

    *CANARY_1L = 0xC0FFEE;
    *CANARY_1R = 0xC0FFEE;
    
    HashMaker (stk);
    ASSERT_OK(stk, "StackExpansion");
}


//-------------------------------------------------------------------------------------------------------
//! StackFree function:
//!     1) It checks that stk is ok and that we correctly called this function
//!     2) Halves memory for the stk->array using realloc
//!     3) Reorganizes memory to use Canaries (just like in StrackConstruct)
//!     4) And then halves stk->capacity
//!     5) Filling Canaries (only after changing capacity!)
//!     6) Checking again, that stk is ok
//!
//! @param [out] stk  - stack
//!
//! @return void
//-------------------------------------------------------------------------------------------------------
void StackFree (MyStack* stk)
{
      ASSERT_OK(stk, "StackFree");
      assert (stk->size <= stk->capacity/4 && stk->capacity >= 4);
      
      size_t check = stk->capacity;
      
      stk->array = (double*) realloc (CANARY_1L, (stk->capacity)*sizeof(double) / 2 + 2*CANARY_SIZE);
      assert (stk->array != NULL);
      stk->array = (double*)((char*)stk->array + CANARY_SIZE);
      
      stk->capacity = stk->capacity / 2;
      assert (check / 2 == stk->capacity);
      
      HashMaker (stk);
      
      *CANARY_1L = 0xC0FFEE;
      *CANARY_1R = 0xC0FFEE;
      
      ASSERT_OK(stk, "StackFree");
}
