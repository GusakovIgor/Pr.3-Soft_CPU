#include "CPU_header.h"

text* Compiler()
{
    struct text* program = (text*) calloc (1, sizeof(text));
    program->name = (char*) calloc (MAX_FILENAME + 1, sizeof(char));       // + 1 for converting .txt into .code
    assert (program->name);
    printf ("Please, enter name of the TXT file with your programm: ");
    scanf  ("%s", program->name);
    
    if (strlen(program->name) > MAX_FILENAME)
    {
        printf ("\nYour file name should be no longer than %d symbols\n\n", MAX_FILENAME);
        assert(!"OK");
    }
    
    program = ProgramConstructor (program);
    Assembler (program);
    
    /* Obiazatelno sdelay free v calculatore!!!
    free(program->name);
    free(program->ptr);
    */
    
    return 0;
}


void Assembler (text* program)
{
    FILE* file = fopen (program->name, "wb");
//    char* word = (char*) calloc (MAX_COMAND_LENGTH, sizeof(char));
    char* pointer = program->ptr;
    int   pos  =  0;

    int   check_lines = -1;

    for (int i = 0; i < program->num_lines; i++)
    {
        /*
        sscanf (pointer, "%s%n", word, &pos);
        pointer += pos + 1;
        */
        
//        check_lines = ComandsConverter (/*word, */file);
        check_lines = ComandsConverter (program);
    }
    
    if (check_lines != program->num_lines)
    {
        printf ("INPUT ERROR: Bad format, please, enter your program in that way:\n");
        assert (!"OK");
    }

    fclose (file);
}


int ComandsConverter (/*char* word, */FILE* bite_code)
{
    /*
    char*   comand_array[] = {"hlt", "in", "out", "push", "pop", "add", "sub", "mul", "segm", "pow", "sqrt", "sin", "cos"};
    char* register_array[] = {"", "rax", "rbx", "rcx", "rdx"};
    
    static int line = 1;
    static int last_comand = -1;
    
    int com_num = -1;
    int reg_num =  1;
    
    for (com_num = -1; com_num < NUM_COMANDS; com_num++)
    {
        if (strpbrk (word, "1234567890") || (com_num >= 0 && strcmp (word, comand_array[com_num]) == 0))
            break;
    }
    for (reg_num = 1; reg_num <= NUM_REGISTERS; reg_num++)
    {
        if (strcmp (word, register_array[reg_num]) == 0)
            break;
    }*/
    
    /*DEBUUG
    printf ("current word is \"%s\"\n", word);
    */
    
    /*
    CheckErrors(com_num, reg_num, last_comand, line)
    
    if (com_num == -1)                                      // Entering nuber, not command
    {
        if (last_comand == CMD_PUSH)
        {
            fprintf (bite_code, " 1 ");
            line++;
        }
        else
            assert (!"OK");
        
        fprintf (bite_code, "%d\n", atoi(word));
        line++;
    }
    else
        if (last_comand == CMD_PUSH)
        {
            fprintf (bite_code, " 2 ");
            fprintf (bite_code, "%d\n", reg_num);
            line++;
        }
        else
        {   
            fprintf (bite_code, "%d", com_num);
            if (com_num != CMD_PUSH && com_num != CMD_POP)
            {
                fprintf (bite_code, "\n");
                line++;
            }
        }
        
    last_comand = com_num;
    
    return line;
    */
}

int ComandsConverter (text* program)
{
    
    sscanf ()
    #define DEF_CMD(name, num, arg)         \
            if (strcmp(cmd, #name) == 0)    \
            if (arg > 0)                    \
            {                               \
                
            }
    
    #include "commands.h"
    else
        printf ("syntax error");
    
}
