/*
 * V2-SOUND / PRISMA-PI / PlayStation 1
 *
 * Audio path is adapted directly from the official PSn00bSDK
 * examples/sound/vagsample/main.c example. The goal of this version is only
 * to prove a stable PRISMA control -> SPU playback path before adding the
 * NIGHT DRIVE image and a real patch bank.
 *
 * V2 live controls:
 *   UP/DOWN    volume 0..15
 *   LEFT/RIGHT tone (sample pitch) 0..15
 *   X          trigger the sample
 *   O          stop the voice
 *
 * The remaining NIGHT DRIVE parameters are displayed but intentionally not
 * wired yet. One subsystem at a time.
 */

#include <stdint.h>
#include <psxgpu.h>
#include <psxapi.h>
#include <psxpad.h>
#include <psxspu.h>
#include <hwregs_c.h>

extern const uint8_t night_sample[];

#define SCREEN_XRES 320
#define SCREEN_YRES 240

#define BGCOLOR_R 3
#define BGCOLOR_G 5
#define BGCOLOR_B 18

typedef struct {
    DISPENV disp;
    DRAWENV draw;
} Framebuffer;

typedef struct {
    Framebuffer db[2];
    int db_active;
} RenderContext;

static RenderContext ctx;

static void init_context(RenderContext *ctx) {
    Framebuffer *db;

    ResetGraph(0);
    ctx->db_active = 0;

    db = &(ctx->db[0]);
    SetDefDispEnv(&(db->disp),           0, 0, SCREEN_XRES, SCREEN_YRES);
    SetDefDrawEnv(&(db->draw), SCREEN_XRES, 0, SCREEN_XRES, SCREEN_YRES);
    setRGB0(&(db->draw), BGCOLOR_R, BGCOLOR_G, BGCOLOR_B);
    db->draw.isbg = 1;
    db->draw.dtd  = 1;

    db = &(ctx->db[1]);
    SetDefDispEnv(&(db->disp), SCREEN_XRES, 0, SCREEN_XRES, SCREEN_YRES);
    SetDefDrawEnv(&(db->draw),           0, 0, SCREEN_XRES, SCREEN_YRES);
    setRGB0(&(db->draw), BGCOLOR_R, BGCOLOR_G, BGCOLOR_B);
    db->draw.isbg = 1;
    db->draw.dtd  = 1;

    PutDrawEnv(&(db->draw));

    FntLoad(960, 0);
    FntOpen(8, 12, 304, 220, 0, 512);
}

static void display(RenderContext *ctx) {
    Framebuffer *db;

    DrawSync(0);
    VSync(0);
    ctx->db_active ^= 1;

    db = &(ctx->db[ctx->db_active]);
    PutDrawEnv(&(db->draw));
    PutDispEnv(&(db->disp));
    SetDispMask(1);
}

/* Standard mono VAG header, same structure used by PSn00bSDK's example. */
typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t interleave;
    uint32_t size;
    uint32_t sample_rate;
    uint16_t _reserved[5];
    uint16_t channels;
    char name[16];
} VAG_Header;

/* The SDK reserves the first 4 KB of SPU RAM and places its dummy sample at
 * 0x1000, so user samples start just after it. */
#define ALLOC_START_ADDR 0x1010
#define SYNTH_CHANNEL 0

static int next_sample_addr = ALLOC_START_ADDR;

static int upload_sample(const void *data, int size) {
    int addr = next_sample_addr;
    size = (size + 63) & ~63;

    SpuSetTransferMode(SPU_TRANSFER_BY_DMA);
    SpuSetTransferStartAddr(addr);
    SpuWrite((const uint32_t *)data, size);
    SpuIsTransferCompleted(SPU_TRANSFER_WAIT);

    next_sample_addr = addr + size;
    return addr;
}

/* Q12 pitch multipliers. TONE 09 is the original sample pitch; each step is
 * approximately one semitone, covering -9..+6 semitones. */
static const uint16_t tone_q12[16] = {
    2435, 2580, 2734, 2896,
    3069, 3251, 3444, 3649,
    3866, 4096, 4340, 4598,
    4871, 5161, 5468, 5793
};

static void stop_voice(void) {
    SpuSetKey(0, 1 << SYNTH_CHANNEL);
}

static void play_voice(int addr, int sample_rate, int volume, int tone) {
    int effective_rate;
    int spu_volume;

    stop_voice();

    effective_rate = (sample_rate * tone_q12[tone]) >> 12;
    spu_volume = (volume * 0x3fff) / 15;

    SPU_CH_FREQ(SYNTH_CHANNEL) = getSPUSampleRate(effective_rate);
    SPU_CH_ADDR(SYNTH_CHANNEL) = getSPUAddr(addr);
    SPU_CH_VOL_L(SYNTH_CHANNEL) = spu_volume;
    SPU_CH_VOL_R(SYNTH_CHANNEL) = spu_volume;

    /* Keep the envelope effectively open for this first playback test, exactly
     * like the SDK sample. Proper NIGHT DRIVE ADSR mapping comes next. */
    SPU_CH_ADSR1(SYNTH_CHANNEL) = 0x00ff;
    SPU_CH_ADSR2(SYNTH_CHANNEL) = 0x0000;

    SpuSetKey(1, 1 << SYNTH_CHANNEL);
}

static int just_pressed(uint16_t last, uint16_t now, uint16_t mask) {
    return (last & mask) && !(now & mask);
}

int main(int argc, const char **argv) {
    init_context(&ctx);
    SpuInit();

    const VAG_Header *vag = (const VAG_Header *)night_sample;
    int sample_addr = upload_sample(vag + 1, __builtin_bswap32(vag->size));
    int sample_rate = __builtin_bswap32(vag->sample_rate);

    uint8_t pad_buff[2][34];
    InitPAD(pad_buff[0], 34, pad_buff[1], 34);
    StartPAD();
    ChangeClearPAD(0);

    uint16_t last_buttons = 0xffff;

    int drive   = 0;
    int volume  = 12;
    int attack  = 5;
    int decay   = 10;
    int sustain = 13;
    int release = 8;
    int rate    = 6;
    int tone    = 9;

    while (1) {
        FntPrint(-1, "PRISMA-PI / PS1     V2-SOUND\n");
        FntPrint(-1, "------------------------------\n");
        FntPrint(-1, "PATCH: 01 NIGHT DRIVE\n\n");
        FntPrint(-1, "DRIVE   %02d\n", drive);
        FntPrint(-1, "VOLUME  %02d   UP/DOWN  [LIVE]\n", volume);
        FntPrint(-1, "ATTACK  %02d\n", attack);
        FntPrint(-1, "DECAY   %02d\n", decay);
        FntPrint(-1, "SUSTAIN %02d\n", sustain);
        FntPrint(-1, "RELEASE %02d\n", release);
        FntPrint(-1, "RATE    %02d\n", rate);
        FntPrint(-1, "TONE    %02d   LEFT/RIGHT [LIVE]\n\n", tone);
        FntPrint(-1, "X: PLAY / TRIGGER   O: STOP\n");
        FntPrint(-1, "SPU: 1 VOICE ACTIVE / 24 AVAILABLE\n");
        FntPrint(-1, "AUDIO BASE: PSn00bSDK VAGSAMPLE\n");

        FntFlush(-1);
        display(&ctx);

        PADTYPE *pad = (PADTYPE *)pad_buff[0];
        if (pad->stat)
            continue;
        if (
            (pad->type != PAD_ID_DIGITAL) &&
            (pad->type != PAD_ID_ANALOG_STICK) &&
            (pad->type != PAD_ID_ANALOG)
        )
            continue;

        uint16_t now = pad->btn;

        if (just_pressed(last_buttons, now, PAD_UP) && volume < 15)
            volume++;
        if (just_pressed(last_buttons, now, PAD_DOWN) && volume > 0)
            volume--;
        if (just_pressed(last_buttons, now, PAD_RIGHT) && tone < 15)
            tone++;
        if (just_pressed(last_buttons, now, PAD_LEFT) && tone > 0)
            tone--;

        if (just_pressed(last_buttons, now, PAD_CROSS))
            play_voice(sample_addr, sample_rate, volume, tone);
        if (just_pressed(last_buttons, now, PAD_CIRCLE))
            stop_voice();

        last_buttons = now;
    }

    return 0;
}
