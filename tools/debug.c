/*
 * debug.c
 * Joel Wolfrath, 2013
 *
 * Implementation of debug functions
 */

#include <tools/debug.h>
#include <io/console.h>

void dump_memory(uint8_t* address, uint32_t length)
{
    printf("Dumping memory at %x:\n", address);
    for(int offset=0; offset < length; offset += (4 * sizeof(uint32_t)))
    {
        uint32_t v1 = *((uint32_t*) (address + offset));
        uint32_t v2 = *((uint32_t*) (address + offset + 4));
        uint32_t v3 = *((uint32_t*) (address + offset + 8));
        uint32_t v4 = *((uint32_t*) (address + offset + 12));
        printf("%x | %x %x %x %x\n", address + offset, v1, v2, v3, v4);
    }
}

void trace_stack(uint32_t maxFrames)
{
    /* Base pointer for the current frame is 8 bytes
     * before the address of the stack parm
     */
    uint32_t* ebp = &maxFrames - 2;
    printf("Stack trace:\n");

    uint32_t currentFrame = 0;
    for(; currentFrame < maxFrames; ++currentFrame)
    {
        /* Get the instruction address for the caller */
        uint32_t eip = ebp[1];

        /* Check to make sure there is a caller */
        if(eip == 0 || *ebp == 0)
        {
            break;
        }
    
        /* Go back one more frame */
        ebp = (uint32_t*)ebp[0];
        printf("  0x%p\n", eip);
    }

    printf("\n");
}

void _die(OsBaseRegs* regs, char* file, uint32_t line)
{
    uint32_t eip = get_caller_eip();
    printf("\n----------------------------------");
    printf("Terminating in %s line %d\n", file, line);
    printf("eip: %x\n\n", eip);

    printf("edi: %x esi: %x\n", regs->edi, regs->esi);
    printf("ebp: %x esp: %x\n", regs->ebp, regs->esp);
    printf("eax: %x ebx: %x\n", regs->eax, regs->ebx);
    printf("ecx: %x edx: %x\n", regs->ecx, regs->edx);

    printf("\n");
    trace_stack(6);

    for(;;)
        ;
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
