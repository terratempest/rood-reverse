#include "common.h"
#include <libapi.h>
#include <setjmp.h>

typedef struct {
    int unk0;
    int unk4;
    int unk8;
    void (*unkC)(void);
} D_800320D4_t;

extern D_800320D4_t* D_800320D4;
extern volatile unsigned short* D_800320DC;
extern unsigned short D_8003104E;

typedef struct {
    unsigned short interruptsInitialized;
    unsigned short inInterrupt;
    void (*handlers[11])();
    unsigned short enabledInterruptsMask;
    unsigned short savedMask;
    int savedPcr;
    jmp_buf buf;
    int stack[1024];
} IntrEnv;

extern IntrEnv D_8003104C;
extern void ChangeClearRCnt(int, int);

void ResetCallback(void) { D_800320D4->unkC(); }

INCLUDE_ASM("build/src/SLUS_010.40/nonmatchings/libetc/INTR", InterruptCallback);

INCLUDE_ASM("build/src/SLUS_010.40/nonmatchings/libetc/INTR", DMACallback);

INCLUDE_ASM("build/src/SLUS_010.40/nonmatchings/libetc/INTR", VSyncCallback);

INCLUDE_ASM("build/src/SLUS_010.40/nonmatchings/libetc/INTR", VSyncCallbacks);

INCLUDE_ASM("build/src/SLUS_010.40/nonmatchings/libetc/INTR", StopCallback);

INCLUDE_ASM("build/src/SLUS_010.40/nonmatchings/libetc/INTR", RestartCallback);

int CheckCallback(void) { return D_8003104E; }

int GetIntrMask(void) { return *D_800320DC; }

int SetIntrMask(int mask)
{
    unsigned short oldMask = *D_800320DC;
    *D_800320DC = mask;
    return oldMask;
}

INCLUDE_ASM("build/src/SLUS_010.40/nonmatchings/libetc/INTR", func_8001FA68);

INCLUDE_RODATA("build/src/SLUS_010.40/nonmatchings/libetc/INTR", D_800101E4);

INCLUDE_ASM("build/src/SLUS_010.40/nonmatchings/libetc/INTR", func_8001FB40);

void* func_8001FD10(int irq, void (*handler)())
{
    void (*previousHandler)() = D_8003104C.handlers[irq];
    int mask;

    if (handler != previousHandler && D_8003104C.interruptsInitialized != 0) {
        mask = *D_800320DC;
        *D_800320DC = 0;
        if (handler != NULL) {
            D_8003104C.handlers[irq] = handler;
            mask |= 1 << irq;
            D_8003104C.enabledInterruptsMask |= 1 << irq;
        } else {
            D_8003104C.handlers[irq] = NULL;
            mask &= ~(1 << irq);
            D_8003104C.enabledInterruptsMask &= ~(1 << irq);
        }
        if (irq == 0) {
            ChangeClearPAD(handler == NULL);
            ChangeClearRCnt(3, handler == NULL);
        }
        if (irq == 4) {
            ChangeClearRCnt(0, handler == NULL);
        }
        if (irq == 5) {
            ChangeClearRCnt(1, handler == NULL);
        }
        if (irq == 6) {
            ChangeClearRCnt(2, handler == NULL);
        }
        *D_800320DC = mask;
    }
    return previousHandler;
}

INCLUDE_ASM("build/src/SLUS_010.40/nonmatchings/libetc/INTR", func_8001FE58);

INCLUDE_ASM("build/src/SLUS_010.40/nonmatchings/libetc/INTR", func_8001FEF8);

INCLUDE_ASM("build/src/SLUS_010.40/nonmatchings/libetc/INTR", memzero);
