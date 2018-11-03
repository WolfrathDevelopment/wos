/*
 * debug.c
 * Joel Wolfrath, 2013
 *
 * Implementation of debug functions
 */

#include <tools/debug.h>

void trace_stack(uint32_t maxFrames)
{
    /* Base pointer for the current frame is 8 bytes
     * before the address of the stack parm */
    uint32_t* ebp = &maxFrames - 2;
    printf("Stack trace:\n");

    uint32_t currentFrame = 0;
    for(; currentFrame < maxFrames; ++currentFrame)
    {
        /* Get the instruction address for the caller */
        uint32_t eip = ebp[1];

        /* Check to make sure there is a caller */
        if(eip == 0)
        {
            break;
        }
    
        /* Go back one more frame */
        ebp = (uint32*)ebp[0];
        uint32_t* args = &ebp[2];
        printf("  0x%p\n", eip);
    }
}

void _debug(char* msg, char* file, uint32_t line)
{
    printf("\nDEBUG (%s at line %d)> %s\n", file, line, msg);
}

void _panic(char* msg, char* file, uint32_t line)
{
    printf("\nPANIC (%s at line %d)> %s", file, line, msg);
    for(;;)
        ;
}

void _assert(int32_t expression, char* file, uint32_t line)
{
    if(!expression)
    {
        _panic("Assertion Failed", file, line);
    }
}

void _trace_rc(OsRc rc, char* file, uint32_t line)
{
    printf("RC TRACE: ");

    switch(rc)
    {
        case RC_SUCCESS:
            printf("SUCCESS");
            break;

        case RC_INVALID_PARM:
            printf("INVALID_PARM");
            break;

        case RC_ALLOC_FAILED:
            printf("ALLOC_FAILED");
            break;

        default:
            printf("UNKNOWN");
            break;
    }

    printf(" (%s at line %d)\n", file, line);
}
