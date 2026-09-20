#ifndef PINS_H
#define PINS_H
// ═══════════════════════════════════════════════════════════
//  ESP32-C2 "Pult" — ST7789V 240x240, Hardware-SPI
//  CS liegt fest auf GND, kein MISO, Backlight invertiert.
// ═══════════════════════════════════════════════════════════

// ── Display-Auflösung ─────────────────────────────────────
#define SCREEN_W  240
#define SCREEN_H  240

// ── SPI ───────────────────────────────────────────────────
#define TFT_SCLK  4    // GPIO04 – Serial Clock
#define TFT_MOSI  6    // GPIO06 – MOSI
// kein MISO am C2-Pult

// ── ST7789 Steuerleitungen ────────────────────────────────
#define TFT_CS   -1    // GND hardwired → kein CS-Pin
#define TFT_DC    5    // GPIO05 – Data/Command
#define TFT_RST   1    // GPIO01 – Reset

// ── Backlight (invertierter Transistor: LOW = AN), PWM via ledc ──
#define TFT_BL   18    // GPIO18

#endif // PINS_H
