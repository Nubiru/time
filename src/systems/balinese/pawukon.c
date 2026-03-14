/* pawukon.c -- Balinese Pawukon 210-day concurrent cycle calendar
 *
 * Pure arithmetic: all 10 concurrent weeks computed via modular arithmetic
 * from a single day-in-cycle value derived from Julian Day offset.
 *
 * Sources: Eiseman, "Bali: Sekala & Niskala" (1990).
 *          Darling, "The Painted Alphabet" (1992).
 *          Dershowitz & Reingold, "Calendrical Calculations" (2018). */

#include "pawukon.h"

#include <math.h>
#include <stddef.h>

/* Epoch: JD 2379344.5 = 15 July 1802 CE = Wuku Sinta, day 0 */
static const double PAWUKON_EPOCH_JD = 2379344.5;

/* Number of defined ceremonies */
#define CEREMONY_COUNT 7

/* ===== 30 Wuku names ===== */

static const char *WUKU_NAMES[PAWUKON_WUKU_COUNT] = {
    "Sinta",        "Landep",       "Ukir",         "Kulantir",
    "Tolu",         "Gumbreg",      "Wariga",       "Warigadean",
    "Julungwangi",  "Sungsang",     "Dungulan",     "Kuningan",
    "Langkir",      "Medangsia",    "Pujut",        "Pahang",
    "Krulut",       "Merakih",      "Tambir",       "Medangkungan",
    "Matal",        "Uye",          "Menail",       "Prangbakat",
    "Bala",         "Ugu",          "Wayang",       "Kelawu",
    "Dukut",        "Watugunung"
};

/* ===== Day names for 10 concurrent weeks ===== */

/* Eka Wara (1-day) */
static const char *EKA_WARA[] = { "Luang" };

/* Dwi Wara (2-day) */
static const char *DWI_WARA[] = { "Menga", "Pepet" };

/* Tri Wara (3-day) */
static const char *TRI_WARA[] = { "Pasah", "Beteng", "Kajeng" };

/* Catur Wara (4-day) */
static const char *CATUR_WARA[] = { "Sri", "Laba", "Jaya", "Menala" };

/* Panca Wara (5-day) */
static const char *PANCA_WARA[] = { "Umanis", "Pahing", "Pon", "Wage", "Kliwon" };

/* Sad Wara (6-day) */
static const char *SAD_WARA[] = {
    "Tungleh", "Aryang", "Urukung", "Paniron", "Was", "Maulu"
};

/* Sapta Wara (7-day) */
static const char *SAPTA_WARA[] = {
    "Redite", "Soma", "Anggara", "Buda", "Wraspati", "Sukra", "Saniscara"
};

/* Asta Wara (8-day) */
static const char *ASTA_WARA[] = {
    "Sri", "Indra", "Guru", "Yama", "Ludra", "Brahma", "Kala", "Uma"
};

/* Sanga Wara (9-day) */
static const char *SANGA_WARA[] = {
    "Dangu", "Jangur", "Gigis", "Nohan", "Ogan",
    "Erangan", "Urungan", "Tulus", "Dadi"
};

/* Dasa Wara (10-day) */
static const char *DASA_WARA[] = {
    "Pandita", "Pati", "Suka", "Duka", "Sri",
    "Manuh", "Manusa", "Raja", "Dewa", "Raksasa"
};

/* Lookup table: day_names[week_length] -> array, lengths[week_length] -> count */
static const char **DAY_NAME_TABLES[] = {
    NULL,        /* 0: invalid */
    EKA_WARA,    /* 1 */
    DWI_WARA,    /* 2 */
    TRI_WARA,    /* 3 */
    CATUR_WARA,  /* 4 */
    PANCA_WARA,  /* 5 */
    SAD_WARA,    /* 6 */
    SAPTA_WARA,  /* 7 */
    ASTA_WARA,   /* 8 */
    SANGA_WARA,  /* 9 */
    DASA_WARA    /* 10 */
};

/* ===== Urip values ===== */

/* Panca Wara urip: Umanis=5, Pahing=9, Pon=7, Wage=4, Kliwon=8 */
static const int PANCA_WARA_URIP[] = { 5, 9, 7, 4, 8 };

/* Sapta Wara urip: Redite=5, Soma=4, Anggara=3, Buda=7, Wraspati=8, Sukra=6, Saniscara=9 */
static const int SAPTA_WARA_URIP[] = { 5, 4, 3, 7, 8, 6, 9 };

/* ===== Major ceremonies ===== */

static const pawukon_ceremony_t CEREMONIES[CEREMONY_COUNT] = {
    { "Galungan",
      "Victory of dharma over adharma",
      10, -1, 3 },       /* Wuku Dungulan, Buda */
    { "Kuningan",
      "Ancestral spirits return to heaven",
      11, -1, 6 },       /* Wuku Kuningan, Saniscara */
    { "Saraswati",
      "Goddess of knowledge honored",
      29, -1, 6 },       /* Wuku Watugunung, Saniscara */
    { "Pagerwesi",
      "Strengthening of spirit",
      0, -1, 3 },        /* Wuku Sinta, Buda */
    { "Tumpek Landep",
      "Blessing of metal objects and technology",
      1, -1, 6 },        /* Wuku Landep, Saniscara */
    { "Tumpek Wayang",
      "Blessing of shadow puppets and performing arts",
      26, -1, 6 },       /* Wuku Wayang, Saniscara */
    { "Siwaratri",
      "Night of Shiva, meditation and fasting",
      10, -1, 5 }        /* Wuku Dungulan, Sukra */
};

/* ===== Positive modulo (handles negative dividends) ===== */

static int pos_mod(int a, int m)
{
    int r = a % m;
    return r < 0 ? r + m : r;
}

/* ===== Public API ===== */

int pawukon_day_of_cycle(double jd)
{
    int days = (int)floor(jd - PAWUKON_EPOCH_JD);
    return pos_mod(days, PAWUKON_CYCLE_DAYS);
}

pawukon_t pawukon_from_jd(double jd)
{
    pawukon_t p;
    int d = pawukon_day_of_cycle(jd);

    p.day_in_cycle = d;
    p.wuku         = d / 7;
    p.wuku_day     = d % 7;
    p.eka_wara     = 0;       /* always 0 */
    p.dwi_wara     = d % 2;
    p.tri_wara     = d % 3;
    p.catur_wara   = d % 4;
    p.panca_wara   = d % 5;
    p.sad_wara     = d % 6;
    p.sapta_wara   = d % 7;
    p.asta_wara    = d % 8;
    p.sanga_wara   = d % 9;
    p.dasa_wara    = d % 10;

    return p;
}

const char *pawukon_wuku_name(int wuku)
{
    if (wuku < 0 || wuku >= PAWUKON_WUKU_COUNT) {
        return "?";
    }
    return WUKU_NAMES[wuku];
}

const char *pawukon_day_name(int week_length, int day_index)
{
    if (week_length < 1 || week_length > PAWUKON_CONCURRENT_WEEKS) {
        return "?";
    }
    if (day_index < 0 || day_index >= week_length) {
        return "?";
    }
    return DAY_NAME_TABLES[week_length][day_index];
}

int pawukon_urip(pawukon_t p)
{
    return PANCA_WARA_URIP[p.panca_wara] + SAPTA_WARA_URIP[p.sapta_wara];
}

int pawukon_is_tumpek(pawukon_t p)
{
    /* Tumpek: Panca Wara = Kliwon (4) AND Sapta Wara = Saniscara (6) */
    return (p.panca_wara == 4) && (p.sapta_wara == 6);
}

int pawukon_is_kajeng_kliwon(pawukon_t p)
{
    /* Kajeng Kliwon: Tri Wara = Kajeng (2) AND Panca Wara = Kliwon (4) */
    return (p.tri_wara == 2) && (p.panca_wara == 4);
}

const char *pawukon_ceremony(pawukon_t p)
{
    for (int i = 0; i < CEREMONY_COUNT; i++) {
        const pawukon_ceremony_t *c = &CEREMONIES[i];
        if (c->wuku >= 0 && c->wuku != p.wuku) {
            continue;
        }
        if (c->sapta_wara >= 0 && c->sapta_wara != p.sapta_wara) {
            continue;
        }
        if (c->panca_wara >= 0 && c->panca_wara != p.panca_wara) {
            continue;
        }
        return c->name;
    }
    return NULL;
}

int pawukon_ceremony_count(void)
{
    return CEREMONY_COUNT;
}

pawukon_ceremony_t pawukon_ceremony_by_index(int index)
{
    if (index < 0 || index >= CEREMONY_COUNT) {
        pawukon_ceremony_t invalid = { NULL, NULL, -1, -1, -1 };
        return invalid;
    }
    return CEREMONIES[index];
}

int pawukon_days_until_wuku(double jd, int target_wuku)
{
    if (target_wuku < 0 || target_wuku >= PAWUKON_WUKU_COUNT) {
        return -1;
    }
    int current_day = pawukon_day_of_cycle(jd);
    int target_day = target_wuku * 7;
    int diff = target_day - current_day;
    if (diff < 0) {
        diff += PAWUKON_CYCLE_DAYS;
    }
    return diff;
}
