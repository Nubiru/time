#ifndef TIME_ZODIAC_H
#define TIME_ZODIAC_H

/* Zodiac sign index from ecliptic longitude (0-360 degrees).
 * Returns 0=Aries, 1=Taurus, ..., 11=Pisces. */
int zodiac_sign(double longitude);

/* Sign name from index (0=Aries ... 11=Pisces).
 * Returns "?" for invalid index. */
const char *zodiac_sign_name(int sign);

/* Degree within sign (0-30) from ecliptic longitude. */
double zodiac_degree(double longitude);

/* Unicode symbol for zodiac sign index.
 * Returns "?" for invalid index. */
const char *zodiac_sign_symbol(int sign);

/* Decan (1, 2, or 3) from ecliptic longitude.
 * Decan 1: 0-10 deg, Decan 2: 10-20 deg, Decan 3: 20-30 deg within sign. */
int zodiac_decan(double longitude);

#endif
