# PRISMA NES UI PACK V1

Primer pack **NES-ready** para PRISMA.

Contenido:
- `prisma_ui_8x8_sheet.png`: 16 tiles 8x8 en una sola tira.
- `prisma_ui_8x8.chr`: los mismos 16 tiles codificados en formato CHR 2bpp de NES (256 bytes).
- `manifest.json`: índice y coordenadas de cada tile.
- `prisma_ui_8x8_contact_sheet.png`: preview ampliado para inspección visual.

## Tiles

0 cursor_left
1 cursor_right
2 cursor_up
3 cursor_down
4 check
5 cross
6 led
7 bar
8 star
9 eye
10 orb
11 pulse
12 triangle
13 noise
14 corner
15 knob

## Paleta lógica

- 0 = transparente
- 1 = cyan
- 2 = magenta
- 3 = amarillo

En NES los valores 1/2/3 son índices lógicos de paleta y pueden remapearse a cualquier subpaleta real.

## Origen

Estos 16 glyphs son originales de PRISMA y se generaron específicamente para el repositorio `8bits`, para tener una base legalmente limpia y técnicamente exacta mientras se importan y adaptan assets CC0 externos.

## Fuentes CC0 externas verificadas para próximas importaciones

- Nat's 8x8 UI Pack — CC0 — https://opengameart.org/content/nats-8x8-ui-pack-over-50-ui-elements
- 8x8 Icons and Symbols — CC0 — https://opengameart.org/content/8x8-icons-and-symbols
- 8x8 UI/Animals/Items — CC0 — https://opengameart.org/content/8x8-uianimalsitems
- Kenney Pixel UI Pack — CC0 — https://kenney.nl/assets/pixel-ui-pack
- Deep Night 8x8 Platformer Assets — CC0 — https://v3x3d.itch.io/deep-night

## Próximo paso

Importar/seleccionar assets externos, registrar archivo exacto + licencia + fuente y generar una versión `nes-ready` reducida a 8x8/16x16 y máximo 3 colores visibles + transparencia.
