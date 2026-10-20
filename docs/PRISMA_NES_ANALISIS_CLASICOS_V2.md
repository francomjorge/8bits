# PRISMA / NES — Análisis de arquitecturas clásicas (V2)

## Objetivo

Extraer patrones de ingeniería de juegos clásicos de NES/6502 para construir PRISMA como instrumento audiovisual estable, rápido de iterar y con una interfaz gráfica de apariencia física.

El foco no es copiar lógica de juego, sino reutilizar conceptos de:
- input robusto
- eventos press / held / repeat
- sincronización por frame
- NMI / vblank
- buffers PPU
- OAM y composición de sprites
- motores de audio por requests
- envelopes, timers y secuencias
- state machines
- interfaces/HUD
- animación
- reducción de flicker

## Hallazgos por juego

### Super Mario Bros.
Fuente: https://github.com/RetrolariumLab/smb1_src

Patrón central: NMI/vblank como scheduler estable del frame. Preparar cambios en RAM y transferirlos al PPU en ventana segura. Para PRISMA: `VRAM_QUEUE`, `PALETTE_DIRTY`, `UI_DIRTY`, `OAM_SHADOW`, `FRAME_READY`.

### Contra
Fuente: https://github.com/vermiceli/nes-contra-us

Separa estado actual del joystick, diferencia/nueva pulsación y lectura válida. Para PRISMA: `PAD_HELD`, `PAD_PRESS`, `PAD_VALID`. Su motor de audio usa slots/prioridades sobre Pulse/Triangle/Noise/DPCM. También resulta útil su manejo de OAM para repartir mejor el flicker.

### Mega Man 3
Fuente: https://github.com/plasticsmoke/megaman3-disassembly-ca65

Muy útil para PRISMA: `joy_press`, `joy_held`, `palette_dirty`, `nametable_dirty`, `column_dirty`, OAM shadow en `$0200`, shadow de paleta y buffer de updates PPU. El sonido se mantiene como subsystem independiente.

### Dr. Mario
Fuente: https://github.com/Nostaljipi/dr-mario-disassembly

Lectura robusta del joystick mediante lecturas repetidas/validadas antes de calcular pressed/held. Arquitectura de audio separada para música, SFX y DMC.

### The Legend of Zelda
Fuente: https://github.com/aldonunez/zelda1-disassembly

Mantiene `ButtonsPressed`, `ButtonsDown`, `HeldButton`, `StillHoldingButton`, `SubsequentButtonRepeat` y `ButtonRepeatTimer`. Muy útil para construir navegación de parámetros con PRESS / HELD / REPEAT. El audio usa requests que el driver consume cada frame.

### Adventure Island / Master Higgins
Fuente: https://github.com/cyneprepou4uk/NES-Games-Disassembly

Variables separadas para botones A/B, `animation_id`, timer de animación, flags NMI, palette buffer y PPU update. Para PRISMA conviene separar `VISUAL_MODE`, `VISUAL_FRAME`, `VISUAL_TIMER`, `VISUAL_RATE`.

### Metroid
Fuente: https://github.com/nmikstas/metroid-disassembly

Ocho bancos de 16 KiB, con Bank07 como engine permanente y Bank06 para gráficos. Aunque PRISMA arranque en Mapper 0, conviene organizarlo desde ahora como `CORE / PATCH DATA / VISUAL DATA / SOUND DATA`.

### Prince of Persia — Apple II 6502
Fuente: https://github.com/jmechner/Prince-of-Persia-Apple-II

Distingue raw input de fresh/smart press y después interpreta el control según contexto/estado. Esto sugiere un `CONTROL_CONTEXT` para reutilizar botones sin volver caótico el input.

## Diagnóstico PRISMA V1–V6

1. PPU: las ROM tempranas hacen demasiado trabajo variable alrededor de vblank.
2. Sprites: las versiones recientes pueden chocar con el límite real de 8 sprites por scanline.
3. Input: edge detection simple no alcanza; necesitamos safe-read + held + press + release + repeat.
4. UI/rendering: lógica, audio, PPU writes y OAM deben desacoplarse.

## PRISMA CORE ENGINE V7

```text
            NMI / VBLANK
                 │
       ┌─────────▼──────────┐
       │ OAM DMA            │
       │ VRAM QUEUE         │
       │ PALETTE DIRTY      │
       │ RESTORE SCROLL     │
       │ FRAME READY        │
       └─────────┬──────────┘
                 │
                 ▼
              MAIN
                 │
       ┌─────────▼──────────┐
       │ INPUT ENGINE       │
       ├────────────────────┤
       │ PATCH ENGINE       │
       ├────────────────────┤
       │ SEQUENCER          │
       ├────────────────────┤
       │ AUDIO ENGINE       │
       ├────────────────────┤
       │ VISUAL ENGINE      │
       ├────────────────────┤
       │ UI DIRTY QUEUE     │
       └────────────────────┘
```

### INPUT ENGINE V7

```text
PAD_RAW
PAD_PREV
PAD_PRESS
PAD_RELEASE
PAD_HELD
PAD_REPEAT
```

Lectura recomendada: dos lecturas y validación. Repeat inicial ~12 frames, repeat posterior ~3 frames.

### UI ENGINE

La imagen del instrumento debe convertirse principalmente en background tiles. Sprites sólo para cursor, LED, highlight, visual dinámico e indicadores. Paneles, labels, marcos y botones estáticos deben ir al nametable y actualizarse con VRAM queue.

### PATCH ENGINE

Cada módulo:

```text
SOURCE
ROOT
OCTAVE
MODE
RATE
LOOP
STEP
VOLUME
ENVELOPE
MODULATION
PATTERN
VISUAL_MODE
VISUAL_AMOUNT
```

Cada parámetro debe tener `VALUE / MIN / MAX / CONTROL / DIRTY`.

### AUDIO ENGINE

```text
PATCH
  ↓
SEQUENCER
  ↓
CHANNEL ARBITER
  ↓
APU RENDER
```

El render del APU debe escribir sólo registros que cambiaron.

### VISUAL ENGINE

Mapeo inicial recomendado:

```text
ROOT       -> radio / posición base
OCTAVE     -> escala espacial
MODE       -> familia de glyphs / forma
RATE       -> velocidad de animación
LOOP       -> active/dim
STEP       -> glyph activo
VOLUME     -> densidad/brillo
ENVELOPE   -> expansión/contracción
MODULATION -> oscilación
SOURCE     -> lenguaje visual
```

## Siguiente implementación

PRISMA V7 debe priorizar: NMI real, controller safe-read, PRESS/HELD/RELEASE/REPEAT, OAM shadow, VRAM queue, dirty flags, UI estática en nametable, control del número de sprites por scanline, audio request queue y patch table data-driven.
