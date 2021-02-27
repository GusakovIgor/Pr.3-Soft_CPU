#include "MyStack.h"

void Unit_Tests (int argc, const char* argv[])
{
    assert(argv != 0);
    assert(argc  > 1);
    
    if (strcmp(*(argv + 1), "--All") == 0)
    {
        printf("\nAll Unit_tests starting\n");
        Test_StackPush ();
        Test_StackPop  ();
        Test_StackTop  ();
        Test_StackExpansion ();
        Test_StackFree ();
    }
    else if (strcmp(*(argv + 1), "--None") == 0)
    {
        printf ("\nBad student! Use Unit_tests next time!!!\n");
        printf ("\nLook at them. Each one is your bro:\n");
        Help_Print();
    }
    else if (strcmp(*(argv + 1), "--Help") == 0)
    {
        Help_Print();
    }
    else if (strpbrk((*(argv + 1)), "12345") != 0)
    {
        for (int i = 1; i <= argc; i++)
        {
            switch (atoi(*(argv + i)) )
            {
                case (1):   Test_StackPush ();
                            break;
                            
                case (2):   Test_StackPop ();
                            break;
                            
                case (3):   Test_StackTop ();
                            break;
                            
                case (4):   Test_StackExpansion ();
                            break;
                            
                case (5):   Test_StackFree ();
                            break;
            }
        }
    }
    else
    {
      printf ("Please, enter one of those commands:");
        Help_Print();
      printf ("Or turn SUPERDEBUG mode off");
    }
}


void Help_Print()
{
    printf ("\"--Help\"  Help Print with commands \n");
    printf ("\"--All\"   All Tests                \n");
    printf ("\"--None\"  Without Unit Tests       \n");
    printf ("\n");
    printf ("\"1\"  Test_StackPush                \n");
    printf ("\"2\"  Test_StackPop                 \n");
    printf ("\"3\"  Test_StackTop                 \n");
    printf ("\"4\"  Test_StackExpansion           \n");
    printf ("\"5\"  Test_StackFree                \n");
}


//-------------------------------------------------------------------------------------------------------
//! Test_StackPush function:
//!     1) This function makes tests of StackPush
//!     2) It has a error variable. It's 0 in the end of test if there was error, and 0 if everything was alright
//!     3) It has an array with elements for tests (exp_numbers) and size of that array (num_elements)
//!
//!     4) It creates test_stk and makes tests (num_tests responsible for how many tests will be)
//!     5) For each test it randomises num_test_el (value that responsible for how many elements we will push and check)
//!     6) Then we pushes elements in array in first cycle and compare their values with elements in exp_numbers in second cycle
//!
//!     7) If there was error printing arrays (exp_numbers and test_stk->array) in way it's easy to compare them
//!     8) If there was no error printing that everything is ok
//!
//!     9) Clears everything
//!
//! @return void
//-------------------------------------------------------------------------------------------------------
void Test_StackPush ()
{
    printf("\nTest_StackPush:\n");
    
    int error = 0;
    const size_t num_elements = 10;
    double exp_number[num_elements] = {-10, 7, 21000, -34.5, 22, 5051, 4004.007, -1, 0, 777};
    
    MyStack* test_stk = StackConstruct (num_elements);
    ASSERT_OK(test_stk, "Test_StackPush");
    
    const size_t num_tests = 7;
    
    size_t num_test_el = 0;
    for (int t = 0; t < num_tests; t++)
    {
        srand(t);
        num_test_el = rand() % num_elements + 1;
        assert (num_test_el > 0);
        assert (num_test_el <= num_elements);
        
        for (int i = 0; i < num_test_el; i++)
        {
            StackPush(test_stk, exp_number[i]);
        }
        ASSERT_OK(test_stk, "Test_StackPush");
        
        for (int i = 0; i < num_test_el; i++)
        {
            if (test_stk->array[i] != exp_number[i])
            {
                error = 1;
            }
        }
        
        if (error == 1)
        {
            printf("#Test %d is Bad\n", t + 1);
            printf(" Expected:\tRecived:\n");
            for (int i = 0; i < num_test_el; i++)
            {
                printf ("%4lg%10lg\n", exp_number[i], (test_stk->array)[i]);
            }
        }
        else
        {
            printf("#Test %d is Good\n", t + 1);
        }
        
        error = 0;
        StackClear(test_stk);
        ASSERT_OK(test_stk, "Test_StackPush");
    }
    ASSERT_OK(test_stk, "Test_StackPush");
    test_stk = StackDestruct(test_stk);
}


//-------------------------------------------------------------------------------------------------------
//! Test_StackPop function:
//!     1) This function makes tests of StackPop
//!     2) It has a error variable. It's 0 in the end of test if there was error, and 0 if everything was alright
//!     3) It has an array with elements for tests (exp_numbers) and size of that array (num_elements)
//!     4) And it also has an array for elements that it will recieve from StackPop (rec_number) with the same size
//!     
//!     5) It creates test_stk and makes tests (num_tests responsible for how many tests will be)
//!           a) Every test ends with StackClear(test_stk)
//!           b) So every test must begins with pushing exp_number in test_stk->array
//!     6) For each test it randomises num_test_el (value that responsible for how many elements we will push, then pop and check)
//!     
//!     7) Then it uses StackPop and gets elements from test_stk->array (from the end because of LIFO concept)
//!     8) It stores them in rec_number
//!     9) And in each iteration of test, compares current rec_number and corresponding element in exp_number
//!
//!    10) If there was error printing arrays (exp_numbers and rec_number) in way it's easy to compare them
//!    11) If there was no error printing that everything is ok
//!
//!    12) Clears everything
//!
//! @return void
//-------------------------------------------------------------------------------------------------------
void Test_StackPop ()
{
    printf("\nTest_StackPop:\n");
    
    int error = 0;
    const size_t num_elements = 10;
    
    MyStack* test_stk = StackConstruct (num_elements);
    ASSERT_OK(test_stk, "Test_StackPop");
    
    double exp_number[num_elements] = {-31, 96, -15304, 34.75, -20, 4351111, -5034.107, 1, 9, 0};
    double rec_number[num_elements] = {  0,  0,      0,     0,   0,       0,         0, 0, 0, 0};
    
    const size_t num_tests = 7;
    
    size_t num_test_el = 0;
    for (int t = 0; t < num_tests; t++)
    {
        for (int i = 0; i < num_elements; i++)
        {
            StackPush (test_stk, exp_number[i]);
        }
        ASSERT_OK(test_stk, "Test_StackPop");
        
        srand(t);
        num_test_el = rand() % num_elements + 1;
        assert (num_test_el > 0);
        assert (num_test_el <= num_elements);
        
        for (int i = num_elements - 1; i >= num_elements - num_test_el; i--)
        {
            rec_number[i] = StackPop(test_stk);
            if (rec_number[i] != exp_number[i])
            {
                error = 1;
            }
        }
        
        if (error == 1)
        {
            printf("#Test %d is Bad\n", t + 1);
            printf(" Expected:\t\tRecived:\n");
            for (int i = 0; i < num_test_el; i++)
            {
                printf ("%4lg%30lg\n", exp_number[i], rec_number[i]);
            }
        }
        else
        {
            printf("#Test %d is Good\n", t + 1);
        }
        
        
        for (int i = 0; i < num_test_el; i++)
        {
            rec_number[i] = 0;
        }
        StackClear(test_stk);
        error = 0;
        
        ASSERT_OK(test_stk, "Test_StackPop");
    }
    test_stk = StackDestruct(test_stk);
}


//-------------------------------------------------------------------------------------------------------
//! Test_StackTop function:
//!     1) This function makes tests of StackTop
//!     2) It has an array with elements for tests (exp_numbers) and size of that array (num_elements)
//!     3) And it also has an array for elements that it will recieve from StackPop (rec_number) with the same size
//!
//!     3) It creates test_stk and makes tests (num_tests responsible for how many tests will be)
//!           a) Every test ends with StackClear(test_stk)
//!           b) So every test must begins with pushing exp_number in test_stk->array
//!     4) For each test it randomises num_test_el (value that responsible for how many elements we will push, then pop and check)
//!
//!     5) Then it takes top element and compares it with the element we need
//!     6) If something go wrong prints expected top and recieved and tells that something wrong
//!     7) If everything is ok, prints that conclusion
//!
//!     8) Clears everything
//!
//! @return void
//-------------------------------------------------------------------------------------------------------
void Test_StackTop ()
{
    printf("\nTest_StackTop:\n");
    
    const size_t num_elements = 10;
    
    MyStack* test_stk = StackConstruct (num_elements);
    ASSERT_OK(test_stk, "Test_StackTop");
    
    double exp_number[num_elements] = {-31, 96, -15304, 34.75, -20, 4351111, -5034.107, 1, 9, 0};
    
    const size_t num_tests = 7;
    
    double top = 0;
    size_t num_test_el = 0;
    for (int t = 0; t < num_tests; t++)
    {
        srand(t);
        num_test_el = rand() % num_elements + 1;
        assert (num_test_el > 0);
        assert (num_test_el <= num_elements);
        
        for (int i = 0; i < num_test_el; i++)
        {
            StackPush (test_stk, exp_number[i]);
        }
        ASSERT_OK(test_stk, "Test_StackTop");
        
        top = StackTop(test_stk);
        assert (top = test_stk->array[test_stk->size]);
        
        if (top != exp_number[num_test_el-1])
        {
            printf ("#Test %d is Bad\n", t + 1);
            printf (" Expected top = %d but recieved %d\n", num_test_el-1, top);
        }
        else
        {
            printf ("#Test %d is Good\n", t + 1);
        }
        
        StackClear(test_stk);
        ASSERT_OK(test_stk, "Test_StackTop");
    }
    test_stk = StackDestruct(test_stk);
}


//-------------------------------------------------------------------------------------------------------
//! Test_StackExpansion function:
//!     1) This function makes tests of StackExpansion
//!     2) It works the same as Test_StackPop function, except the fact, that it pushes not all elements, only 2 * num_test_el
//!        and that can be more, than num_elements
//!     3) Clears everything
//!
//! @return void
//-------------------------------------------------------------------------------------------------------
void Test_StackExpansion ()
{
    printf("\nTest_StackExpansion:\n");
    
    int error = 0;
    const size_t num_elements = 10;
    
    MyStack* test_stk = StackConstruct (num_elements);
    ASSERT_OK(test_stk, "Test_StackExpansion");
    
    double exp_number [2*num_elements] = {-31, 96, -15304, 34.75, -20, 4351111, -5034.107,  1, 9, 0, -10,  7,  21000, -34.5,  22,    5051,  4004.007, -1, 0, 777};
    double rec_number [2*num_elements] = {  0,  0,      0,     0,   0,       0,         0,  0, 0, 0,    0,  0,      0,     0,   0,       0,        0,  0, 0, 0};
    
    const size_t num_tests = 7;
    
    size_t num_test_el = 0;
    for (int t = 0; t < num_tests; t++)
    {
        srand(t);
        num_test_el = rand() % num_elements + num_elements + 1;
        assert (num_test_el > 0);
        
        for (int i = 0; i < num_test_el; i++)
        {
            StackPush(test_stk, exp_number[i]);
        }
        ASSERT_OK(test_stk, "Test_Expansion");
        
        for (int i = num_test_el-1; i >= 0; i--)
        {
            rec_number[i] = StackPop(test_stk);
            if (rec_number[i] != exp_number[i])
            {
              error = 1;
            }
        }
        
        
        if (error == 1)
        {
            printf ("#Test %d is Bad\n", t + 1);
            printf (" Expected:    Recieved:\n");
            printf ("stk->capacity = %d\n", test_stk->capacity);
            for (int i = 0; i < num_test_el; i++)
            {
                printf("%lg %lg\n", exp_number[i], rec_number[i]);
            }
        }
        else
        {
            printf ("#Test %d is Good\n", t + 1);
        }
        
        
        for (int i = 0; i < 2*num_elements; i++)
        {
            rec_number[i] = 0;
        }
        
        error = 0;
    }
    test_stk = StackDestruct(test_stk);
}


//-------------------------------------------------------------------------------------------------------
//! Test_StackFree function:
//!     1) This function makes tests of StackFree
//!     2) It creates stack and array the same size (num_elements) with elements for tests
//!     3) For each test it fills stack, using push 
//!         (capacity automaticly became big enough for num_elements, cause StackExpansion works well)
//!     4) And randomizing nuber of elments, that it will pop (num_test_el > half of stack)
//!     
//!     5) Then depending on num_elements and num_test_el calculating result (capacity that stack should have)
//!     6) And compares result with test_stk->capacity
//!     7) If they are not equal printing them and message, that test is not ok
//!     8) If they are equal printing message, that test is ok
//!
//!     9) Clears Everything
//!
//! @return void
//-------------------------------------------------------------------------------------------------------
void Test_StackFree ()
{
    printf("\nTest_StackFree:\n");
    
    const size_t num_elements = 20;
    MyStack* test_stk = StackConstruct (num_elements);
    ASSERT_OK(test_stk, "Test_StackFree");
    
    double exp_number [num_elements] = {-31, 96, -15304, 34.75, -20, 4351111, -5034.107,  1, 9, 0, -10,  7,  21000, -34.5,  22,    5051,  4004.007, -1, 0, 777};
    
    size_t num_test_el = 0;
    double temp = 0;
    int result = num_elements;
    
    const size_t num_tests = 7;
    for (int t = 0; t < num_tests; t++)
    {
        for (int i = 0; i < num_elements; i++)
        {
            StackPush (test_stk, exp_number[i]);
        }
        ASSERT_OK(test_stk, "Test_StackFree");
        
        result = test_stk->capacity;
        temp = 0;
        
        srand(t);
        num_test_el = rand() % (num_elements/2) + (num_elements/2) + 1;
        assert (num_test_el > 0);
        assert (num_test_el <= num_elements);
        
        for (int i = 0; i < num_test_el; i++)
        {
          temp = StackPop(test_stk);
        }
        ASSERT_OK(test_stk, "Test_StackFree");
        
        while ((num_elements - num_test_el - 1 <= (result / 4)) && result >= 4)        // num_elements - num_test_el is bigger then size on 1
        {                                                                              // so we should decrease difference on 1 (in StackPop comparation going by size)
          result = result / 2;
        }
        
        if (result != test_stk->capacity)
        {
          printf ("#Test %d is Bad\n", t + 1);
          printf (" Expected result capacity = %u  Recieved - %u", result, test_stk->capacity);
        }
        else
        {
          printf ("#Test %d is Good\n", t + 1);
        }
        
        StackClear(test_stk);
    }
    test_stk = StackDestruct(test_stk);
}
