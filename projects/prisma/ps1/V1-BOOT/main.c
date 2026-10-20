/*
 * PRISMA-PI PS1 V1-BOOT
 *
 * Adapted directly from PSn00bSDK examples/beginner/hello/main.c.
 * Original example: (C) 2020-2023 Lameguy64, spicyjpeg - MPL licensed.
 *
 * Purpose: prove the PS1 toolchain, CD image and emulator boot path before
 * adding NIGHT DRIVE assets.
 */

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <psxgpu.h>

#define OT_LENGTH 16
#define BUFFER_LENGTH 8192
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
    SetDefDrawEnv(&ctx->buffers[0].draw_env, 0, 0, w, h);
    SetDefDispEnv(&ctx->buffers[0].disp_env, 0, 0, w, h);
    SetDefDrawEnv(&ctx->buffers[1].draw_env, 0, h, w, h);
    SetDefDispEnv(&ctx->buffers[1].disp_env, 0, h, w, h);

    setRGB0(&ctx->buffers[0].draw_env, r, g, b);
    setRGB0(&ctx->buffers[1].draw_env, r, g, b);
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

    RenderBuffer *draw_buffer = &ctx->buffers[ctx->active_buffer];
    RenderBuffer *disp_buffer = &ctx->buffers[ctx->active_buffer ^ 1];

    PutDispEnv(&disp_buffer->disp_env);
    DrawOTagEnv(&draw_buffer->ot[OT_LENGTH - 1], &draw_buffer->draw_env);

    ctx->active_buffer ^= 1;
    ctx->next_packet = disp_buffer->buffer;
    ClearOTagR(disp_buffer->ot, OT_LENGTH);
}

static void *new_primitive(RenderContext *ctx, int z, size_t size) {
    RenderBuffer *buffer = &ctx->buffers[ctx->active_buffer];
    uint8_t *prim = ctx->next_packet;

    addPrim(&buffer->ot[z], prim);
    ctx->next_packet += size;
    assert(ctx->next_packet <= &buffer->buffer[BUFFER_LENGTH]);
    return prim;
}

static void draw_text(RenderContext *ctx, int x, int y, int z, const char *text) {
    RenderBuffer *buffer = &ctx->buffers[ctx->active_buffer];
    ctx->next_packet = (uint8_t *) FntSort(
        &buffer->ot[z], ctx->next_packet, x, y, text
    );
    assert(ctx->next_packet <= &buffer->buffer[BUFFER_LENGTH]);
}

int main(int argc, const char **argv) {
    ResetGraph(0);
    FntLoad(960, 0);

    RenderContext ctx;
    setup_context(&ctx, SCREEN_XRES, SCREEN_YRES, 8, 8, 24);

    int x = 0;
    int dx = 1;

    for (;;) {
        if (x < 0 || x > SCREEN_XRES - 64)
            dx = -dx;
        x += dx;

        TILE *tile = (TILE *) new_primitive(&ctx, 1, sizeof(TILE));
        setTile(tile);
        setXY0(tile, x, 112);
        setWH(tile, 64, 16);
        setRGB0(tile, 0, 220, 240);

        draw_text(&ctx, 72, 88, 0, "PRISMA-PI / PLAYSTATION 1");
        draw_text(&ctx, 88, 104, 0, "V1-BOOT OK");

        flip_buffers(&ctx);
    }

    return 0;
}
