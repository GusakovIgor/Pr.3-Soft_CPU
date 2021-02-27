#include "CPU_header.h"

int main (int argc, const char* argv[])
{
    char* name = (char*) calloc (MAX_FILENAME, sizeof(char));
    //name = argv[1];
    name = NameProcessing ((char*) argv[1]);
    
    FILE* file = fopen (name, "rb");
    char* code = (char*) calloc (MAX_CODE_LEN, sizeof(char));
    size_t code_size = fread (code, sizeof(char), MAX_CODE_LEN, file);
    fclose (file);
    
    code += sizeof (FileHeader);
    code_size -= sizeof (FileHeader);
    int index = 0;
    
    for (int infex = 0; index < code_size; index++)
    {
        printf ("%d\n", code[index]);
    }    
    
    return 0;
}
