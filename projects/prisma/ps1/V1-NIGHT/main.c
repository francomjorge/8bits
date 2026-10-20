/*
 * PRISMA-PI / PlayStation 1
 * V1-NIGHT
 *
 * Base: PSn00bSDK examples/graphics/rgb24/main.c
 * Original RGB24 example by Lameguy64 / PSn00bSDK.
 *
 * This first PS1 milestone deliberately does ONE thing:
 * boot using PSn00bSDK and display the NIGHT DRIVE image as a native TIM.
 * No custom boot code, no custom GPU initialization, no audio yet.
 */

#include <sys/types.h>
#include <psxgpu.h>

extern u_long tim_image[];

int main(void) {
    DISPENV disp;
    TIM_IMAGE tim;

    /* Same initialization pattern as PSn00bSDK's rgb24 example. */
    ResetGraph(0);

    /* Native PS1 320x240 display, 24-bit direct color for this static proof. */
    SetDefDispEnv(&disp, 0, 0, 320, 240);
    disp.isrgb24 = 1;
    disp.isinter = 0;

    PutDispEnv(&disp);
    SetDispMask(1);

    /* Load the prebuilt NIGHT DRIVE TIM directly into VRAM. */
    GetTimInfo(tim_image, &tim);
    LoadImage(tim.prect, tim.paddr);
    DrawSync(0);

    for (;;) {
        /* V1 is intentionally static. */
    }

    return 0;
}
