/* human_population.c — Human population through history.
 * Pure functions only. No malloc, no globals, no side effects.
 * Data source: Population Reference Bureau (2023), UN estimates. */

#include "human_population.h"

#include <stdio.h>
#include <string.h>

/* ===== Static Data ===== */

static const pop_milestone_t MILESTONES[POP_MILESTONE_COUNT] = {
    { 1, 1804, 123, "Industrial revolution" },
    { 2, 1927,  33, "Post-WWI recovery" },
    { 3, 1960,  14, "Green Revolution begins" },
    { 4, 1974,  13, "Global modernization" },
    { 5, 1987,  12, "Medical advances" },
    { 6, 1999,  12, "Turn of millennium" },
    { 7, 2011,  11, "Digital age" },
    { 8, 2022,   0, "Post-pandemic" }
};

static const pop_datapoint_t DATAPOINTS[POP_DATAPOINT_COUNT] = {
    { -10000,    5.0, "Pre-agriculture",      "Hunter-gatherer societies" },
    {  -5000,   18.0, "Early agriculture",     "Farming settlements spread" },
    {  -3000,   45.0, "Bronze Age",            "First cities and writing" },
    {  -1000,   72.0, "Iron Age",              "Mediterranean civilizations" },
    {   -500,  100.0, "Classical Antiquity",    "Greek golden age" },
    {      1,  300.0, "Roman Empire",           "Pax Romana begins" },
    {    500,  210.0, "Post-Roman",             "Fall of Western Rome" },
    {   1000,  310.0, "Medieval",               "Agricultural improvements" },
    {   1200,  360.0, "High Middle Ages",       "Cathedral building era" },
    {   1340,  443.0, "Pre-plague",             "Peak medieval population" },
    {   1400,  350.0, "Post-plague",            "Black Death aftermath" },
    {   1500,  461.0, "Renaissance",            "Age of exploration" },
    {   1600,  554.0, "Early Modern",           "Scientific revolution" },
    {   1700,  603.0, "Enlightenment",          "Agricultural revolution" },
    {   1800,  989.0, "Industrial era",         "Industrial revolution" },
    {   1804, 1000.0, "1 billion milestone",    "First billion reached" },
    {   1900, 1600.0, "Modern era",             "Urbanization accelerates" },
    {   1927, 2000.0, "2 billion milestone",    "Interwar period" },
    {   1950, 2500.0, "Post-WWII",             "Baby boom begins" },
    {   1960, 3000.0, "3 billion milestone",    "Green Revolution" },
    {   1974, 4000.0, "4 billion milestone",    "Global development" },
    {   1987, 5000.0, "5 billion milestone",    "Information age" },
    {   1999, 6000.0, "6 billion milestone",    "Internet era" },
    {   2011, 7000.0, "7 billion milestone",    "Smartphone revolution" },
    {   2022, 8000.0, "8 billion milestone",    "Post-pandemic recovery" },
    {   2024, 8100.0, "8 billion era",          "AI and climate awareness" }
};

/* Ever-born anchor points: (year, cumulative billions born).
 * Based on PRB estimates and methodology. */
#define EVER_BORN_ANCHOR_COUNT 7

static const int EVER_BORN_YEARS[EVER_BORN_ANCHOR_COUNT] = {
    -10000, 1, 1200, 1800, 1900, 2000, 2024
};

static const double EVER_BORN_VALUES[EVER_BORN_ANCHOR_COUNT] = {
    5.0, 36.0, 55.0, 73.0, 83.0, 100.0, 109.0
};

/* ===== Helper: linear interpolation in a table ===== */

static double interp_table(const int *years, const double *values,
                           int count, int year)
{
    /* Before first point */
    if (year <= years[0]) {
        return values[0];
    }
    /* After last point */
    if (year >= years[count - 1]) {
        return values[count - 1];
    }
    /* Find bracketing interval */
    for (int i = 1; i < count; i++) {
        if (year <= years[i]) {
            double t = (double)(year - years[i - 1]) /
                       (double)(years[i] - years[i - 1]);
            return values[i - 1] + t * (values[i] - values[i - 1]);
        }
    }
    return values[count - 1];
}

/* ===== Public API ===== */

pop_milestone_t pop_milestone_get(int index)
{
    if (index < 0 || index >= POP_MILESTONE_COUNT) {
        pop_milestone_t empty = { 0, 0, 0, NULL };
        return empty;
    }
    return MILESTONES[index];
}

int pop_milestone_count(void)
{
    return POP_MILESTONE_COUNT;
}

pop_datapoint_t pop_datapoint_get(int index)
{
    if (index < 0 || index >= POP_DATAPOINT_COUNT) {
        pop_datapoint_t empty = { 0, 0.0, NULL, NULL };
        return empty;
    }
    return DATAPOINTS[index];
}

int pop_datapoint_count(void)
{
    return POP_DATAPOINT_COUNT;
}

double pop_alive_at_year(int year)
{
    /* Before earliest data: ~1 million */
    if (year < -10000) {
        return 1.0;
    }

    /* After latest data: conservative linear extrapolation */
    if (year > 2024) {
        /* ~80 million/year growth rate (UN medium variant ~2020s) */
        double years_past = (double)(year - 2024);
        return 8100.0 + years_past * 80.0;
    }

    /* Interpolate between data points */
    for (int i = 1; i < POP_DATAPOINT_COUNT; i++) {
        if (year <= DATAPOINTS[i].year) {
            double t = (double)(year - DATAPOINTS[i - 1].year) /
                       (double)(DATAPOINTS[i].year - DATAPOINTS[i - 1].year);
            return DATAPOINTS[i - 1].population +
                   t * (DATAPOINTS[i].population - DATAPOINTS[i - 1].population);
        }
    }

    return DATAPOINTS[POP_DATAPOINT_COUNT - 1].population;
}

double pop_ever_born_at_year(int year)
{
    /* Before earliest anchor: scale proportionally */
    if (year < EVER_BORN_YEARS[0]) {
        /* Very rough: 1 billion per 10000 years pre-agriculture */
        double years_before = (double)(EVER_BORN_YEARS[0] - year);
        double fraction = years_before / 50000.0;
        return EVER_BORN_VALUES[0] * (1.0 - fraction);
    }

    return interp_table(EVER_BORN_YEARS, EVER_BORN_VALUES,
                        EVER_BORN_ANCHOR_COUNT, year);
}

pop_summary_t pop_summary(int year)
{
    pop_summary_t s;
    memset(&s, 0, sizeof(s));

    s.alive_millions = pop_alive_at_year(year);
    s.ever_born_billions = pop_ever_born_at_year(year);

    /* Find nearest milestone at or below current population */
    s.nearest_milestone_billions = 0;
    for (int i = 0; i < POP_MILESTONE_COUNT; i++) {
        double milestone_pop = (double)MILESTONES[i].billions * 1000.0;
        if (s.alive_millions >= milestone_pop) {
            s.nearest_milestone_billions = MILESTONES[i].billions;
        }
    }

    /* Find era name from nearest data point */
    s.era_name = "Unknown";
    for (int i = 0; i < POP_DATAPOINT_COUNT; i++) {
        if (DATAPOINTS[i].year <= year) {
            s.era_name = DATAPOINTS[i].era;
        }
    }

    /* Format summary string */
    char alive_buf[64];
    char born_buf[64];
    pop_format_number(s.alive_millions, alive_buf, 64);
    snprintf(born_buf, sizeof(born_buf), "%.0f", s.ever_born_billions);
    snprintf(s.summary, sizeof(s.summary),
             "~%s alive, %s billion ever born", alive_buf, born_buf);

    return s;
}

int pop_format_number(double millions, char *buf, int buf_size)
{
    if (buf_size <= 0) {
        return 0;
    }

    if (millions >= 1000.0) {
        /* Format as billions */
        double billions = millions / 1000.0;
        return snprintf(buf, (unsigned long)buf_size,
                        "%.1f billion", billions);
    }

    if (millions >= 1.0) {
        /* Format as whole millions */
        return snprintf(buf, (unsigned long)buf_size,
                        "%.0f million", millions);
    }

    /* Sub-million: show as thousands */
    double thousands = millions * 1000.0;
    return snprintf(buf, (unsigned long)buf_size,
                    "%.0f thousand", thousands);
}

int pop_year_of_billion(int n)
{
    if (n < 1 || n > POP_MILESTONE_COUNT) {
        return 0;
    }
    return MILESTONES[n - 1].year;
}
