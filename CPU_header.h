#include "TXLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys\stat.h>
#include <memory.h>
#include <ctype.h>
#include "Stack/MyStack.h"

const int MAX_FILENAME     = 30;      // For program->name
const int MAX_LABLE_NAME   = 15;
const int MAX_CODE_LEN     = 100000;  // For program->bin_buff
const int MAX_COMAND_LEN   = 10;       
const int MAX_NUM_LABLES   = 500;
const int STACK_START_SIZE = 100;     // For arrays with names of comands
const int NUM_REGISTERS    = 6;       // For arrays with names of registers
const int VERSION          = 6;

const int RAM_SIZE         = 100;
const int VRAM_SIZE        = 1000000;
const int pix_size         = 1;

const int MODE_NUMBER = 0;
const int MODE_WORD   = 1;

// DEFINING COMMANDS
#define DEF_CMD(name, num, arg, code)   \
                CMD_##name = num,       \

enum comands
{
    #include "Commands.h"
};

#undef DEF_CMD

// END OF DEFINING COMMANDS

enum registers {    VRAM = 0,
                    RAX = 1, 
                    RBX = 2, 
                    RCX = 3, 
                    RDX = 4,
                    CAT = 5,
                    MYAU = 6     };
            

struct text;
struct CPU;
struct FileHeader;
struct lable;


// Text_processing file
text* ProgramConstructor (char* name);

size_t TextGetter   (text* program);
size_t SizeGetter   (const char* name);
size_t TextReader   (text* program, size_t size);

size_t WordsCounter (char* text, size_t num_symbals);
int CommentCleaner  (char* text, size_t num_symbals, int cur_symbal);
char*  NameProcessing (char* name);
//---------------------------------------------------


// Assembler file
lable* CreateLabels ();
void Assembler      (text* program, char* bin_buff, lable* lables, int ofs);
void Sign_maker     (char* bin_buff, int* ofs);
void MakeLable      (lable* lables, char* temp, char* check, int* ofs, int count);
int  SearchLable    (lable* lables, char* temp);
text* ProgramDestructor (text* program);

void ComplicComProcessing   (char* buff, char* bin_buff, int* ofs, int* pos, char* temp, int* count, lable* lables, int num);                 
void PushPopProcessing      (char* bin_buff, int* ofs, char* temp_1, int count);                // Thanks Uliana for that functions
void JmpProcessing          (char* bin_buff, int* ofs, char* temp, int count, lable* lables);       // (I was trying to make it all in DEF_CMD macro)
void DrawProcessing         (char* buff, char* bin_buff, int* ofs, int*pos, char* temp, int* count);
char ModeProcessing (char* temp_1, char* temp_2, int count);
void ArgInsert      (char* bin_buff, int* ofs, char* temp, int count);
int  FindRegNumber  (char* temp, int count);
//--------------------------------------------------

// Calculator file
CPU* CPU_Construct  (char* file_name);
CPU* CPU_Destruct    (CPU* processor);
void CodeReader      (CPU* processor, char* file_name);
void SignatureCheck  (CPU* processor);
void StackDebugPrint (CPU* processor);
double ArgGetter     (CPU* processor, int mode, double* RAM);
double* RAM_Maker    ();
//--------------------------------------------------

struct text
{
    size_t num_words;
    char* name;
    char* buff;
};


struct CPU
{
    MyStack* stack;
    MyStack* calls;
    
    int pc;
    
    double* registers;
    
    char* code;
    size_t code_size;
};


struct FileHeader
{
    const char signature[3];
    int version;
};


struct lable
{
    int adr;
    char name[MAX_LABLE_NAME];
};
