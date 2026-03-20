/* audio_culture.c -- Cultural audio timbre data.
 *
 * Defines harmonic partial arrays for each cultural tradition.
 * When a calendar system's convergence event is active, its sonic
 * identity can layer into the audio mix.
 *
 * Partial amplitudes are inspired by the harmonic spectrum of each
 * tradition's characteristic instrument, simplified to 8 partials.
 *
 * Brass (shofar): strong odd harmonics, gradual decay
 * Bell/bowl: strong fundamental, decaying even harmonics, bright high partials
 * String (oud/sitar): rich all harmonics, slow decay
 * Flute (wood): strong fundamental, weak overtones
 * Plucked string (guzheng/koto): moderate decay, some even prominence
 *
 * Pure data module: no GL, no malloc, no globals, no side effects. */

#include "audio_culture.h"

#define SUPPORTED_COUNT 21

static const audio_culture_t TIMBRES[SUPPORTED_COUNT] = {
    /* --- Original 10 timbres --- */

    {CD_SYS_ASTRONOMY, "Astronomy", "pure tone",
     440.0f, 2, {1.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 0.15f},

    {CD_SYS_ASTROLOGY, "Astrology", "glass harmonica",
     392.0f, 4, {1.0f, 0.15f, 0.05f, 0.35f, 0.0f, 0.0f, 0.0f, 0.0f}, 0.18f},

    {CD_SYS_TZOLKIN, "Tzolkin", "wooden flute",
     330.0f, 3, {1.0f, 0.15f, 0.05f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 0.25f},

    {CD_SYS_ICHING, "I Ching", "bronze bell",
     165.0f, 6, {1.0f, 0.6f, 0.8f, 0.3f, 0.5f, 0.2f, 0.0f, 0.0f}, 0.22f},

    {CD_SYS_CHINESE, "Chinese", "guzheng pluck",
     262.0f, 6, {1.0f, 0.5f, 0.3f, 0.4f, 0.2f, 0.1f, 0.0f, 0.0f}, 0.22f},

    {CD_SYS_HEBREW, "Hebrew", "shofar brass",
     220.0f, 6, {1.0f, 0.0f, 0.7f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f}, 0.25f},

    {CD_SYS_ISLAMIC, "Islamic", "oud strings",
     196.0f, 8, {1.0f, 0.8f, 0.6f, 0.5f, 0.4f, 0.3f, 0.2f, 0.15f}, 0.20f},

    {CD_SYS_HINDU, "Hindu", "sitar buzz",
     240.0f, 8, {1.0f, 0.9f, 0.7f, 0.6f, 0.5f, 0.45f, 0.35f, 0.3f}, 0.20f},

    {CD_SYS_BUDDHIST, "Buddhist", "singing bowl",
     264.0f, 5, {1.0f, 0.3f, 0.6f, 0.2f, 0.4f, 0.0f, 0.0f, 0.0f}, 0.30f},

    {CD_SYS_PERSIAN, "Persian", "tar strings",
     220.0f, 7, {1.0f, 0.7f, 0.5f, 0.4f, 0.35f, 0.25f, 0.15f, 0.0f}, 0.20f},

    /* --- 11 new timbres (full system coverage) --- */

    {CD_SYS_COPTIC, "Coptic", "sistrum rattle",
     350.0f, 6, {1.0f, 0.55f, 0.7f, 0.4f, 0.5f, 0.3f, 0.0f, 0.0f}, 0.18f},

    {CD_SYS_CELTIC, "Celtic", "harp shimmer",
     294.0f, 5, {1.0f, 0.4f, 0.2f, 0.15f, 0.1f, 0.0f, 0.0f, 0.0f}, 0.22f},

    {CD_SYS_BAHAI, "Bahai", "reed flute",
     293.0f, 4, {1.0f, 0.25f, 0.1f, 0.05f, 0.0f, 0.0f, 0.0f, 0.0f}, 0.20f},

    {CD_SYS_JAPANESE, "Japanese", "koto pluck",
     277.0f, 6, {1.0f, 0.45f, 0.25f, 0.35f, 0.15f, 0.08f, 0.0f, 0.0f}, 0.20f},

    {CD_SYS_EGYPTIAN, "Egyptian", "ney flute",
     261.0f, 4, {1.0f, 0.2f, 0.08f, 0.03f, 0.0f, 0.0f, 0.0f, 0.0f}, 0.22f},

    {CD_SYS_FRENCH, "French Republican", "harpsichord",
     349.0f, 7, {1.0f, 0.75f, 0.55f, 0.4f, 0.3f, 0.2f, 0.1f, 0.0f}, 0.18f},

    {CD_SYS_KOREAN, "Korean", "gayageum pluck",
     246.0f, 6, {1.0f, 0.5f, 0.35f, 0.3f, 0.2f, 0.1f, 0.0f, 0.0f}, 0.20f},

    {CD_SYS_THAI, "Thai", "ranat ek",
     310.0f, 4, {1.0f, 0.3f, 0.15f, 0.08f, 0.0f, 0.0f, 0.0f, 0.0f}, 0.22f},

    {CD_SYS_TAMIL, "Tamil", "veena wire",
     220.0f, 7, {1.0f, 0.8f, 0.6f, 0.5f, 0.35f, 0.25f, 0.15f, 0.0f}, 0.20f},

    {CD_SYS_MYANMAR, "Myanmar", "saung harp",
     233.0f, 5, {1.0f, 0.35f, 0.18f, 0.12f, 0.06f, 0.0f, 0.0f, 0.0f}, 0.22f},

    {CD_SYS_ZOROASTRIAN, "Zoroastrian", "santur dulcimer",
     277.0f, 6, {1.0f, 0.6f, 0.45f, 0.35f, 0.25f, 0.15f, 0.0f, 0.0f}, 0.20f},
};

/* Null timbre for unsupported systems */
static const audio_culture_t NULL_TIMBRE = {
    CD_SYS_COUNT, "?", "?",
    0.0f, 0, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 0.0f};

audio_culture_t audio_culture_get(cd_system_t system) {
    for (int i = 0; i < SUPPORTED_COUNT; i++) {
        if (TIMBRES[i].system == system) {
            return TIMBRES[i];
        }
    }
    return NULL_TIMBRE;
}

const char* audio_culture_name(cd_system_t system) {
    for (int i = 0; i < SUPPORTED_COUNT; i++) {
        if (TIMBRES[i].system == system) {
            return TIMBRES[i].name;
        }
    }
    return "?";
}

const char* audio_culture_desc(cd_system_t system) {
    for (int i = 0; i < SUPPORTED_COUNT; i++) {
        if (TIMBRES[i].system == system) {
            return TIMBRES[i].timbre_desc;
        }
    }
    return "?";
}

int audio_culture_count(void) {
    return SUPPORTED_COUNT;
}

int audio_culture_has_timbre(cd_system_t system) {
    for (int i = 0; i < SUPPORTED_COUNT; i++) {
        if (TIMBRES[i].system == system) {
            return 1;
        }
    }
    return 0;
}
