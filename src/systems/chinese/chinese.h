#ifndef TIME_CHINESE_H
#define TIME_CHINESE_H

/* Chinese sexagenary cycle result for a Gregorian year. */
typedef struct {
    int stem;       /* 0-9 (Heavenly Stem index) */
    int branch;     /* 0-11 (Earthly Branch index) */
    int animal;     /* 0-11 (same as branch — Rat=0, Ox=1, ...) */
    int element;    /* 0-4 (Wood=0, Fire=1, Earth=2, Metal=3, Water=4) */
    int polarity;   /* 0=Yang, 1=Yin */
    int cycle_year; /* 1-60 within the sexagenary cycle */
} chinese_year_t;

/* Compute Chinese year from Gregorian year. */
chinese_year_t chinese_year_from_gregorian(int year);

/* Compute Chinese year from Julian Day (extracts year via jd_to_gregorian). */
chinese_year_t chinese_year_from_jd(double jd);

/* Heavenly Stem name (0-9). Returns "?" for invalid. */
const char *chinese_stem_name(int stem);

/* Earthly Branch name (0-11). Returns "?" for invalid. */
const char *chinese_branch_name(int branch);

/* Zodiac animal name (0-11). Returns "?" for invalid. */
const char *chinese_animal_name(int animal);

/* Element name (0-4). Returns "?" for invalid. */
const char *chinese_element_name(int element);

/* Polarity name (0=Yang, 1=Yin). Returns "?" for invalid. */
const char *chinese_polarity_name(int polarity);

/* Unicode animal emoji (0-11). Returns "?" for invalid. */
const char *chinese_animal_symbol(int animal);

#endif
