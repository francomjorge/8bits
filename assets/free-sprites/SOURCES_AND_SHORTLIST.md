# PRISMA — Free sprite sources and first shortlist

Objetivo: construir una biblioteca visual legalmente simple de reutilizar, modificar, recolorear y convertir a tiles CHR de NES. Prioridad: **CC0 / dominio público**, assets 8×8 o 16×16 y paletas reducidas.

## Fuentes verificadas

### 1. Nat's 8x8 UI Pack
- Página: https://opengameart.org/content/nats-8x8-ui-pack-over-50-ui-elements
- Licencia: **CC0**
- Tamaño: 8×8
- Más de 50 elementos UI.
- Usa una paleta de 4 colores.
- Incluye tilemap PNG y PNG separados.
- Uso PRISMA: cursores, indicadores, controles, LEDs, pequeños símbolos y estados.

### 2. Nat's 8x8 Starter Pack
- Página: https://opengameart.org/content/nats-8x8-starter-pack
- Licencia: **CC0**
- Tamaño: 8×8
- Más de 480 sprites.
- Incluye animales, scenery y tiles de grass/sand/stone/house/water.
- Uso PRISMA: partículas figurativas, iconos animados, fondos y elementos visuales generativos.

### 3. RogueDB32
- Página: https://opengameart.org/content/roguedb32
- Licencia: **CC0**
- La mayoría de los tiles son 8×8.
- Incluye GUI, tipografías, objetos y tiles roguelike.
- Uso PRISMA: marcos, esquinas, glyphs, indicadores, iconografía y paneles.

### 4. Deep Night — 8x8 Platformer Assets
- Página: https://v3x3d.itch.io/deep-night
- Licencia: **CC0 1.0 Universal**
- 400+ tiles únicos.
- Player animado, enemigos, puertas, señales, llaves, monedas, cuerdas, vines, etc.
- Uso PRISMA: personaje central, ciclos de animación, objetos reactivos y símbolos figurativos.

### 5. Kenney Pixel UI Pack
- Página: https://kenney.nl/assets/pixel-ui-pack
- Licencia: **CC0**
- 750 assets.
- Panels, buttons, cursors, pointers, bars, checkmarks y scrollers.
- Uso PRISMA: interfaz física del instrumento y controles visuales.

### 6. Kenney Cursor Pixel Pack
- Página: https://kenney.nl/assets/cursor-pixel-pack
- Licencia: **CC0**
- 180 assets.
- Tamaño: 16×16.
- Uso PRISMA: cursor activo, selección de parámetro, apuntadores y feedback de input.

### 7. Kenney Pixel Platformer
- Página: https://kenney.nl/assets/pixel-platformer
- Licencia: **CC0**
- 200 assets.
- Tamaño nativo: 18×18.
- Uso PRISMA: objetos y pequeñas figuras; requiere reducción/regrillado a 16×16 o 24×24.

### 8. Kenney Pattern Pack Pixel
- Página: https://kenney.nl/assets/pattern-pack-pixel
- Licencia: **CC0**
- 60 patterns de 16×16.
- Uso PRISMA: texturas, fondos dinámicos, modulaciones, dithering y visuales repetitivos.

---

# Shortlist inicial — 30 elementos para convertir a NES

Los nombres de abajo son **roles concretos para la biblioteca PRISMA**. Cuando importemos los packs, cada rol se vinculará al archivo exacto elegido y a su fuente.

| ID | Sprite / rol | Fuente prioritaria | Conversión NES | Uso |
|---|---|---|---|---|
| S01 | Cursor flecha | Nat UI / Kenney Cursor | 8×8 | parámetro seleccionado |
| S02 | Cursor mano | Kenney Cursor | 16×16→2×2 tiles | selección directa |
| S03 | Check / activo | Nat UI | 8×8 | LOOP ON |
| S04 | Cross / apagado | Nat UI | 8×8 | LOOP OFF |
| S05 | LED circular | Nat UI / creación propia derivada | 8×8 | trigger |
| S06 | Barra vertical | Kenney UI | 8×8 | volumen / envelope |
| S07 | Flecha izquierda | Nat UI | 8×8 | decremento |
| S08 | Flecha derecha | Nat UI | 8×8 | incremento |
| S09 | Flecha arriba | Nat UI | 8×8 | octave + |
| S10 | Flecha abajo | Nat UI | 8×8 | octave - |
| S11 | Marco esquina | RogueDB32 | 8×8 | panel modular |
| S12 | Separador / notch | RogueDB32 | 8×8 | estructura UI |
| S13 | Glyph estrella | RogueDB32 / Nat Starter | 8×8 | trigger visual |
| S14 | Glyph cruz | RogueDB32 | 8×8 | MODE visual |
| S15 | Glyph ojo | RogueDB32 / adaptación | 8×8 | source/monitor |
| S16 | Orbe / partícula | Nat Starter | 8×8 | partículas |
| S17 | Moneda / disco | Deep Night | 8×8 | secuenciador circular |
| S18 | Llave | Deep Night | 8×8 | lock/sync |
| S19 | Señal | Deep Night | 8×8 | marker/contexto |
| S20 | Puerta | Deep Night | 8×8/16×16 | cambio de sección |
| S21 | Cuerda / línea | Deep Night | 8×8 repetible | waveform/vertical chain |
| S22 | Vine / curva | Deep Night | 8×8 repetible | modulación |
| S23 | Player idle | Deep Night | metasprite | figura central |
| S24 | Player run 1 | Deep Night | metasprite | animación musical |
| S25 | Player run 2 | Deep Night | metasprite | animación musical |
| S26 | Player jump | Deep Night | metasprite | note trigger |
| S27 | Enemy simple | Deep Night | 8×8/16×16 | visual generado |
| S28 | Tile agua | Nat Starter | 8×8 | LFO / wave |
| S29 | Tile stone | Nat Starter | 8×8 | textura base |
| S30 | Pattern geométrico | Kenney Pattern Pixel | 16×16→4 tiles | fondo reactivo |

## Orden de importación

**Fase A — inmediatamente útiles:** S01–S16. Son UI/glyphs y deberían poder entrar en una sola página CHR con muy poca limpieza.

**Fase B — visuales dinámicos:** S17–S22 y S28–S30.

**Fase C — metasprites:** S23–S27, que requieren extracción de frames y reducción a paleta NES.

## Reglas de adaptación a NES

1. Priorizar 8×8 nativo.
2. Cada sprite hardware: transparencia + máximo 3 colores visibles.
3. Si un asset necesita 4+ colores visibles, dividir en layers sólo cuando valga la pena.
4. Evitar más de 8 sprites sobre la misma scanline.
5. Paneles, bordes y botones estáticos deben convertirse en background tiles, no OAM.
6. Mantener original + versión `nes-ready` separadas.
7. Registrar siempre fuente y licencia aunque sea CC0.

## Estructura prevista

```text
assets/free-sprites/
  SOURCES_AND_SHORTLIST.md
  originals/
    nats-ui/
    nats-starter/
    roguedb32/
    deep-night/
    kenney/
  nes-ready/
    ui/
    glyphs/
    particles/
    characters/
    patterns/
```
