#include "MyStack.h"

//-------------------------------------------------------------------------------------------------------
//! LogsClear function:
//!     1) It opens file "Logs.txt" in mode "w" (writing), so file automatically clears
//!     2) Then it closes "Logs.txt"
//!     3) And we can enjoy that we have such good, clean logs file
//!
//! @return void
//-------------------------------------------------------------------------------------------------------
void LogsClear ()
{
  FILE* Logs = fopen ("Logs.txt", "w");
  fclose (Logs);
}


//-------------------------------------------------------------------------------------------------------
//! StackOK function:
//!     1) Checks stk on following errors:
//!        a) CANARY_ARRAY_PROTECTION_ERROR
//!        b) CONSTRUCTION_ERROR 
//!        c) MEMORY_ERROR
//!        d) SIZE_ERROR
//!        e) CHECK_IF_EMPTY_ERROR
//!        f) FILLING_ERROR
//!        g) ALL_IS_OK
//!
//!
//! @param [out] stk - stack
//!
//! @return integer - code of the error
//-------------------------------------------------------------------------------------------------------
int StackOK (MyStack* stk)
{
    if (stk == NULL)
    {
        return CONSTRUCTION_ERROR;
    }
  
    if (stk->array == NULL)
    {
        return MEMORY_ERROR;
    }
    
    /*if(HashCompare(stk) == HASH_PROTECTION_ERROR)
    {
        return HASH_PROTECTION_ERROR;
    }*/
    
    if (*CANARY_2L != 0XBEEF || *CANARY_2R != 0XBEEF)
    {
        return CANARY_STACK_PROTECTION_ERROR;
    }
  
    if (*CANARY_1L != 0XC0FFEE || *CANARY_1R != 0XC0FFEE)
    {
        return CANARY_ARRAY_PROTECTION_ERROR;
    }
    
    if (stk->capacity < stk->size)
    {
        return SIZE_ERROR;
    }

    if (stk->size == 0 && isnan(stk->array[0]) && stk->is_empty == false)
    {
        return CHECK_IF_EMPTY_ERROR;
    }
    
    if ( (int)stk->size < 0)
    {
        return FILLING_ERROR;
    }
  
    for (int i = 0; i < stk->capacity; i++)
    {  
        if ((i <= stk->size && !stk->is_empty && isnan(stk->array[i])) ||
            (i > stk->size  && !isnan(stk->array[i])))
        {
            return FILLING_ERROR;
        }
    }
    
    return I_AM_OK;
}


void StackDump(MyStack* stk, char* func)
{
    //printf("OK\n");
    FILE* Logs = fopen("Logs.txt", "a"); 
    printf("OK\n");
    int er_code = StackOK (stk); 
    printf("OK\n");
    printf("er_code = %d\n", er_code);                                                                                     
    if (er_code != 1)                                                                                                 
    {                                                                                                                                                                                 
        fprintf(Logs, "ERROR in function %s with code: %d\n", func, er_code);                                                                
        switch (er_code)                                                                                                
        {
            case (-7):  fprintf(Logs, "HASH_PROTECTION_ERROR: Stack or array in stack is changing not in stack functions\n"); 
                        break;
            case (-6):  fprintf(Logs, "CANARY_STACK_PROTECTION_ERROR: Elements outside stack structure are changing\n"); 
                        break;                                                                                            
            case (-5):  fprintf(Logs, "CANARY_ARRAY_PROTECTION_ERROR: Elements outside array are changing\n");            
                        break;                                                                                            
            case (-4):  fprintf(Logs, "CONSTRUCTION_ERROR: Pointer on stk is NULL\n");                                    
                        break;                                                                                            
            case (-3):  fprintf(Logs, "MEMORY_ERROR: Pointer on array is NULL\n");                                        
                        break;                                                                                            
            case (-2):  fprintf(Logs, "SIZE_ERROR: Capacity < size, or size < 0. Probably expansion works incorrect\n");  
                        break;                                                                                            
            case (-1):  fprintf(Logs, "CHECK_IF_EMPTY_ERROR: Probably you put assert before checking if stack empty or something wrong with element stk->array[0]\n");
                        break;                                                                                            
            case ( 0):  fprintf(Logs, "FILLING_ERROR: Some value in stk is NAN when it shouldn't be, or it's not NAN when should be\n");
                        break;                                                                                            
        }                                                                                                               
    }
    fprintf(Logs, "\nCanary_2left = %X\n", *CANARY_2L); 
    fprintf(Logs, "\nStack ");                                                                               
    if (er_code != 1)                                                                                                 
    {                                                                                                                 
        fprintf(Logs, "(NOT OK) ");                                                                                     
    }                                                                                                                 
    else                                                                                                              
    {                                                                                                                 
        fprintf(Logs, "(OK) ");                                                                                         
    }                                                                                                                 
    fprintf(Logs, "[%p]\n", stk);
    fprintf(Logs, "Hash = %lld\n", stk->hash);
    fprintf(Logs, "size = %d\n",   (stk)->size);                                                                    
    fprintf(Logs, "capacity = %d\n", (stk)->capacity);                                                                
    fprintf(Logs, "\nCanary_1L = %X\n", *((can_type*)((char*)stk->array - CANARY_SIZE)) );                            
    fprintf(Logs, "array [%p]\n", stk->array);                                                                        
    fprintf(Logs, "      {\n");
    
    for (int i = 0; i < (stk)->capacity; i++)
    {                                                                                                                 
        fprintf(Logs, "    ");                                                                                        
        if (isnan((stk)->array[i]))                                                                                      
            fprintf(Logs, "* ");                                                                                                            
        else                                                                                                         
            fprintf(Logs, "  "); 
                                                                                                          
        fprintf(Logs, "[%d] = %lg\n", i, (stk)->array[i]);                                                            
    }                                                                                                                 
        
    fprintf(Logs, "      }\n");                                                                                       
    fprintf(Logs, "Canary_1R = %X\n", *((can_type*)((char*)stk->array + stk->capacity*sizeof(double))) );             
    fprintf(Logs, "\nCanary_2R = %X", *CANARY_2R);                                                                    
    fprintf(Logs, "\n\n\n\n\n");   
    printf("\nLAST_OK\n\n");                                                                                   
    fclose (Logs);
}


void ASSERT_OK(MyStack* stk, char* func)
{
    if (StackOK(stk) < 1)  
    {                         
        printf ("\nYou can see, what's wrong in file \"Logs.txt\"\n\n");
        printf("\"%s\"\n", func);
        StackDump(stk, func);    
        assert(!"OK");          
    }
}


long long int HashCounter (const void* begin, const void* end)
{
    long long int value = 0;
    int count = 0;
    
    for (char* p = (char*)begin; p < (char*)end; p++)
    {
        //printf ("%d - OK\n", count++);
        int temp1 = *p & 126;
        int temp2 = *p >> 1;
        
        value = (value << 1) + temp1^temp2;
    }
    
    return value;
}


void HashMaker (MyStack* stk)
{
    /*
    stk->hash = 0;
    printf ("capacity = %d\n", stk->capacity);
    printf ("sizeof(MyStack) = %d\n", sizeof(MyStack));
    printf ("%d", *(stk->array + 1129));
    printf ("%d", *(stk + 1129));
    stk->hash = HashCounter(stk->array, stk->array + stk->capacity) + HashCounter(stk, stk + sizeof(MyStack));
    */
}


int HashCompare (MyStack* stk)
{
    long long int rem_hash = stk->hash;
    
    stk->hash = 0;
    long long int test_value = HashCounter(stk->array, stk->array + stk->capacity) + HashCounter(stk, stk + sizeof(MyStack));
    stk->hash = rem_hash;
    
    //printf("test_value = %lld\n", test_value);
    //printf("stk->hash = %lld\n", stk->hash);
    if (test_value != stk->hash)
    {
        return HASH_PROTECTION_ERROR;
    }
}
