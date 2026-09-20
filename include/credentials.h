#ifndef CREDENTIALS_H
#define CREDENTIALS_H
// ═══════════════════════════════════════════════════════════
//  FALLBACK-Credentials — NUR reincompilierter Notnagel.
//
//  USE_FALLBACK_CREDENTIALS 0 (default):
//     Gerät ignoriert die Werte hier und startet ohne gültige
//     Config im SETUP-AP-Modus (für Verschenken/Weitergeben).
//
//  USE_FALLBACK_CREDENTIALS 1:
//     Jungfräuliches Gerät zieht diese Werte einmalig als
//     Startkonfiguration (bequem fürs eigene Gerät).
// ═══════════════════════════════════════════════════════════
#define USE_FALLBACK_CREDENTIALS 0

#define FALLBACK_WIFI_SSID   ""
#define FALLBACK_WIFI_PASS   ""

#define FALLBACK_OW_KEY      ""
#define FALLBACK_OW_CITY     ""
#define FALLBACK_OW_COUNTRY  "DE"
#define FALLBACK_OW_UNITS    "metric"
#define FALLBACK_OW_LANG     "de"

#endif
