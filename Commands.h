/*** System ***/

DEF_CMD (HLT,  0, 0, 
                    {
                    processor->pc = processor->code_size;    
                    })

DEF_CMD (START, 1, 0,
                    {
                    processor->pc = *(int*)(processor->code + processor->pc + 1);
                    })

DEF_CMD (IN,   2, 0, 
                    {
                    int number = 0;
                    scanf ("%d", &number);
                    StackPush (processor->stack, number);
                    processor->pc++;
                    })

DEF_CMD (OUT,  3, 0, 
                    {
                    double number = StackPop (processor->stack);
                    if (number > 1e9)
                    {
                        printf ("\n%lg\n\n", number);
                    }
                    else
                    {
                        printf ("\n%.3f\n\n", number);
                    }
                    processor->pc++;
                    })



/*** Stack **/

DEF_CMD (PUSH, 4, 2, 
                    {
                    ++processor->pc;
                    int mode = processor->code[processor->pc++];
                    StackPush (processor->stack, ArgGetter(processor, mode, RAM));
                    })

DEF_CMD (POP,  5, 2, 
                    {
                    double temp = StackPop (processor->stack);
                    ++processor->pc;
                    int mode = processor->code[processor->pc++];
                    double arg = 0;
                    if (mode & 1)
                        {
                            arg += processor->registers[processor->code[processor->pc++]];
                        }
                    if ((mode & 2)/2)
                        {
                            arg += *(double*)(processor->code + processor->pc);
                            processor->pc += sizeof(double);
                        }
                    if ((mode & 4)/4)
                        {
                            *(RAM + (int) arg) = temp;
                        }
                    else
                        processor->registers[processor->code[processor->pc - 1]] = temp;
                    })



/*** Arithmetic ***/

DEF_CMD (ADD,  6, 0, 
                    {
                    StackPush (processor->stack, StackPop(processor->stack) + StackPop(processor->stack));
                    processor->pc++;
                    })

DEF_CMD (SUB,  7, 0, 
                    {
                    double temp = StackPop (processor->stack);
                    StackPush (processor->stack, StackPop(processor->stack) - temp);
                    processor->pc++;
                    })

DEF_CMD (MULT, 8, 0, 
                    {
                    StackPush (processor->stack, StackPop(processor->stack) * StackPop(processor->stack));
                    processor->pc++;
                    })

DEF_CMD (DIV, 9, 0, 
                    {
                    double temp = StackPop (processor->stack);
                    if (temp != 0)
                        StackPush (processor->stack, StackPop(processor->stack) / temp);
                    else
                        {
                            printf ("Trying to segment by zero");
                            assert (!"OK");
                        }
                    processor->pc++;
                    })

DEF_CMD (POW,  10, 0, 
                    {
                    int temp = StackPop(processor->stack);
                    StackPush (processor->stack, pow(StackPop(processor->stack), temp));
                    processor->pc++;
                    })

DEF_CMD (SQRT, 11, 0, 
                    {
                    StackPush (processor->stack, sqrt(StackPop(processor->stack)));
                    processor->pc++;
                    })

DEF_CMD (SIN, 12, 0, 
                    {
                    StackPush (processor->stack, sin(StackPop(processor->stack)));
                    processor->pc++;
                    })

DEF_CMD (COS, 13, 0, 
                    {
                    StackPush (processor->stack, cos(StackPop(processor->stack)));
                    processor->pc++;
                    })



/*** Jumps ***/

DEF_CMD (JMP, 14, 1, 
                    {
                    processor->pc = *(int*)(processor->code + processor->pc + 1);
                    })

DEF_CMD (JA,  15, 1, 
                    {
                    double temp1 = StackPop(processor->stack);
                    double temp2 = StackPop(processor->stack);
                    if (temp2 > temp1)
                        {
                            processor->pc = *(int*)(processor->code + processor->pc + 1);
                        }
                    else
                        {
                            processor->pc += sizeof(char) + sizeof(int);
                        }
                    })

DEF_CMD (JAE, 16, 1, 
                    {
                    double temp1 = StackPop(processor->stack);
                    double temp2 = StackPop(processor->stack);
                    if (temp2 >= temp1)
                        {
                            processor->pc = *(int*)(processor->code + processor->pc + 1);
                        }
                    else
                        {
                            processor->pc += sizeof(char) + sizeof(int);
                        }
                    })

DEF_CMD (JB,  17, 1, 
                    {
                    double temp1 = StackPop(processor->stack);
                    double temp2 = StackPop(processor->stack);
                    if (temp2 < temp1)
                        {
                            processor->pc = *(int*)(processor->code + processor->pc + 1);
                        }
                    else
                        {
                            processor->pc += sizeof(char) + sizeof(int);
                        }
                    })

DEF_CMD (JBE, 18, 1, 
                    {
                    double temp1 = StackPop(processor->stack);
                    double temp2 = StackPop(processor->stack);
                    if (temp2 <= temp1)
                        {
                            processor->pc = *(int*)(processor->code + processor->pc + 1);
                        }
                    else
                        {
                            processor->pc += sizeof(char) + sizeof(int);
                        }
                    })

DEF_CMD (JE,  19, 1, 
                    {
                    double temp1 = StackPop(processor->stack);
                    double temp2 = StackPop(processor->stack);
                    if (temp2 == temp1)
                        {
                            processor->pc = *(int*)(processor->code + processor->pc + 1);
                        }
                    else
                        {
                            processor->pc += sizeof(char) + sizeof(int);
                        }
                    })

DEF_CMD (JNE, 20, 1, 
                    {
                    double temp1 = StackPop(processor->stack);
                    double temp2 = StackPop(processor->stack);
                    if (temp2 != temp1)
                        {
                            processor->pc = *(int*)(processor->code + processor->pc + 1);
                        }
                    else
                        {
                            processor->pc += sizeof(char) + sizeof(int);
                        }
                    })
DEF_CMD (JT,  21, 1, 
                    {
                    
                    })



/*** Functions ***/

DEF_CMD (CALL, 22, 1,
                    { 
                    StackPush (processor->calls, processor->pc + sizeof(char) + sizeof(int));
                    processor->pc = *(int*)(processor->code + processor->pc + 1);
                    })

DEF_CMD (RET,  23, 0, 
                    {
                    processor->pc = StackPop(processor->calls);
                    })



/*** Special ***/

DEF_CMD (ERR, 24, 0,
                    {
                    printf("\nDiscriminant < 0\n\n");
                    processor->pc += sizeof(char);
                    })

DEF_CMD (NOROOTS, 25, 0,
                    {
                    printf("\nNo roots\n\n");
                    processor->pc += sizeof(char);
                    })

DEF_CMD (INFROOTS, 26, 0,
                    {
                    printf("\nINF roots\n\n");
                    processor->pc += sizeof(char);
                    })

DEF_CMD (DRAW, 27, 3,
                    {
                    printf ("VSE SLOGHNO\n");
                    const int X_size = RAM[0];
                    const int Y_size = RAM[1];
                    
                    processor->pc += sizeof(char);
                    int R = *(int*)(processor->code + processor->pc);
                    
                    processor->pc += sizeof(int);
                    int G = *(int*)(processor->code + processor->pc);
                    
                    processor->pc += sizeof(int);
                    int B = *(int*)(processor->code + processor->pc);
                    
                    processor->pc += sizeof(int);
                    
                    RGBQUAD* BUFF = txVideoMemory();
                    assert (BUFF);
                    txLock();
                    for (int i = 0; i < X_size*Y_size; i++)
                        if (VRAM[i])
                        {
                            (*(BUFF + i)).rgbRed   = R;
                            (*(BUFF + i)).rgbGreen = G;
                            (*(BUFF + i)).rgbBlue  = B;
                        }
                    txUnlock();
                    txRedrawWindow();
                    
                    })

DEF_CMD (CREATEWINDOW, 28, 3,
                    {
                    const int X_size = RAM[0];
                    const int Y_size = RAM[1];
                    
                    //txCreateWindow (X_size, Y_size);
                    
                    Application* App = (Application*) calloc (1, sizeof(Application));

                    SDL_Init (App);

                    processor->pc += sizeof(char);
                    int R = *(int*)(processor->code + processor->pc);
                    
                    processor->pc += sizeof(int);
                    int G = *(int*)(processor->code + processor->pc);
                    
                    processor->pc += sizeof(int);
                    int B = *(int*)(processor->code + processor->pc);
                    
                    txSetFillColor (RGB(R, G, B));
                    txRectangle (0, Y_size, X_size, 0);
                    
                    processor->pc += sizeof (int);
                    })
                    
DEF_CMD (LROUND, 29, 0,
                    {
                       double temp = StackPop(processor->stack);
                       StackPush (processor->stack, lround(temp));
                       processor->pc += sizeof (char);     
                    })