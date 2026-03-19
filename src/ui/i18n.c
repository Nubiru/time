#include "i18n.h"

#include <string.h>

/* ---- Static translation table (English defaults) ---- */

static const i18n_entry_t default_strings[] = {
    /* System names (16) */
    { "system.gregorian",    "Gregorian" },
    { "system.hebrew",       "Hebrew" },
    { "system.islamic",      "Islamic" },
    { "system.chinese",      "Chinese" },
    { "system.hindu",        "Hindu" },
    { "system.buddhist",     "Buddhist" },
    { "system.mayan",        "Mayan" },
    { "system.coptic",       "Coptic" },
    { "system.ethiopian",    "Ethiopian" },
    { "system.zoroastrian",  "Zoroastrian" },
    { "system.celtic",       "Celtic" },
    { "system.myanmar",      "Myanmar" },
    { "system.egyptian",     "Egyptian" },
    { "system.astrology",    "Astrology" },
    { "system.iching",       "I Ching" },
    { "system.human_design", "Human Design" },

    /* UI labels (14) */
    { "ui.zoom_level",    "Zoom level" },
    { "ui.view_space",    "Space View" },
    { "ui.view_earth",    "Earth View" },
    { "ui.festival_alert","Festival alert" },
    { "ui.retrograde",    "retrograde" },
    { "ui.direct",        "direct" },
    { "ui.day",           "day" },
    { "ui.days",          "days" },
    { "ui.month",         "month" },
    { "ui.months",        "months" },
    { "ui.year",          "year" },
    { "ui.years",         "years" },
    { "ui.loading",       "Loading..." },
    { "ui.error",         "Error" },

    /* Additional system names (4) */
    { "system.kabbalah",    "Kabbalah" },
    { "system.geology",     "Geology" },
    { "system.earth",       "Earth" },
    { "system.unified",     "Unified" },

    /* Depth tier labels (4) */
    { "depth.surface",      "Surface" },
    { "depth.context",      "Context" },
    { "depth.cycle",        "Cycle" },
    { "depth.cosmic",       "Cosmic" },

    /* View names (6) */
    { "view.space",         "Space View" },
    { "view.earth",         "Earth View" },
    { "view.tzolkin",       "Tzolkin View" },
    { "view.iching",        "I Ching View" },
    { "view.calendar",      "Calendar View" },
    { "view.deep_time",     "Deep Time View" },

    /* Time controls (8) */
    { "time.paused",        "Paused" },
    { "time.reversed",      "Reversed" },
    { "time.speed",         "Speed" },
    { "time.now",           "Now" },
    { "time.realtime",      "Real-time" },
    { "time.faster",        "Faster" },
    { "time.slower",        "Slower" },
    { "time.jump_to",       "Jump to" },

    /* Layer categories (6) */
    { "layer.astronomy",    "Astronomy" },
    { "layer.astrology",    "Astrology" },
    { "layer.calendar",     "Calendars" },
    { "layer.sacred",       "Sacred Systems" },
    { "layer.earth",        "Earth" },
    { "layer.display",      "Display" },

    /* Content labels (8) */
    { "content.wisdom",     "Wisdom" },
    { "content.fun_fact",   "Fun Fact" },
    { "content.story",      "Cultural Story" },
    { "content.achievement","Achievement" },
    { "content.drama",      "Dramatization" },
    { "content.card",       "Card" },
    { "content.help",       "Help" },
    { "content.command",    "Command" },

    /* Sensitivity labels (3) */
    { "sensitivity.general",    "General" },
    { "sensitivity.respectful", "Respectful" },
    { "sensitivity.sacred",     "Sacred" },

    /* Navigation (6) */
    { "nav.home",           "Home" },
    { "nav.settings",       "Settings" },
    { "nav.about",          "About" },
    { "nav.share",          "Share" },
    { "nav.search",         "Search" },
    { "nav.close",          "Close" },

    /* Calendar terms (6) */
    { "cal.month",          "Month" },
    { "cal.day",            "Day" },
    { "cal.cycle",          "Cycle" },
    { "cal.element",        "Element" },
    { "cal.sign",           "Sign" },
    { "cal.festival",       "Festival" },

    /* Toast messages (6) */
    { "toast.speed_changed",    "Speed changed" },
    { "toast.view_changed",     "View changed" },
    { "toast.layer_toggled",    "Layer toggled" },
    { "toast.jump_complete",    "Jump complete" },
    { "toast.location_set",     "Location set" },
    { "toast.time_paused",      "Time paused" },

    /* Accessibility (4) */
    { "a11y.skip_to_content",  "Skip to content" },
    { "a11y.screen_reader",    "Screen reader mode" },
    { "a11y.high_contrast",    "High contrast" },
    { "a11y.reduced_motion",   "Reduced motion" },

    /* Welcome journey (6) */
    { "welcome.when_born",     "When did you arrive on this planet?" },
    { "welcome.where_born",    "Where on Earth were you standing?" },
    { "welcome.skip",          "Skip" },
    { "welcome.continue",      "Continue" },
    { "welcome.reveal",        "Discover your cosmic identity" },
    { "welcome.complete",      "Welcome to Time" },

    /* Error states (4) */
    { "error.no_data",         "No data available" },
    { "error.invalid_date",    "Invalid date" },
    { "error.out_of_range",    "Date out of range" },
    { "error.unknown",         "Unknown error" },

    /* Attribution format (2) */
    { "attr.source",           "Source" },
    { "attr.author",           "Author" },

    /* Planets (8) */
    { "planet.sun",       "Sun" },
    { "planet.moon",      "Moon" },
    { "planet.mercury",   "Mercury" },
    { "planet.venus",     "Venus" },
    { "planet.mars",      "Mars" },
    { "planet.jupiter",   "Jupiter" },
    { "planet.saturn",    "Saturn" },
    { "planet.neptune",   "Neptune" }
};

#define DEFAULT_STRING_COUNT \
    ((int)(sizeof(default_strings) / sizeof(default_strings[0])))

/* ============================================================
 * P0 LOCALE TRANSLATIONS — Spanish, Arabic, Chinese
 * Same key ordering as default_strings for parallel lookup.
 * ============================================================ */

/* ---- Spanish (ES) ---- */

static const i18n_entry_t es_strings[] = {
    /* System names */
    { "system.gregorian",    "Gregoriano" },
    { "system.hebrew",       "Hebreo" },
    { "system.islamic",      "Isl\xc3\xa1mico" },
    { "system.chinese",      "Chino" },
    { "system.hindu",        "Hind\xc3\xba" },
    { "system.buddhist",     "Budista" },
    { "system.mayan",        "Maya" },
    { "system.coptic",       "Copto" },
    { "system.ethiopian",    "Et\xc3\xadope" },
    { "system.zoroastrian",  "Zoroastriano" },
    { "system.celtic",       "Celta" },
    { "system.myanmar",      "Myanmar" },
    { "system.egyptian",     "Egipcio" },
    { "system.astrology",    "Astrolog\xc3\xad" "a" },
    { "system.iching",       "I Ching" },
    { "system.human_design", "Dise\xc3\xb1o Humano" },
    /* UI labels */
    { "ui.zoom_level",    "Nivel de zoom" },
    { "ui.view_space",    "Vista Espacial" },
    { "ui.view_earth",    "Vista Terrestre" },
    { "ui.festival_alert","Alerta de festival" },
    { "ui.retrograde",    "retr\xc3\xb3grado" },
    { "ui.direct",        "directo" },
    { "ui.day",           "d\xc3\xad" "a" },
    { "ui.days",          "d\xc3\xad" "as" },
    { "ui.month",         "mes" },
    { "ui.months",        "meses" },
    { "ui.year",          "a\xc3\xb1o" },
    { "ui.years",         "a\xc3\xb1os" },
    { "ui.loading",       "Cargando..." },
    { "ui.error",         "Error" },
    /* Additional system names */
    { "system.kabbalah",    "C\xc3\xa1" "bala" },
    { "system.geology",     "Geolog\xc3\xad" "a" },
    { "system.earth",       "Tierra" },
    { "system.unified",     "Unificado" },
    /* Depth */
    { "depth.surface",      "Superficie" },
    { "depth.context",      "Contexto" },
    { "depth.cycle",        "Ciclo" },
    { "depth.cosmic",       "C\xc3\xb3smico" },
    /* Views */
    { "view.space",         "Vista Espacial" },
    { "view.earth",         "Vista Terrestre" },
    { "view.tzolkin",       "Vista Tzolkin" },
    { "view.iching",        "Vista I Ching" },
    { "view.calendar",      "Vista Calendario" },
    { "view.deep_time",     "Vista Tiempo Profundo" },
    /* Time */
    { "time.paused",        "Pausado" },
    { "time.reversed",      "Invertido" },
    { "time.speed",         "Velocidad" },
    { "time.now",           "Ahora" },
    { "time.realtime",      "Tiempo real" },
    { "time.faster",        "M\xc3\xa1s r\xc3\xa1pido" },
    { "time.slower",        "M\xc3\xa1s lento" },
    { "time.jump_to",       "Saltar a" },
    /* Layers */
    { "layer.astronomy",    "Astronom\xc3\xad" "a" },
    { "layer.astrology",    "Astrolog\xc3\xad" "a" },
    { "layer.calendar",     "Calendarios" },
    { "layer.sacred",       "Sistemas Sagrados" },
    { "layer.earth",        "Tierra" },
    { "layer.display",      "Pantalla" },
    /* Content */
    { "content.wisdom",     "Sabidur\xc3\xad" "a" },
    { "content.fun_fact",   "Dato Curioso" },
    { "content.story",      "Historia Cultural" },
    { "content.achievement","Logro" },
    { "content.drama",      "Dramatizaci\xc3\xb3n" },
    { "content.card",       "Carta" },
    { "content.help",       "Ayuda" },
    { "content.command",    "Comando" },
    /* Sensitivity */
    { "sensitivity.general",    "General" },
    { "sensitivity.respectful", "Respetuoso" },
    { "sensitivity.sacred",     "Sagrado" },
    /* Navigation */
    { "nav.home",           "Inicio" },
    { "nav.settings",       "Configuraci\xc3\xb3n" },
    { "nav.about",          "Acerca de" },
    { "nav.share",          "Compartir" },
    { "nav.search",         "Buscar" },
    { "nav.close",          "Cerrar" },
    /* Calendar */
    { "cal.month",          "Mes" },
    { "cal.day",            "D\xc3\xad" "a" },
    { "cal.cycle",          "Ciclo" },
    { "cal.element",        "Elemento" },
    { "cal.sign",           "Signo" },
    { "cal.festival",       "Festival" },
    /* Toast */
    { "toast.speed_changed",    "Velocidad cambiada" },
    { "toast.view_changed",     "Vista cambiada" },
    { "toast.layer_toggled",    "Capa alternada" },
    { "toast.jump_complete",    "Salto completo" },
    { "toast.location_set",     "Ubicaci\xc3\xb3n establecida" },
    { "toast.time_paused",      "Tiempo pausado" },
    /* Accessibility */
    { "a11y.skip_to_content",  "Saltar al contenido" },
    { "a11y.screen_reader",    "Modo lector de pantalla" },
    { "a11y.high_contrast",    "Alto contraste" },
    { "a11y.reduced_motion",   "Movimiento reducido" },
    /* Welcome */
    { "welcome.when_born",     "\xc2\xbf" "Cu\xc3\xa1ndo llegaste a este planeta?" },
    { "welcome.where_born",    "\xc2\xbf" "D\xc3\xb3nde en la Tierra estabas?" },
    { "welcome.skip",          "Omitir" },
    { "welcome.continue",      "Continuar" },
    { "welcome.reveal",        "Descubre tu identidad c\xc3\xb3smica" },
    { "welcome.complete",      "Bienvenido a Time" },
    /* Error */
    { "error.no_data",         "No hay datos disponibles" },
    { "error.invalid_date",    "Fecha inv\xc3\xa1lida" },
    { "error.out_of_range",    "Fecha fuera de rango" },
    { "error.unknown",         "Error desconocido" },
    /* Attribution */
    { "attr.source",           "Fuente" },
    { "attr.author",           "Autor" },
    /* Planets */
    { "planet.sun",       "Sol" },
    { "planet.moon",      "Luna" },
    { "planet.mercury",   "Mercurio" },
    { "planet.venus",     "Venus" },
    { "planet.mars",      "Marte" },
    { "planet.jupiter",   "J\xc3\xbapiter" },
    { "planet.saturn",    "Saturno" },
    { "planet.neptune",   "Neptuno" }
};

#define ES_STRING_COUNT \
    ((int)(sizeof(es_strings) / sizeof(es_strings[0])))

/* ---- Arabic (AR) ---- */

static const i18n_entry_t ar_strings[] = {
    /* System names */
    { "system.gregorian",    "\xd9\x85\xd9\x8a\xd9\x84\xd8\xa7\xd8\xaf\xd9\x8a" },
    { "system.hebrew",       "\xd8\xb9\xd8\xa8\xd8\xb1\xd9\x8a" },
    { "system.islamic",      "\xd8\xa5\xd8\xb3\xd9\x84\xd8\xa7\xd9\x85\xd9\x8a" },
    { "system.chinese",      "\xd8\xb5\xd9\x8a\xd9\x86\xd9\x8a" },
    { "system.hindu",        "\xd9\x87\xd9\x86\xd8\xaf\xd9\x88\xd8\xb3\xd9\x8a" },
    { "system.buddhist",     "\xd8\xa8\xd9\x88\xd8\xb0\xd9\x8a" },
    { "system.mayan",        "\xd9\x85\xd8\xa7\xd9\x8a\xd8\xa7" },
    { "system.coptic",       "\xd9\x82\xd8\xa8\xd8\xb7\xd9\x8a" },
    { "system.ethiopian",    "\xd8\xa5\xd8\xab\xd9\x8a\xd9\x88\xd8\xa8\xd9\x8a" },
    { "system.zoroastrian",  "\xd8\xb2\xd8\xb1\xd8\xa7\xd8\xaf\xd8\xb4\xd8\xaa\xd9\x8a" },
    { "system.celtic",       "\xd9\x83\xd9\x84\xd8\xaa\xd9\x8a" },
    { "system.myanmar",      "\xd9\x85\xd9\x8a\xd8\xa7\xd9\x86\xd9\x85\xd8\xa7\xd8\xb1" },
    { "system.egyptian",     "\xd9\x85\xd8\xb5\xd8\xb1\xd9\x8a" },
    { "system.astrology",    "\xd8\xb9\xd9\x84\xd9\x85 \xd8\xa7\xd9\x84\xd8\xaa\xd9\x86\xd8\xac\xd9\x8a\xd9\x85" },
    { "system.iching",       "\xd8\xa5\xd9\x8a \xd8\xaa\xd8\xb4\xd9\x8a\xd9\x86\xd8\xba" },
    { "system.human_design", "\xd8\xa7\xd9\x84\xd8\xaa\xd8\xb5\xd9\x85\xd9\x8a\xd9\x85 \xd8\xa7\xd9\x84\xd8\xa8\xd8\xb4\xd8\xb1\xd9\x8a" },
    /* UI labels */
    { "ui.zoom_level",    "\xd9\x85\xd8\xb3\xd8\xaa\xd9\x88\xd9\x89 \xd8\xa7\xd9\x84\xd8\xaa\xd9\x83\xd8\xa8\xd9\x8a\xd8\xb1" },
    { "ui.view_space",    "\xd8\xb9\xd8\xb1\xd8\xb6 \xd8\xa7\xd9\x84\xd9\x81\xd8\xb6\xd8\xa7\xd8\xa1" },
    { "ui.view_earth",    "\xd8\xb9\xd8\xb1\xd8\xb6 \xd8\xa7\xd9\x84\xd8\xa3\xd8\xb1\xd8\xb6" },
    { "ui.festival_alert","\xd8\xaa\xd9\x86\xd8\xa8\xd9\x8a\xd9\x87 \xd8\xb9\xd9\x8a\xd8\xaf" },
    { "ui.retrograde",    "\xd8\xaa\xd8\xb1\xd8\xa7\xd8\xac\xd8\xb9" },
    { "ui.direct",        "\xd9\x85\xd8\xa8\xd8\xa7\xd8\xb4\xd8\xb1" },
    { "ui.day",           "\xd9\x8a\xd9\x88\xd9\x85" },
    { "ui.days",          "\xd8\xa3\xd9\x8a\xd8\xa7\xd9\x85" },
    { "ui.month",         "\xd8\xb4\xd9\x87\xd8\xb1" },
    { "ui.months",        "\xd8\xa3\xd8\xb4\xd9\x87\xd8\xb1" },
    { "ui.year",          "\xd8\xb3\xd9\x86\xd8\xa9" },
    { "ui.years",         "\xd8\xb3\xd9\x86\xd9\x88\xd8\xa7\xd8\xaa" },
    { "ui.loading",       "\xd8\xac\xd8\xa7\xd8\xb1\xd9\x8d \xd8\xa7\xd9\x84\xd8\xaa\xd8\xad\xd9\x85\xd9\x8a\xd9\x84..." },
    { "ui.error",         "\xd8\xae\xd8\xb7\xd8\xa3" },
    /* Additional system names */
    { "system.kabbalah",    "\xd8\xa7\xd9\x84\xd9\x82\xd8\xa8\xd8\xa7\xd9\x84\xd8\xa9" },
    { "system.geology",     "\xd8\xa7\xd9\x84\xd8\xac\xd9\x8a\xd9\x88\xd9\x84\xd9\x88\xd8\xac\xd9\x8a\xd8\xa7" },
    { "system.earth",       "\xd8\xa7\xd9\x84\xd8\xa3\xd8\xb1\xd8\xb6" },
    { "system.unified",     "\xd8\xa7\xd9\x84\xd9\x85\xd9\x88\xd8\xad\xd9\x91\xd8\xaf" },
    /* Depth */
    { "depth.surface",      "\xd8\xa7\xd9\x84\xd8\xb3\xd8\xb7\xd8\xad" },
    { "depth.context",      "\xd8\xa7\xd9\x84\xd8\xb3\xd9\x8a\xd8\xa7\xd9\x82" },
    { "depth.cycle",        "\xd8\xa7\xd9\x84\xd8\xaf\xd9\x88\xd8\xb1\xd8\xa9" },
    { "depth.cosmic",       "\xd8\xa7\xd9\x84\xd9\x83\xd9\x88\xd9\x86\xd9\x8a" },
    /* Views */
    { "view.space",         "\xd8\xb9\xd8\xb1\xd8\xb6 \xd8\xa7\xd9\x84\xd9\x81\xd8\xb6\xd8\xa7\xd8\xa1" },
    { "view.earth",         "\xd8\xb9\xd8\xb1\xd8\xb6 \xd8\xa7\xd9\x84\xd8\xa3\xd8\xb1\xd8\xb6" },
    { "view.tzolkin",       "\xd8\xb9\xd8\xb1\xd8\xb6 \xd8\xaa\xd8\xb2\xd9\x88\xd9\x84\xd9\x83\xd9\x8a\xd9\x86" },
    { "view.iching",        "\xd8\xb9\xd8\xb1\xd8\xb6 \xd8\xa5\xd9\x8a \xd8\xaa\xd8\xb4\xd9\x8a\xd9\x86\xd8\xba" },
    { "view.calendar",      "\xd8\xb9\xd8\xb1\xd8\xb6 \xd8\xa7\xd9\x84\xd8\xaa\xd9\x82\xd9\x88\xd9\x8a\xd9\x85" },
    { "view.deep_time",     "\xd8\xb9\xd8\xb1\xd8\xb6 \xd8\xa7\xd9\x84\xd8\xb2\xd9\x85\xd9\x86 \xd8\xa7\xd9\x84\xd8\xb9\xd9\x85\xd9\x8a\xd9\x82" },
    /* Time */
    { "time.paused",        "\xd9\x85\xd8\xaa\xd9\x88\xd9\x82\xd9\x81" },
    { "time.reversed",      "\xd9\x85\xd8\xb9\xd9\x83\xd9\x88\xd8\xb3" },
    { "time.speed",         "\xd8\xa7\xd9\x84\xd8\xb3\xd8\xb1\xd8\xb9\xd8\xa9" },
    { "time.now",           "\xd8\xa7\xd9\x84\xd8\xa2\xd9\x86" },
    { "time.realtime",      "\xd9\x88\xd9\x82\xd8\xaa \xd8\xad\xd9\x82\xd9\x8a\xd9\x82\xd9\x8a" },
    { "time.faster",        "\xd8\xa3\xd8\xb3\xd8\xb1\xd8\xb9" },
    { "time.slower",        "\xd8\xa3\xd8\xa8\xd8\xb7\xd8\xa3" },
    { "time.jump_to",       "\xd8\xa7\xd9\x86\xd8\xaa\xd9\x82\xd9\x84 \xd8\xa5\xd9\x84\xd9\x89" },
    /* Layers */
    { "layer.astronomy",    "\xd8\xb9\xd9\x84\xd9\x85 \xd8\xa7\xd9\x84\xd9\x81\xd9\x84\xd9\x83" },
    { "layer.astrology",    "\xd8\xb9\xd9\x84\xd9\x85 \xd8\xa7\xd9\x84\xd8\xaa\xd9\x86\xd8\xac\xd9\x8a\xd9\x85" },
    { "layer.calendar",     "\xd8\xa7\xd9\x84\xd8\xaa\xd9\x82\xd9\x88\xd9\x8a\xd9\x85\xd8\xa7\xd8\xaa" },
    { "layer.sacred",       "\xd8\xa7\xd9\x84\xd8\xa3\xd9\x86\xd8\xb8\xd9\x85\xd8\xa9 \xd8\xa7\xd9\x84\xd9\x85\xd9\x82\xd8\xaf\xd8\xb3\xd8\xa9" },
    { "layer.earth",        "\xd8\xa7\xd9\x84\xd8\xa3\xd8\xb1\xd8\xb6" },
    { "layer.display",      "\xd8\xa7\xd9\x84\xd8\xb9\xd8\xb1\xd8\xb6" },
    /* Content */
    { "content.wisdom",     "\xd8\xad\xd9\x83\xd9\x85\xd8\xa9" },
    { "content.fun_fact",   "\xd8\xad\xd9\x82\xd9\x8a\xd9\x82\xd8\xa9 \xd9\x85\xd9\x85\xd8\xaa\xd8\xb9\xd8\xa9" },
    { "content.story",      "\xd9\x82\xd8\xb5\xd8\xa9 \xd8\xab\xd9\x82\xd8\xa7\xd9\x81\xd9\x8a\xd8\xa9" },
    { "content.achievement","\xd8\xa5\xd9\x86\xd8\xac\xd8\xa7\xd8\xb2" },
    { "content.drama",      "\xd8\xaf\xd8\xb1\xd8\xa7\xd9\x85\xd8\xa7" },
    { "content.card",       "\xd8\xa8\xd8\xb7\xd8\xa7\xd9\x82\xd8\xa9" },
    { "content.help",       "\xd9\x85\xd8\xb3\xd8\xa7\xd8\xb9\xd8\xaf\xd8\xa9" },
    { "content.command",    "\xd8\xa3\xd9\x85\xd8\xb1" },
    /* Sensitivity */
    { "sensitivity.general",    "\xd8\xb9\xd8\xa7\xd9\x85" },
    { "sensitivity.respectful", "\xd9\x85\xd8\xad\xd8\xaa\xd8\xb1\xd9\x85" },
    { "sensitivity.sacred",     "\xd9\x85\xd9\x82\xd8\xaf\xd8\xb3" },
    /* Navigation */
    { "nav.home",           "\xd8\xa7\xd9\x84\xd8\xb1\xd8\xa6\xd9\x8a\xd8\xb3\xd9\x8a\xd8\xa9" },
    { "nav.settings",       "\xd8\xa7\xd9\x84\xd8\xa5\xd8\xb9\xd8\xaf\xd8\xa7\xd8\xaf\xd8\xa7\xd8\xaa" },
    { "nav.about",          "\xd8\xad\xd9\x88\xd9\x84" },
    { "nav.share",          "\xd9\x85\xd8\xb4\xd8\xa7\xd8\xb1\xd9\x83\xd8\xa9" },
    { "nav.search",         "\xd8\xa8\xd8\xad\xd8\xab" },
    { "nav.close",          "\xd8\xa5\xd8\xba\xd9\x84\xd8\xa7\xd9\x82" },
    /* Calendar */
    { "cal.month",          "\xd8\xb4\xd9\x87\xd8\xb1" },
    { "cal.day",            "\xd9\x8a\xd9\x88\xd9\x85" },
    { "cal.cycle",          "\xd8\xaf\xd9\x88\xd8\xb1\xd8\xa9" },
    { "cal.element",        "\xd8\xb9\xd9\x86\xd8\xb5\xd8\xb1" },
    { "cal.sign",           "\xd8\xa8\xd8\xb1\xd8\xac" },
    { "cal.festival",       "\xd8\xb9\xd9\x8a\xd8\xaf" },
    /* Toast */
    { "toast.speed_changed",    "\xd8\xaa\xd9\x85 \xd8\xaa\xd8\xba\xd9\x8a\xd9\x8a\xd8\xb1 \xd8\xa7\xd9\x84\xd8\xb3\xd8\xb1\xd8\xb9\xd8\xa9" },
    { "toast.view_changed",     "\xd8\xaa\xd9\x85 \xd8\xaa\xd8\xba\xd9\x8a\xd9\x8a\xd8\xb1 \xd8\xa7\xd9\x84\xd8\xb9\xd8\xb1\xd8\xb6" },
    { "toast.layer_toggled",    "\xd8\xaa\xd9\x85 \xd8\xaa\xd8\xa8\xd8\xaf\xd9\x8a\xd9\x84 \xd8\xa7\xd9\x84\xd8\xb7\xd8\xa8\xd9\x82\xd8\xa9" },
    { "toast.jump_complete",    "\xd8\xa7\xd9\x83\xd8\xaa\xd9\x85\xd9\x84 \xd8\xa7\xd9\x84\xd8\xa7\xd9\x86\xd8\xaa\xd9\x82\xd8\xa7\xd9\x84" },
    { "toast.location_set",     "\xd8\xaa\xd9\x85 \xd8\xaa\xd8\xad\xd8\xaf\xd9\x8a\xd8\xaf \xd8\xa7\xd9\x84\xd9\x85\xd9\x88\xd9\x82\xd8\xb9" },
    { "toast.time_paused",      "\xd8\xaa\xd9\x85 \xd8\xa5\xd9\x8a\xd9\x82\xd8\xa7\xd9\x81 \xd8\xa7\xd9\x84\xd9\x88\xd9\x82\xd8\xaa" },
    /* Accessibility */
    { "a11y.skip_to_content",  "\xd8\xa7\xd9\x86\xd8\xaa\xd9\x82\xd9\x84 \xd8\xa5\xd9\x84\xd9\x89 \xd8\xa7\xd9\x84\xd9\x85\xd8\xad\xd8\xaa\xd9\x88\xd9\x89" },
    { "a11y.screen_reader",    "\xd9\x88\xd8\xb6\xd8\xb9 \xd9\x82\xd8\xa7\xd8\xb1\xd8\xa6 \xd8\xa7\xd9\x84\xd8\xb4\xd8\xa7\xd8\xb4\xd8\xa9" },
    { "a11y.high_contrast",    "\xd8\xaa\xd8\xa8\xd8\xa7\xd9\x8a\xd9\x86 \xd8\xb9\xd8\xa7\xd9\x84\xd9\x8d" },
    { "a11y.reduced_motion",   "\xd8\xad\xd8\xb1\xd9\x83\xd8\xa9 \xd9\x85\xd8\xae\xd9\x81\xd9\x81\xd8\xa9" },
    /* Welcome */
    { "welcome.when_born",     "\xd9\x85\xd8\xaa\xd9\x89 \xd9\x88\xd8\xb5\xd9\x84\xd8\xaa \xd8\xa5\xd9\x84\xd9\x89 \xd9\x87\xd8\xb0\xd8\xa7 \xd8\xa7\xd9\x84\xd9\x83\xd9\x88\xd9\x83\xd8\xa8\xd8\x9f" },
    { "welcome.where_born",    "\xd8\xa3\xd9\x8a\xd9\x86 \xd9\x83\xd9\x86\xd8\xaa \xd8\xb9\xd9\x84\xd9\x89 \xd8\xa7\xd9\x84\xd8\xa3\xd8\xb1\xd8\xb6\xd8\x9f" },
    { "welcome.skip",          "\xd8\xaa\xd8\xae\xd8\xb7\xd9\x8a" },
    { "welcome.continue",      "\xd9\x85\xd8\xaa\xd8\xa7\xd8\xa8\xd8\xb9\xd8\xa9" },
    { "welcome.reveal",        "\xd8\xa7\xd9\x83\xd8\xaa\xd8\xb4\xd9\x81 \xd9\x87\xd9\x88\xd9\x8a\xd8\xaa\xd9\x83 \xd8\xa7\xd9\x84\xd9\x83\xd9\x88\xd9\x86\xd9\x8a\xd8\xa9" },
    { "welcome.complete",      "\xd9\x85\xd8\xb1\xd8\xad\xd8\xa8\xd9\x8b\xd8\xa7 \xd8\xa8\xd9\x83 \xd9\x81\xd9\x8a \xd8\xa7\xd9\x84\xd9\x88\xd9\x82\xd8\xaa" },
    /* Error */
    { "error.no_data",         "\xd9\x84\xd8\xa7 \xd8\xaa\xd9\x88\xd8\xac\xd8\xaf \xd8\xa8\xd9\x8a\xd8\xa7\xd9\x86\xd8\xa7\xd8\xaa" },
    { "error.invalid_date",    "\xd8\xaa\xd8\xa7\xd8\xb1\xd9\x8a\xd8\xae \xd8\xba\xd9\x8a\xd8\xb1 \xd8\xb5\xd8\xa7\xd9\x84\xd8\xad" },
    { "error.out_of_range",    "\xd8\xaa\xd8\xa7\xd8\xb1\xd9\x8a\xd8\xae \xd8\xae\xd8\xa7\xd8\xb1\xd8\xac \xd8\xa7\xd9\x84\xd9\x86\xd8\xb7\xd8\xa7\xd9\x82" },
    { "error.unknown",         "\xd8\xae\xd8\xb7\xd8\xa3 \xd8\xba\xd9\x8a\xd8\xb1 \xd9\x85\xd8\xb9\xd8\xb1\xd9\x88\xd9\x81" },
    /* Attribution */
    { "attr.source",           "\xd8\xa7\xd9\x84\xd9\x85\xd8\xb5\xd8\xaf\xd8\xb1" },
    { "attr.author",           "\xd8\xa7\xd9\x84\xd9\x85\xd8\xa4\xd9\x84\xd9\x81" },
    /* Planets */
    { "planet.sun",       "\xd8\xa7\xd9\x84\xd8\xb4\xd9\x85\xd8\xb3" },
    { "planet.moon",      "\xd8\xa7\xd9\x84\xd9\x82\xd9\x85\xd8\xb1" },
    { "planet.mercury",   "\xd8\xb9\xd8\xb7\xd8\xa7\xd8\xb1\xd8\xaf" },
    { "planet.venus",     "\xd8\xa7\xd9\x84\xd8\xb2\xd9\x87\xd8\xb1\xd8\xa9" },
    { "planet.mars",      "\xd8\xa7\xd9\x84\xd9\x85\xd8\xb1\xd9\x8a\xd8\xae" },
    { "planet.jupiter",   "\xd8\xa7\xd9\x84\xd9\x85\xd8\xb4\xd8\xaa\xd8\xb1\xd9\x8a" },
    { "planet.saturn",    "\xd8\xb2\xd8\xad\xd9\x84" },
    { "planet.neptune",   "\xd9\x86\xd8\xa8\xd8\xaa\xd9\x88\xd9\x86" }
};

#define AR_STRING_COUNT \
    ((int)(sizeof(ar_strings) / sizeof(ar_strings[0])))

/* ---- Chinese Simplified (ZH) ---- */

static const i18n_entry_t zh_strings[] = {
    /* System names */
    { "system.gregorian",    "\xe5\x85\xac\xe5\x8e\x86" },
    { "system.hebrew",       "\xe5\xb8\x8c\xe4\xbc\xaf\xe6\x9d\xa5\xe5\x8e\x86" },
    { "system.islamic",      "\xe4\xbc\x8a\xe6\x96\xaf\xe5\x85\xb0\xe5\x8e\x86" },
    { "system.chinese",      "\xe5\x86\x9c\xe5\x8e\x86" },
    { "system.hindu",        "\xe5\x8d\xb0\xe5\xba\xa6\xe6\x95\x99\xe5\x8e\x86" },
    { "system.buddhist",     "\xe4\xbd\x9b\xe5\x8e\x86" },
    { "system.mayan",        "\xe7\x8e\x9b\xe9\x9b\x85\xe5\x8e\x86" },
    { "system.coptic",       "\xe7\xa7\x91\xe6\x99\xae\xe7\x89\xb9\xe5\x8e\x86" },
    { "system.ethiopian",    "\xe5\x9f\x83\xe5\xa1\x9e\xe4\xbf\x84\xe6\xaf\x94\xe4\xba\x9a\xe5\x8e\x86" },
    { "system.zoroastrian",  "\xe7\x90\x90\xe7\xbd\x97\xe4\xba\x9a\xe6\x96\xaf\xe5\xbe\xb7\xe5\x8e\x86" },
    { "system.celtic",       "\xe5\x87\xaf\xe5\xb0\x94\xe7\x89\xb9\xe5\x8e\x86" },
    { "system.myanmar",      "\xe7\xbc\x85\xe7\x94\xb8\xe5\x8e\x86" },
    { "system.egyptian",     "\xe5\x8f\xa4\xe5\x9f\x83\xe5\x8f\x8a\xe5\x8e\x86" },
    { "system.astrology",    "\xe5\x8d\xa0\xe6\x98\x9f\xe6\x9c\xaf" },
    { "system.iching",       "\xe6\x98\x93\xe7\xbb\x8f" },
    { "system.human_design", "\xe4\xba\xba\xe7\xb1\xbb\xe8\xae\xbe\xe8\xae\xa1" },
    /* UI labels */
    { "ui.zoom_level",    "\xe7\xbc\xa9\xe6\x94\xbe\xe7\xba\xa7\xe5\x88\xab" },
    { "ui.view_space",    "\xe5\xa4\xaa\xe7\xa9\xba\xe8\xa7\x86\xe5\x9b\xbe" },
    { "ui.view_earth",    "\xe5\x9c\xb0\xe7\x90\x83\xe8\xa7\x86\xe5\x9b\xbe" },
    { "ui.festival_alert","\xe8\x8a\x82\xe6\x97\xa5\xe6\x8f\x90\xe9\x86\x92" },
    { "ui.retrograde",    "\xe9\x80\x86\xe8\xa1\x8c" },
    { "ui.direct",        "\xe9\xa1\xba\xe8\xa1\x8c" },
    { "ui.day",           "\xe5\xa4\xa9" },
    { "ui.days",          "\xe5\xa4\xa9" },
    { "ui.month",         "\xe6\x9c\x88" },
    { "ui.months",        "\xe6\x9c\x88" },
    { "ui.year",          "\xe5\xb9\xb4" },
    { "ui.years",         "\xe5\xb9\xb4" },
    { "ui.loading",       "\xe5\x8a\xa0\xe8\xbd\xbd\xe4\xb8\xad..." },
    { "ui.error",         "\xe9\x94\x99\xe8\xaf\xaf" },
    /* Additional system names */
    { "system.kabbalah",    "\xe5\x8d\xa1\xe5\xb7\xb4\xe6\x8b\x89" },
    { "system.geology",     "\xe5\x9c\xb0\xe8\xb4\xa8\xe5\xad\xa6" },
    { "system.earth",       "\xe5\x9c\xb0\xe7\x90\x83" },
    { "system.unified",     "\xe7\xbb\x9f\xe4\xb8\x80" },
    /* Depth */
    { "depth.surface",      "\xe8\xa1\xa8\xe9\x9d\xa2" },
    { "depth.context",      "\xe8\x83\x8c\xe6\x99\xaf" },
    { "depth.cycle",        "\xe5\x91\xa8\xe6\x9c\x9f" },
    { "depth.cosmic",       "\xe5\xae\x87\xe5\xae\x99" },
    /* Views */
    { "view.space",         "\xe5\xa4\xaa\xe7\xa9\xba\xe8\xa7\x86\xe5\x9b\xbe" },
    { "view.earth",         "\xe5\x9c\xb0\xe7\x90\x83\xe8\xa7\x86\xe5\x9b\xbe" },
    { "view.tzolkin",       "\xe5\x8d\x93\xe5\xb0\x94\xe9\x87\x91\xe8\xa7\x86\xe5\x9b\xbe" },
    { "view.iching",        "\xe6\x98\x93\xe7\xbb\x8f\xe8\xa7\x86\xe5\x9b\xbe" },
    { "view.calendar",      "\xe6\x97\xa5\xe5\x8e\x86\xe8\xa7\x86\xe5\x9b\xbe" },
    { "view.deep_time",     "\xe6\xb7\xb1\xe6\x97\xb6\xe8\xa7\x86\xe5\x9b\xbe" },
    /* Time */
    { "time.paused",        "\xe5\xb7\xb2\xe6\x9a\x82\xe5\x81\x9c" },
    { "time.reversed",      "\xe5\xb7\xb2\xe5\x8f\x8d\xe8\xbd\xac" },
    { "time.speed",         "\xe9\x80\x9f\xe5\xba\xa6" },
    { "time.now",           "\xe7\x8e\xb0\xe5\x9c\xa8" },
    { "time.realtime",      "\xe5\xae\x9e\xe6\x97\xb6" },
    { "time.faster",        "\xe5\x8a\xa0\xe9\x80\x9f" },
    { "time.slower",        "\xe5\x87\x8f\xe9\x80\x9f" },
    { "time.jump_to",       "\xe8\xb7\xb3\xe8\xbd\xac\xe5\x88\xb0" },
    /* Layers */
    { "layer.astronomy",    "\xe5\xa4\xa9\xe6\x96\x87\xe5\xad\xa6" },
    { "layer.astrology",    "\xe5\x8d\xa0\xe6\x98\x9f\xe6\x9c\xaf" },
    { "layer.calendar",     "\xe5\x8e\x86\xe6\xb3\x95" },
    { "layer.sacred",       "\xe7\xa5\x9e\xe5\x9c\xa3\xe7\xb3\xbb\xe7\xbb\x9f" },
    { "layer.earth",        "\xe5\x9c\xb0\xe7\x90\x83" },
    { "layer.display",      "\xe6\x98\xbe\xe7\xa4\xba" },
    /* Content */
    { "content.wisdom",     "\xe6\x99\xba\xe6\x85\xa7" },
    { "content.fun_fact",   "\xe8\xb6\xa3\xe9\x97\xbb" },
    { "content.story",      "\xe6\x96\x87\xe5\x8c\x96\xe6\x95\x85\xe4\xba\x8b" },
    { "content.achievement","\xe6\x88\x90\xe5\xb0\xb1" },
    { "content.drama",      "\xe6\x88\x8f\xe5\x89\xa7\xe5\x8c\x96" },
    { "content.card",       "\xe5\x8d\xa1\xe7\x89\x87" },
    { "content.help",       "\xe5\xb8\xae\xe5\x8a\xa9" },
    { "content.command",    "\xe5\x91\xbd\xe4\xbb\xa4" },
    /* Sensitivity */
    { "sensitivity.general",    "\xe9\x80\x9a\xe7\x94\xa8" },
    { "sensitivity.respectful", "\xe5\xb0\x8a\xe9\x87\x8d" },
    { "sensitivity.sacred",     "\xe7\xa5\x9e\xe5\x9c\xa3" },
    /* Navigation */
    { "nav.home",           "\xe9\xa6\x96\xe9\xa1\xb5" },
    { "nav.settings",       "\xe8\xae\xbe\xe7\xbd\xae" },
    { "nav.about",          "\xe5\x85\xb3\xe4\xba\x8e" },
    { "nav.share",          "\xe5\x88\x86\xe4\xba\xab" },
    { "nav.search",         "\xe6\x90\x9c\xe7\xb4\xa2" },
    { "nav.close",          "\xe5\x85\xb3\xe9\x97\xad" },
    /* Calendar */
    { "cal.month",          "\xe6\x9c\x88" },
    { "cal.day",            "\xe6\x97\xa5" },
    { "cal.cycle",          "\xe5\x91\xa8\xe6\x9c\x9f" },
    { "cal.element",        "\xe5\x85\x83\xe7\xb4\xa0" },
    { "cal.sign",           "\xe6\x98\x9f\xe5\xba\xa7" },
    { "cal.festival",       "\xe8\x8a\x82\xe6\x97\xa5" },
    /* Toast */
    { "toast.speed_changed",    "\xe9\x80\x9f\xe5\xba\xa6\xe5\xb7\xb2\xe6\x9b\xb4\xe6\x94\xb9" },
    { "toast.view_changed",     "\xe8\xa7\x86\xe5\x9b\xbe\xe5\xb7\xb2\xe6\x9b\xb4\xe6\x94\xb9" },
    { "toast.layer_toggled",    "\xe5\x9b\xbe\xe5\xb1\x82\xe5\xb7\xb2\xe5\x88\x87\xe6\x8d\xa2" },
    { "toast.jump_complete",    "\xe8\xb7\xb3\xe8\xbd\xac\xe5\xae\x8c\xe6\x88\x90" },
    { "toast.location_set",     "\xe4\xbd\x8d\xe7\xbd\xae\xe5\xb7\xb2\xe8\xae\xbe\xe5\xae\x9a" },
    { "toast.time_paused",      "\xe6\x97\xb6\xe9\x97\xb4\xe5\xb7\xb2\xe6\x9a\x82\xe5\x81\x9c" },
    /* Accessibility */
    { "a11y.skip_to_content",  "\xe8\xb7\xb3\xe8\x87\xb3\xe5\x86\x85\xe5\xae\xb9" },
    { "a11y.screen_reader",    "\xe5\xb1\x8f\xe5\xb9\x95\xe9\x98\x85\xe8\xaf\xbb\xe5\x99\xa8\xe6\xa8\xa1\xe5\xbc\x8f" },
    { "a11y.high_contrast",    "\xe9\xab\x98\xe5\xaf\xb9\xe6\xaf\x94\xe5\xba\xa6" },
    { "a11y.reduced_motion",   "\xe5\x87\x8f\xe5\xb0\x91\xe5\x8a\xa8\xe6\x95\x88" },
    /* Welcome */
    { "welcome.when_born",     "\xe4\xbd\xa0\xe4\xbd\x95\xe6\x97\xb6\xe6\x9d\xa5\xe5\x88\xb0\xe8\xbf\x99\xe4\xb8\xaa\xe6\x98\x9f\xe7\x90\x83\xef\xbc\x9f" },
    { "welcome.where_born",    "\xe4\xbd\xa0\xe5\xbd\x93\xe6\x97\xb6\xe5\x9c\xa8\xe5\x9c\xb0\xe7\x90\x83\xe7\x9a\x84\xe5\x93\xaa\xe9\x87\x8c\xef\xbc\x9f" },
    { "welcome.skip",          "\xe8\xb7\xb3\xe8\xbf\x87" },
    { "welcome.continue",      "\xe7\xbb\xa7\xe7\xbb\xad" },
    { "welcome.reveal",        "\xe6\x8e\xa2\xe7\xb4\xa2\xe4\xbd\xa0\xe7\x9a\x84\xe5\xae\x87\xe5\xae\x99\xe8\xba\xab\xe4\xbb\xbd" },
    { "welcome.complete",      "\xe6\xac\xa2\xe8\xbf\x8e\xe6\x9d\xa5\xe5\x88\xb0 Time" },
    /* Error */
    { "error.no_data",         "\xe6\x9a\x82\xe6\x97\xa0\xe6\x95\xb0\xe6\x8d\xae" },
    { "error.invalid_date",    "\xe6\x97\xa5\xe6\x9c\x9f\xe6\x97\xa0\xe6\x95\x88" },
    { "error.out_of_range",    "\xe6\x97\xa5\xe6\x9c\x9f\xe8\xb6\x85\xe5\x87\xba\xe8\x8c\x83\xe5\x9b\xb4" },
    { "error.unknown",         "\xe6\x9c\xaa\xe7\x9f\xa5\xe9\x94\x99\xe8\xaf\xaf" },
    /* Attribution */
    { "attr.source",           "\xe6\x9d\xa5\xe6\xba\x90" },
    { "attr.author",           "\xe4\xbd\x9c\xe8\x80\x85" },
    /* Planets */
    { "planet.sun",       "\xe5\xa4\xaa\xe9\x98\xb3" },
    { "planet.moon",      "\xe6\x9c\x88\xe4\xba\xae" },
    { "planet.mercury",   "\xe6\xb0\xb4\xe6\x98\x9f" },
    { "planet.venus",     "\xe9\x87\x91\xe6\x98\x9f" },
    { "planet.mars",      "\xe7\x81\xab\xe6\x98\x9f" },
    { "planet.jupiter",   "\xe6\x9c\xa8\xe6\x98\x9f" },
    { "planet.saturn",    "\xe5\x9c\x9f\xe6\x98\x9f" },
    { "planet.neptune",   "\xe6\xb5\xb7\xe7\x8e\x8b\xe6\x98\x9f" }
};

#define ZH_STRING_COUNT \
    ((int)(sizeof(zh_strings) / sizeof(zh_strings[0])))

/* ============================================================
 * LOCALE DATA — per-locale table dispatch
 * ============================================================ */

typedef struct {
    const i18n_entry_t *entries;
    int count;
} i18n_locale_table_t;

static const i18n_locale_table_t locale_tables[I18N_LOCALE_COUNT] = {
    [I18N_LOCALE_EN] = { NULL, 0 },   /* English is the default */
    [I18N_LOCALE_ES] = { es_strings, ES_STRING_COUNT },
    [I18N_LOCALE_AR] = { ar_strings, AR_STRING_COUNT },
    [I18N_LOCALE_ZH] = { zh_strings, ZH_STRING_COUNT }
    /* All other locales default to { NULL, 0 } — fall back to English */
};

/* ---- Locale metadata tables (21 locales) ---- */

static const char *locale_names[] = {
    "English",      /* EN */
    "Spanish",      /* ES */
    "Arabic",       /* AR */
    "Hebrew",       /* HE */
    "Chinese",      /* ZH */
    "Hindi",        /* HI */
    "Japanese",     /* JA */
    "French",       /* FR */
    "Portuguese",   /* PT */
    "German",       /* DE */
    "Russian",      /* RU */
    "Korean",       /* KO */
    "Thai",         /* TH */
    "Turkish",      /* TR */
    "Indonesian",   /* ID */
    "Myanmar",      /* MY */
    "Amharic",      /* AM */
    "Bengali",      /* BN */
    "Vietnamese",   /* VI */
    "Swahili",      /* SW */
    "Persian"       /* FA */
};

static const char *locale_codes[] = {
    "en", "es", "ar", "he", "zh", "hi", "ja",
    "fr", "pt", "de", "ru", "ko", "th", "tr",
    "id", "my", "am", "bn", "vi", "sw", "fa"
};

static const i18n_plural_rule_t locale_plural_rules[] = {
    I18N_PLURAL_ONE_OTHER,   /* EN */
    I18N_PLURAL_ONE_OTHER,   /* ES */
    I18N_PLURAL_ARABIC,      /* AR */
    I18N_PLURAL_ONE_OTHER,   /* HE */
    I18N_PLURAL_EAST_ASIAN,  /* ZH */
    I18N_PLURAL_ONE_OTHER,   /* HI */
    I18N_PLURAL_EAST_ASIAN,  /* JA */
    I18N_PLURAL_ONE_OTHER,   /* FR */
    I18N_PLURAL_ONE_OTHER,   /* PT */
    I18N_PLURAL_ONE_OTHER,   /* DE */
    I18N_PLURAL_SLAVIC,      /* RU */
    I18N_PLURAL_EAST_ASIAN,  /* KO */
    I18N_PLURAL_EAST_ASIAN,  /* TH */
    I18N_PLURAL_ONE_OTHER,   /* TR */
    I18N_PLURAL_EAST_ASIAN,  /* ID */
    I18N_PLURAL_EAST_ASIAN,  /* MY */
    I18N_PLURAL_ONE_OTHER,   /* AM */
    I18N_PLURAL_ONE_OTHER,   /* BN */
    I18N_PLURAL_EAST_ASIAN,  /* VI */
    I18N_PLURAL_ONE_OTHER,   /* SW */
    I18N_PLURAL_ONE_OTHER    /* FA */
};

static const bool locale_rtl[] = {
    false,  /* EN */
    false,  /* ES */
    true,   /* AR */
    true,   /* HE */
    false,  /* ZH */
    false,  /* HI */
    false,  /* JA */
    false,  /* FR */
    false,  /* PT */
    false,  /* DE */
    false,  /* RU */
    false,  /* KO */
    false,  /* TH */
    false,  /* TR */
    false,  /* ID */
    false,  /* MY */
    false,  /* AM */
    false,  /* BN */
    false,  /* VI */
    false,  /* SW */
    true    /* FA */
};

static const char *form_names[] = {
    "one", "two", "few", "many", "other"
};

/* ---- Implementation ---- */

const char *i18n_get(const char *key)
{
    if (key == NULL) {
        return "";
    }
    for (int i = 0; i < DEFAULT_STRING_COUNT; i++) {
        if (strcmp(key, default_strings[i].key) == 0) {
            return default_strings[i].value;
        }
    }
    /* Key not found — return the key itself */
    return key;
}

const char *i18n_locale_name(i18n_locale_t locale)
{
    if (locale < 0 || locale >= I18N_LOCALE_COUNT) {
        return "Unknown";
    }
    return locale_names[locale];
}

i18n_plural_rule_t i18n_plural_rule(i18n_locale_t locale)
{
    if (locale < 0 || locale >= I18N_LOCALE_COUNT) {
        return I18N_PLURAL_ONE_OTHER;
    }
    return locale_plural_rules[locale];
}

static i18n_plural_form_t plural_one_other(int count)
{
    if (count == 1) {
        return I18N_FORM_ONE;
    }
    return I18N_FORM_OTHER;
}

static i18n_plural_form_t plural_east_asian(int count)
{
    (void)count;
    return I18N_FORM_OTHER;
}

static i18n_plural_form_t plural_arabic(int count)
{
    if (count < 0) {
        return I18N_FORM_OTHER;
    }
    if (count == 0) {
        return I18N_FORM_OTHER;
    }
    if (count == 1) {
        return I18N_FORM_ONE;
    }
    if (count == 2) {
        return I18N_FORM_TWO;
    }
    if (count >= 3 && count <= 10) {
        return I18N_FORM_FEW;
    }
    if (count >= 11 && count <= 99) {
        return I18N_FORM_MANY;
    }
    return I18N_FORM_OTHER;
}

static i18n_plural_form_t plural_slavic(int count)
{
    if (count < 0) {
        return I18N_FORM_OTHER;
    }

    int mod10 = count % 10;
    int mod100 = count % 100;

    /* Exception: 11-14 are always "many" */
    if (mod100 >= 11 && mod100 <= 14) {
        return I18N_FORM_MANY;
    }
    if (mod10 == 1) {
        return I18N_FORM_ONE;
    }
    if (mod10 >= 2 && mod10 <= 4) {
        return I18N_FORM_FEW;
    }
    return I18N_FORM_MANY;
}

i18n_plural_form_t i18n_plural_form(i18n_plural_rule_t rule, int count)
{
    switch (rule) {
    case I18N_PLURAL_ONE_OTHER:
        return plural_one_other(count);
    case I18N_PLURAL_EAST_ASIAN:
        return plural_east_asian(count);
    case I18N_PLURAL_ARABIC:
        return plural_arabic(count);
    case I18N_PLURAL_SLAVIC:
        return plural_slavic(count);
    default:
        return I18N_FORM_OTHER;
    }
}

const char *i18n_plural_form_name(i18n_plural_form_t form)
{
    if (form < 0 || form >= I18N_FORM_COUNT) {
        return "other";
    }
    return form_names[form];
}

int i18n_key_count(void)
{
    return DEFAULT_STRING_COUNT;
}

i18n_entry_t i18n_entry(int index)
{
    if (index < 0 || index >= DEFAULT_STRING_COUNT) {
        i18n_entry_t empty = { NULL, NULL };
        return empty;
    }
    return default_strings[index];
}

bool i18n_key_exists(const char *key)
{
    if (key == NULL || key[0] == '\0') {
        return false;
    }
    for (int i = 0; i < DEFAULT_STRING_COUNT; i++) {
        if (strcmp(key, default_strings[i].key) == 0) {
            return true;
        }
    }
    return false;
}

i18n_locale_t i18n_locale_from_code(const char *code)
{
    if (code == NULL || code[0] == '\0') {
        return I18N_LOCALE_EN;
    }
    for (int i = 0; i < I18N_LOCALE_COUNT; i++) {
        if (strcmp(code, locale_codes[i]) == 0) {
            return (i18n_locale_t)i;
        }
    }
    return I18N_LOCALE_EN;
}

const char *i18n_locale_code(i18n_locale_t locale)
{
    if (locale < 0 || locale >= I18N_LOCALE_COUNT) {
        return "";
    }
    return locale_codes[locale];
}

bool i18n_is_rtl(i18n_locale_t locale)
{
    if (locale < 0 || locale >= I18N_LOCALE_COUNT) {
        return false;
    }
    return locale_rtl[locale];
}

const char *i18n_get_locale(const char *key, i18n_locale_t locale)
{
    if (key == NULL) {
        return "";
    }

    /* Try locale-specific table first */
    if (locale >= 0 && locale < I18N_LOCALE_COUNT) {
        const i18n_locale_table_t *table = &locale_tables[locale];
        if (table->entries != NULL) {
            for (int i = 0; i < table->count; i++) {
                if (strcmp(key, table->entries[i].key) == 0) {
                    return table->entries[i].value;
                }
            }
        }
    }

    /* Fall back to English default */
    return i18n_get(key);
}

float i18n_locale_coverage(i18n_locale_t locale)
{
    if (locale < 0 || locale >= I18N_LOCALE_COUNT) {
        return 0.0f;
    }
    if (locale == I18N_LOCALE_EN) {
        return 1.0f;
    }
    if (DEFAULT_STRING_COUNT == 0) {
        return 0.0f;
    }
    const i18n_locale_table_t *table = &locale_tables[locale];
    if (table->entries == NULL || table->count == 0) {
        return 0.0f;
    }
    return (float)table->count / (float)DEFAULT_STRING_COUNT;
}

int i18n_translated_count(i18n_locale_t locale)
{
    if (locale < 0 || locale >= I18N_LOCALE_COUNT) {
        return 0;
    }
    if (locale == I18N_LOCALE_EN) {
        return DEFAULT_STRING_COUNT;
    }
    const i18n_locale_table_t *table = &locale_tables[locale];
    if (table->entries == NULL) {
        return 0;
    }
    return table->count;
}
