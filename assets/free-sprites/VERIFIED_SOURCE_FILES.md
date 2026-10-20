# PRISMA — Verified source files (no invented assets)

Regla de trabajo: **no inventar sprites ni atribuirles origen si no fue verificado**. Para los assets externos, conservar siempre la página fuente, licencia, autor y archivo original. La conversión NES se hace a partir del asset real, no de una reconstrucción aproximada.

## OpenGameArt — 8x8 UI/Animals/Items

Autor: Min  
Licencia: CC0  
Página: https://opengameart.org/content/8x8-uianimalsitems

Archivos publicados por la página:

- `8x8Uithings.png` — UI: health, shield, 3 cursors, pause, mute, big/small arrows  
  Archivo original: https://opengameart.org/sites/default/files/8x8Uithings_0.png
- `8x8Items.png` — food, coin frames, health/shield/sword/gun/ammo icons  
  Archivo original: https://opengameart.org/sites/default/files/8x8Items_0.png
- `8x8animals.png` — pig/cow/sheep con animaciones  
  Archivo original: https://opengameart.org/sites/default/files/8x8animals_0.png

Uso PRISMA prioritario:
- cursores reales del pack
- pause / mute
- flechas grandes y pequeñas
- animación de moneda 3×8x8
- iconos 4x4 y 8x8

## OpenGameArt — 8x8 Icons and Symbols

Autor: DezrasDragons  
Licencia: CC0  
Página: https://opengameart.org/content/8x8-icons-and-symbols

La página especifica sprites 8x8, 4 colores / 2-bit, organizados en grilla 8x8 con borde de 1 píxel.

Archivos publicados:

- `equipment_8x8.png`  
  Original: https://opengameart.org/sites/default/files/equipment_8x8.png
- `symbols_8x8.png`  
  Original: https://opengameart.org/sites/default/files/symbols_8x8.png

Uso PRISMA prioritario:
- símbolos matemáticos
- símbolos alquímicos
- símbolos astrológicos
- iconos de equipamiento cuando sean útiles como glyphs

No renombrar un símbolo sin confirmar visualmente cuál celda exacta se extrajo.

## OpenGameArt — Nat's 8x8 UI Pack

Autor: nateonus  
Licencia: CC0  
Página: https://opengameart.org/content/nats-8x8-ui-pack-over-50-ui-elements

La página confirma:
- 8x8
- paleta de 4 colores
- PNG tilemap sin bordes
- PNGs separados
- Readme

Archivo publicado:
- `Nat's8x8UIPack.zip`  
  Original: https://opengameart.org/sites/default/files/Nat%27s8x8UIPack.zip

Uso PRISMA: UI, cursores, indicadores, botones y elementos de estado, pero sólo después de importar el ZIP real y registrar los nombres reales de sus archivos.

## Deep Night — VEXED

Autor: VEXED  
Licencia: CC0 1.0 Universal  
Página: https://v3x3d.itch.io/deep-night

La página confirma:
- 400+ tiles únicos de 8x8
- player animado
- enemigos animados
- puertas, señales, llaves, monedas, cuerdas, vines y tiles repetibles
- animaciones de player: correr, saltar, empujar (según devlog del autor)

Archivo publicado:
- `Deep-Night.zip` (16 kB), desde la página de itch.io.

Uso PRISMA: personaje central y visuales figurativos. No recrear estos sprites de memoria: importar el ZIP original primero.

## Kenney — Pixel UI Pack

Autor: Kenney  
Licencia: CC0  
Página: https://kenney.nl/assets/pixel-ui-pack

La página confirma 750 archivos/assets de UI pixel.

Uso PRISMA: paneles, botones, barras, cursores y elementos de interfaz. Seleccionar del archivo original descargado; no reconstruir un elemento basándonos sólo en el preview.

---

## Política de conversión NES

Para cada asset importado:

1. conservar `originals/...` sin modificar;
2. crear una entrada de manifest con `source_page`, `author`, `license`, `original_filename`;
3. extraer la celda/frame exacto;
4. registrar coordenadas de extracción si viene de spritesheet;
5. convertir a 8x8 / 16x16 sin redibujarlo salvo limpieza técnica explícitamente documentada;
6. reducir a transparencia + máximo 3 colores visibles por sprite hardware;
7. guardar PNG de referencia + CHR 2bpp;
8. si una adaptación cambia el arte, marcarla como `adapted`, nunca como `original`.

## Estado actual

Los assets externos anteriores están **verificados por fuente/licencia**, pero no deben considerarse importados en `originals/` hasta que el archivo binario real esté dentro del repositorio.

El pack `nes-ready/ui/v1` generado anteriormente por PRISMA es material propio de prueba y **no debe presentarse como proveniente de esos packs externos**.
