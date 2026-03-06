/* yuga.c -- Hindu Yuga cycle data and position calculations.
 * Data source: Surya Siddhanta Ch. 12.
 * All pure functions: no malloc, no globals, no side effects. */

#include "yuga.h"
#include <math.h>

/* Static data for the 4 Yugas */
static const yuga_t YUGAS[YUGA_COUNT] = {
    {
        .id = YUGA_SATYA,
        .name = "Satya",
        .meaning = "Truth/Golden Age",
        .description = "Age of truth and perfection; dharma stands on four legs",
        .years = SATYA_YUGA_YEARS,
        .ratio = 4,
        .dharma_fraction = 1.0
    },
    {
        .id = YUGA_TRETA,
        .name = "Treta",
        .meaning = "Third/Silver Age",
        .description = "Age of three-quarter virtue; dharma stands on three legs",
        .years = TRETA_YUGA_YEARS,
        .ratio = 3,
        .dharma_fraction = 0.75
    },
    {
        .id = YUGA_DVAPARA,
        .name = "Dvapara",
        .meaning = "Second/Bronze Age",
        .description = "Age of half virtue; dharma stands on two legs",
        .years = DVAPARA_YUGA_YEARS,
        .ratio = 2,
        .dharma_fraction = 0.5
    },
    {
        .id = YUGA_KALI,
        .name = "Kali",
        .meaning = "Strife/Iron Age",
        .description = "Age of strife and discord; dharma stands on one leg",
        .years = KALI_YUGA_YEARS,
        .ratio = 1,
        .dharma_fraction = 0.25
    }
};

/* Yuga order within a Maha-Yuga: Satya -> Treta -> Dvapara -> Kali */
static const int YUGA_ORDER[YUGA_COUNT] = {
    YUGA_SATYA, YUGA_TRETA, YUGA_DVAPARA, YUGA_KALI
};


yuga_t yuga_get(int index)
{
    if (index < 0 || index >= YUGA_COUNT) {
        yuga_t invalid = {
            .id = -1,
            .name = "?",
            .meaning = "?",
            .description = "",
            .years = 0,
            .ratio = 0,
            .dharma_fraction = 0.0
        };
        return invalid;
    }
    return YUGAS[index];
}

const char *yuga_name(int index)
{
    if (index < 0 || index >= YUGA_COUNT) {
        return "?";
    }
    return YUGAS[index].name;
}

const char *yuga_meaning(int index)
{
    if (index < 0 || index >= YUGA_COUNT) {
        return "?";
    }
    return YUGAS[index].meaning;
}

yuga_position_t yuga_position(long long years_since_epoch)
{
    yuga_position_t pos;
    pos.yuga_index = YUGA_KALI;
    pos.years_elapsed = 0;
    pos.years_remaining = 0;
    pos.fraction = 0.0;
    pos.maha_yuga_number = 1;

    if (years_since_epoch < KALI_YUGA_YEARS) {
        /* Still within the initial Kali Yuga */
        pos.yuga_index = YUGA_KALI;
        pos.years_elapsed = years_since_epoch;
        pos.years_remaining = KALI_YUGA_YEARS - years_since_epoch;
        pos.fraction = (double)years_since_epoch / (double)KALI_YUGA_YEARS;
        pos.maha_yuga_number = 1;
        return pos;
    }

    /* Past the initial Kali Yuga: subsequent full Maha-Yuga cycles */
    long long remaining = years_since_epoch - KALI_YUGA_YEARS;
    long long maha_yuga_count = remaining / MAHA_YUGA_YEARS;
    long long within_maha = remaining % MAHA_YUGA_YEARS;

    pos.maha_yuga_number = (int)(maha_yuga_count + 2);

    /* Find which Yuga within this Maha-Yuga */
    for (int i = 0; i < YUGA_COUNT; i++) {
        int yi = YUGA_ORDER[i];
        long long yuga_len = YUGAS[yi].years;
        if (within_maha < yuga_len) {
            pos.yuga_index = yi;
            pos.years_elapsed = within_maha;
            pos.years_remaining = yuga_len - within_maha;
            pos.fraction = (double)within_maha / (double)yuga_len;
            return pos;
        }
        within_maha -= yuga_len;
    }

    /* Should not reach here, but handle gracefully */
    return pos;
}

yuga_position_t yuga_position_from_jd(double jd)
{
    double years_f = (jd - KALI_YUGA_EPOCH_JD) / 365.25;
    long long years_since = (long long)floor(years_f);
    if (years_since < 0) {
        years_since = 0;
    }
    return yuga_position(years_since);
}

yuga_position_t yuga_for_year(int ce_year)
{
    long long years_since = (long long)ce_year - (long long)KALI_YUGA_EPOCH_CE;
    if (years_since < 0) {
        years_since = 0;
    }
    return yuga_position(years_since);
}

long long yuga_maha_yuga_years(void)
{
    return MAHA_YUGA_YEARS;
}

long long yuga_kalpa_years(void)
{
    return KALPA_YEARS;
}

long long yuga_manvantara_years(void)
{
    return (long long)MANVANTARA_YUGAS * MAHA_YUGA_YEARS;
}

double yuga_dharma(int yuga_index)
{
    if (yuga_index < 0 || yuga_index >= YUGA_COUNT) {
        return 0.0;
    }
    return YUGAS[yuga_index].dharma_fraction;
}

int yuga_count(void)
{
    return YUGA_COUNT;
}
