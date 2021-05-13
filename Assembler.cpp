#include "CPU_header.h"

int main (int argc, const char* argv[])
{   
    assert (argc > 1);
    text* program = ProgramConstructor ((char*) argv[1]);
    assert (program);
    
    char* bin_buff = (char*) calloc (MAX_CODE_LEN, sizeof(char));
    assert (bin_buff);
    
    lable*  lables = CreateLabels ();
    
    for (int i = 0; i < 2; i++)
    {
        int ofs = 0;
        Sign_maker (bin_buff, &ofs);
        
        bin_buff[ofs] = CMD_START;
        ofs += sizeof(char) + sizeof(int);
        
        Assembler  (program, bin_buff, lables, ofs);
    }
    
    free (lables);
    free (bin_buff);
    
    program = ProgramDestructor (program);
    
    return 0;
}


lable* CreateLabels ()
{
    lable*  lables = (lable*)  calloc (MAX_NUM_LABLES + 1, sizeof(lable));      // Plus 1 because user probably want to use labels 1-50
    assert (lables);                                                            // if MAX_NUM_LABELS = 50, so we've got labels 0-50
    
    for (int i = 0; i < MAX_NUM_LABLES + 1; i++)
    {
        lables[i].adr = -1;
    }
    
    return lables;
}


void Assembler (text* program, char* bin_buff, lable* lables, int ofs)
{
    static size_t iter = 0;
    
    char* temp     = (char*) calloc (MAX_COMAND_LEN, sizeof(char)); assert (temp);
    char* check    = (char*) calloc (MAX_COMAND_LEN, sizeof(char)); assert (check);
    
    int pos = 0;
    int com_len = 0;
    int reg_number = 0;
    
    #define DEF_CMD(name, num, arg, code)                                           \
        else if (strcmpi(temp, #name) == 0)                                         \
        {                                                                           \
            if (CMD_##name == CMD_START)                                            \
            {                                                                       \
                *(int*)(bin_buff + sizeof(FileHeader) + sizeof(char)) = ofs;        \
            }                                                                       \
            else                                                                    \
            {                                                                       \
                bin_buff[ofs] = CMD_##name;                                         \
                ofs += sizeof(char);                                                \
                                                                                    \
                if (arg > 0)                                                        \
                {                                                                   \
                    count++;                                                        \
                    sscanf (program->buff + pos, "%s%n", temp, &com_len);           \
                    pos += com_len;                                                 \
                                                                                    \
                    ComplicComProcessing (program->buff, bin_buff, &ofs, &pos, temp, &count, lables, num);\
                }                                                                   \
                                                                                    \
            }                                                                       \
        }                                                                           \
    
    for (int count = 0; count < program->num_words; count++)
    {
        sscanf (program->buff + pos, "%s%n", temp, &com_len);   // Sscanf reading string with all separators and add them to the com_len
        pos += com_len;                                         // So if i scanf string "pop     rax" at fist iteration com_len = 3
                                                                // And at the second it's 8, so i can use it
        check = strchr(temp, ':');
        if (check)
        {
            if (iter > 0)
                continue;
            MakeLable (lables, temp, check, &ofs, count + 1);    // count + 1 cause we counts from 0 and in debug user need from 1
        }
        #include "Commands.h"
        else
        {
            printf ("Syntax Error!\n");
            printf ("In word: %s\n", temp);
            assert (!"OK");
        }

    }
    #undef DEF_CMD
    
    free (temp);
    free (check);
    
    FILE* code = fopen(program->name, "wb+");   // Why not creating file when mode is "wb"?...
    fwrite (bin_buff, ofs, 1, code);
    fclose (code);
    
    iter++;
}


void Sign_maker (char* bin_buff, int* ofs)
{
    FileHeader Sign = {"IG", VERSION};
    
    memcpy(bin_buff, &Sign, sizeof(Sign));
    *ofs += sizeof(Sign);
}


void MakeLable (lable* lables, char* temp, char* check, int* ofs, int count)
{
    int mode = -1;
    static int last_word_lable = 1;
    
    if (strcmp(check, ":") != 0)
    {
        printf ("\nSyntaxError!\n");
        printf ("Delete this \"%s\" from label %s (word %d)\n\n", check + 1, temp, count);
        assert (!"OK");
    }
    else
    {
        *(check) ='\0';
        if (strspn(temp, "1234567890") != strlen(temp))
        {
            mode = MODE_WORD;
            if (strspn(temp, "1234567890") != 0)
            {
                printf ("\nSyntaxError!\n");
                printf ("Those symbols in label must be an integer or word: %s (word %d)\n", temp, count);
                printf ("Don't mix!\n\n");
                assert (!"OK");
            }
        }
        else
        {
            mode = MODE_NUMBER;
        }
    }
    
    if (mode == MODE_NUMBER)
    {
        int cur_lable = atoi(temp);
        if (cur_lable <= last_word_lable)
        {
            while (lables[last_word_lable].adr != -1)
                last_word_lable++;
            assert (last_word_lable < MAX_NUM_LABLES);
            
            
            lables[last_word_lable].adr = lables[cur_lable].adr;
            strcpy (lables[last_word_lable].name, lables[cur_lable].name);
            strcpy (lables[cur_lable].name, "");
        }
        lables[cur_lable].adr = *ofs;
    }
    else if (mode == MODE_WORD)
    {
        while (lables[last_word_lable].adr != -1)
            last_word_lable++;
        assert (last_word_lable < MAX_NUM_LABLES);
        
        lables[last_word_lable].adr  =  *ofs;
        strcpy(lables[last_word_lable].name, temp);
    }
    else
    {
        printf ("\nError in MakeLable, cannot find out mode\n\n");
        assert (!"OK");
    }
}


int SearchLable (lable* lables, char* temp)
{
    for (int i = 0; i < MAX_NUM_LABLES + 1; i++)
    {
        if (strcmp(temp, lables[i].name) == 0)
        {
            return i;
        }
    }
    return -1;
}


void ComplicComProcessing (char* buff, char* bin_buff, int* ofs, int* pos, char* temp, int* count, lable* lables, int num)
{
    if (num == CMD_PUSH)
    {
        PushPopProcessing (bin_buff, ofs, temp, *count);
    }
    else if (num == CMD_POP)
    {
        PushPopProcessing (bin_buff, ofs, temp, *count);
    }
    else if ((CMD_JMP  <= num && num <=  CMD_JT) || num == CMD_CALL)
    {
        JmpProcessing (bin_buff, ofs, temp, *count, lables);
    }
    else if (num == CMD_DRAW || num == CMD_CREATEWINDOW)
    {
        DrawProcessing (buff, bin_buff, ofs, pos, temp, count);
    }
    
}


void PushPopProcessing (char* bin_buff, int* ofs, char* temp_1, int count)
{
        char* temp_2 = strchr(temp_1, '+');
        if (temp_2)
        {
            *temp_2 = '\0';
            temp_2++;
        }
        char mode = ModeProcessing (temp_1, temp_2, count);
        
        bin_buff[*ofs] = mode;
        *ofs += sizeof(char);
        //printf (" %d ", mode);
        
        if ((mode & 4)/4)
            temp_1++;
            
        if ((mode & 1) && ((mode & 4)/4) && !((mode & 2)/2))
        {
            int len = strlen(temp_1);
            temp_1[len-1] = '\0';
        }
        
        ArgInsert (bin_buff, ofs, temp_1, count);
        if (temp_2)
            ArgInsert (bin_buff, ofs, temp_2, count);
}


char ModeProcessing (char* temp_1, char* temp_2, int count)
{
    int mem = 0;
    int reg = 0;
    int imid_const = 0;
    
    if (temp_1[0] == '[')
    {
        mem = 1;
    }
    
    if (strpbrk(temp_1, "1234567890"))
        imid_const = 1;
    else
        reg = 1;
    
    if (temp_2)
    {
        if (strpbrk(temp_2, "1234567890"))
            imid_const = 1;
        else
            reg = 1;
    }
    
    char mode = imid_const*2 + reg + mem*4;
    
    return mode;
}


int FindRegNumber (char* temp, int count)
{
    if (strcmpi(temp, "VRAM") == 0)
    {
        return VRAM;
    }
    else if (strcmpi(temp, "rax") == 0)
    {
        return RAX;
    }
    else if (strcmpi(temp, "rbx") == 0)
    {
        return RBX;
    }
    else if (strcmpi(temp, "rcx") == 0)
    {
        return RCX;
    }
    else if (strcmpi(temp, "rdx") == 0)
    {
        return RDX;
    }
    else if (strcmpi(temp, "cat") == 0)
    {
        return CAT;
    }
    else if (strcmpi(temp, "myau") == 0)
    {
        return MYAU;
    }
    else
    {
        printf ("ERROR in word %d (%s)\n", count, temp);
        printf ("Name of register after push or pop doesn't match any of the existing register");
        printf ("\n\n(word is any symbol-number combination that you entered)");
        assert (!"OK");
    }
}


void ArgInsert (char* bin_buff, int* ofs, char* temp, int count)
{
    if (strpbrk(temp, "1234567890"))
    {   
        *((double*) (bin_buff + *ofs)) = atof(temp);
        *ofs += sizeof(double);
        //printf ("%lg", atof(temp));
        
    }
    else
    {
        int reg_number = FindRegNumber (temp, count);
        bin_buff[*ofs] = reg_number;
        *ofs += sizeof(char);
        //printf ("%d", reg_number);
    }
}


void JmpProcessing (char* bin_buff, int* ofs, char* temp, int count, lable* lables)
{
    int mode = -1;
    
    if (temp[0] != ':')
    {
        printf ("\nSyntax Error!\n");
        printf ("Please enter lable in format \":x\", where x is integer (number of needed lable)\n");
        printf ("%s\n", temp);
        printf ("(in word %d)\n\n", count);
        assert (!"OK");
    }
    else
    {
        temp += 1;
        if (strspn(temp, "1234567890") != strlen(temp))
        {
            mode = MODE_WORD;
            if (strspn(temp, "1234567890") != 0)
            {
                printf ("\nSyntaxError!\n");
                printf ("Those symbols in label must be an integer or word: %s (word %d)\n", temp, count);
                printf ("Don't mix!\n\n");
                assert (!"OK");
            }
        }
        else
        {
            mode = MODE_NUMBER;
        }
    }
    
    int cur_lable = -1;
    if (mode == MODE_NUMBER)
    {
        cur_lable = atoi(temp);
    }
    else if (mode == MODE_WORD)
    {
        cur_lable = SearchLable (lables, temp);
    }
    else
    {
        printf ("\nError in JmpProcessing, cannot find out mode\n\n");
        assert (!"OK");
    }
    
    *(int*)(bin_buff + *ofs) = (cur_lable == -1) ?  -1 : lables[cur_lable].adr;
    *ofs += sizeof(int);
    //printf (" %d", lables[cur_lable].adr);
}


void DrawProcessing (char* buff, char* bin_buff, int* ofs, int*pos, char* temp, int* count)
{
    int com_len = 0;
    
    int R = atoi(temp);
    
    (*count)++;
    sscanf (buff + *pos, "%s%n", temp, &com_len);
        *pos += com_len;
    int G = atoi (temp);
    
    (*count)++;
    sscanf (buff + *pos, "%s%n", temp, &com_len);
        *pos += com_len;
    int B = atoi (temp);
    
    *(int*)(bin_buff + *ofs) = R;
    *ofs += sizeof(int);
    //printf (" %d", R);
    
    *(int*)(bin_buff + *ofs) = G;
    *ofs += sizeof(int);
    //printf (" %d", G);
    
    *(int*)(bin_buff + *ofs) = B;
    *ofs += sizeof(int);
    //printf (" %d", B);
    
}


text* ProgramDestructor (text* program)
{
    free (program->name);
    free (program->buff);
    free (program);
    
    program  = NULL;
    return program;
}


