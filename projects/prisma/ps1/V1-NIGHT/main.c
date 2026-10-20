/*
 * V1-NIGHT / PRISMA-PI / PlayStation 1
 *
 * Base: PSn00bSDK examples/beginner/hello/main.c
 * Uses the SDK's proven double-buffering, ordering table, TILE primitives
 * and debug-font API. No custom boot or low-level GPU bring-up.
 */

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <psxgpu.h>

#define OT_LENGTH 16
#define BUFFER_LENGTH 16384
#define SCREEN_XRES 320
#define SCREEN_YRES 240

typedef struct {
    DISPENV disp_env;
    DRAWENV draw_env;
    uint32_t ot[OT_LENGTH];
    uint8_t buffer[BUFFER_LENGTH];
} RenderBuffer;

typedef struct {
    RenderBuffer buffers[2];
    uint8_t *next_packet;
    int active_buffer;
} RenderContext;

static void setup_context(RenderContext *ctx, int w, int h, int r, int g, int b) {
    SetDefDrawEnv(&(ctx->buffers[0].draw_env), 0, 0, w, h);
    SetDefDispEnv(&(ctx->buffers[0].disp_env), 0, 0, w, h);
    SetDefDrawEnv(&(ctx->buffers[1].draw_env), 0, h, w, h);
    SetDefDispEnv(&(ctx->buffers[1].disp_env), 0, h, w, h);

    setRGB0(&(ctx->buffers[0].draw_env), r, g, b);
    setRGB0(&(ctx->buffers[1].draw_env), r, g, b);
    ctx->buffers[0].draw_env.isbg = 1;
    ctx->buffers[1].draw_env.isbg = 1;

    ctx->active_buffer = 0;
    ctx->next_packet = ctx->buffers[0].buffer;
    ClearOTagR(ctx->buffers[0].ot, OT_LENGTH);
    SetDispMask(1);
}

static void flip_buffers(RenderContext *ctx) {
    DrawSync(0);
    VSync(0);

    RenderBuffer *draw_buffer = &(ctx->buffers[ctx->active_buffer]);
    RenderBuffer *disp_buffer = &(ctx->buffers[ctx->active_buffer ^ 1]);

    PutDispEnv(&(disp_buffer->disp_env));
    DrawOTagEnv(&(draw_buffer->ot[OT_LENGTH - 1]), &(draw_buffer->draw_env));

    ctx->active_buffer ^= 1;
    ctx->next_packet = disp_buffer->buffer;
    ClearOTagR(disp_buffer->ot, OT_LENGTH);
}

static void *new_primitive(RenderContext *ctx, int z, size_t size) {
    RenderBuffer *buffer = &(ctx->buffers[ctx->active_buffer]);
    uint8_t *prim = ctx->next_packet;
    addPrim(&(buffer->ot[z]), prim);
    ctx->next_packet += size;
    assert(ctx->next_packet <= &(buffer->buffer[BUFFER_LENGTH]));
    return (void *)prim;
}

static void rect(RenderContext *ctx, int x, int y, int w, int h,
                 int r, int g, int b, int z) {
    TILE *t = (TILE *)new_primitive(ctx, z, sizeof(TILE));
    setTile(t);
    setXY0(t, x, y);
    setWH(t, w, h);
    setRGB0(t, r, g, b);
}

static void label(RenderContext *ctx, int x, int y, int z, const char *s) {
    RenderBuffer *buffer = &(ctx->buffers[ctx->active_buffer]);
    ctx->next_packet = (uint8_t *)FntSort(&(buffer->ot[z]), ctx->next_packet, x, y, s);
    assert(ctx->next_packet <= &(buffer->buffer[BUFFER_LENGTH]));
}

static void draw_night(RenderContext *ctx) {
    rect(ctx, 0,   0, 320, 28,  8,  6, 35, 8);
    rect(ctx, 0,  28, 320, 26, 22,  8, 58, 8);
    rect(ctx, 0,  54, 320, 25, 55, 12, 82, 8);
    rect(ctx, 0,  79, 320, 23,115, 22, 95, 8);

    rect(ctx, 138, 44, 44,  8, 255,205, 90, 7);
    rect(ctx, 132, 52, 56,  8, 255,150, 95, 7);
    rect(ctx, 128, 60, 64,  8, 255, 90,125, 7);
    rect(ctx, 126, 68, 68,  7, 245, 45,150, 7);

    rect(ctx, 0, 102, 320, 30,  5,  6, 20, 7);
    rect(ctx,  8,  78, 25, 54, 10, 10, 33, 6);
    rect(ctx, 40,  88, 20, 44, 14, 10, 38, 6);
    rect(ctx, 67,  72, 29, 60,  8, 10, 31, 6);
    rect(ctx,103,  91, 19, 41, 12, 10, 39, 6);
    rect(ctx,204,  84, 27, 48, 10,  9, 35, 6);
    rect(ctx,239,  70, 34, 62,  7,  9, 30, 6);
    rect(ctx,282,  87, 26, 45, 13, 10, 38, 6);

    for (int x = 14; x < 300; x += 18) {
        rect(ctx, x, 108, 5, 3, 0, 220, 255, 5);
        if ((x / 18) & 1)
            rect(ctx, x + 5, 118, 4, 3, 255, 45, 185, 5);
    }

    rect(ctx, 0, 132, 320, 50,  6,  5, 16, 8);
    rect(ctx, 0, 132, 320,  2,255, 45,170, 5);
    rect(ctx, 0, 155, 320,  1,  0,190,255, 5);
    rect(ctx, 0, 178, 320,  1,255, 45,170, 5);
    rect(ctx,158,136,  4,  6,235,215,115, 5);
    rect(ctx,155,148, 10,  5,235,215,115, 5);
    rect(ctx,151,160, 18,  4,235,215,115, 5);
    rect(ctx,146,172, 28,  3,235,215,115, 5);

    rect(ctx, 0, 182, 320, 58,  3,  7, 18, 8);
    rect(ctx, 0, 182, 320,  2,  0,220,255, 5);
    rect(ctx,  8,192, 94, 37,  8, 17, 36, 7);
    rect(ctx,111,192, 91, 37,  8, 17, 36, 7);
    rect(ctx,211,192,101, 37,  8, 17, 36, 7);

    label(ctx, 10,   8, 0, "PRISMA-PI / PS1");
    label(ctx, 10,  20, 0, "V1-NIGHT");
    label(ctx, 17, 198, 0, "NIGHT DRIVE");
    label(ctx, 17, 212, 0, "VISUAL TEST");
    label(ctx,120, 198, 0, "GPU: OK");
    label(ctx,120, 212, 0, "320x240");
    label(ctx,220, 198, 0, "NEXT: PAD");
    label(ctx,220, 212, 0, "NEXT: AUDIO");
}

int main(int argc, const char **argv) {
    ResetGraph(0);
    FntLoad(960, 0);

    RenderContext ctx;
    setup_context(&ctx, SCREEN_XRES, SCREEN_YRES, 0, 0, 0);

    for (;;) {
        draw_night(&ctx);
        flip_buffers(&ctx);
    }

    return 0;
}
