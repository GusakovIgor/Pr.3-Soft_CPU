#include "CPU_header.h"

text* ProgramConstructor (char* name)
{   
    text* program = (text*) calloc (1, sizeof(text));
    assert (program);
    
    program->name = (char*) calloc (MAX_FILENAME, sizeof(char));
    assert (program->name);
    program->name = name;
    assert (strlen(program->name) < MAX_FILENAME);
    
    size_t num_symb = TextGetter (program);
    program->num_words = WordsCounter (program->buff, num_symb);
    program->name = NameProcessing (program->name);
    
    return program;
}


char* NameProcessing (char* name)
{
    int len = strlen(name);
    assert (name[len-4] == '.');
    
    const char* extension = ".bin";
    assert (extension[0] == '.');
    
    for (int i = len-4, j = 0; i <= len-1; i++, j++)
    {
        name[i] = extension[j];
    }
    
    printf ("\nYou can see byte-code of your program in file: %s\n", name);

    return name;
}


size_t TextGetter (text* program)
{
    size_t size = SizeGetter (program->name);
    program->buff = (char*) calloc (size + 1, sizeof(char));
    assert (program->buff != NULL);
    
    size_t num_symbals = TextReader (program, size);
    assert (num_symbals);
    
    return num_symbals;
}


size_t SizeGetter (const char* name)
{
    FILE* file = fopen (name, "r");
    
    struct stat finfo;
    stat(name, &finfo);
    size_t size = finfo.st_size;
        
    fclose (file);
    
    return size;
}


size_t TextReader (text* program, size_t size)
{
    FILE* file = fopen (program->name, "r");
    
    size_t num_symb = fread (program->buff, sizeof(char), size, file);
    *(program->buff + num_symb) = '\0';
    
    fclose (file);
    
    return num_symb;
}


size_t WordsCounter (char* text, size_t num_symbals)
{
    assert (text);
    size_t num_words = 1;
    
    int i = 0;
    if (text[i] == ';')
        i = CommentCleaner (text, num_symbals, i);
    
    for (; i < num_symbals - 1; i++)
        while (isspace(text[i]))
        {
            if (text[i + 1] == ';')
            {
                i = CommentCleaner (text, num_symbals, ++i);
            }
            else if (!isspace(text[i + 1]))
            {
                num_words++;
                break;
            }
            else
                i++;
        }
    
    return num_words;
}


int CommentCleaner (char* text, size_t num_symbals, int cur_symbal)
{
    assert (text[cur_symbal] == ';');
    
    while (text[cur_symbal] != '\n' && cur_symbal < num_symbals)
    {
        text[cur_symbal] = ' ';
        cur_symbal++;
        if (text[cur_symbal] == ';')
        {
            printf("ERROR in comments, you can't put one in another");
            assert(!"OK");
        }
    }
    
    return cur_symbal;
}


