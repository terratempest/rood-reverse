#include "common.h"
#include <libgte.h>
#include <libgpu.h>
#include <memory.h>

typedef struct {
    TILE tile;
    DR_MODE mode;
    int maxchars;
    SPRT_8* sprt;
    char* text;
    int count;
    int noclip;
} FntStream;

extern FntStream D_80032864[8];
extern int D_800329E4;
extern u_long D_800329EC[];
extern u_short D_8003E2A0;
extern u_short D_8003E2A2;

INCLUDE_ASM("build/src/SLUS_010.40/nonmatchings/libgpu/FONT", SetDumpFnt);

void FntLoad(int tx, int ty)
{
    D_8003E2A2 = LoadClut2(D_800329EC, tx, ty + 0x80);
    D_8003E2A0 = LoadTPage(D_800329EC + 0x80, 0, 0, tx, ty, 0x80, 0x20);
    D_800329E4 = 0;
    memset(D_80032864, 0, sizeof(D_80032864));
}

INCLUDE_ASM("build/src/SLUS_010.40/nonmatchings/libgpu/FONT", FntOpen);

INCLUDE_ASM("build/src/SLUS_010.40/nonmatchings/libgpu/FONT", FntFlush);

INCLUDE_RODATA("build/src/SLUS_010.40/nonmatchings/libgpu/FONT", D_800107C4);

INCLUDE_ASM("build/src/SLUS_010.40/nonmatchings/libgpu/FONT", FntPrint);
