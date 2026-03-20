/* surf_display.h — Display-ready surf forecast formatter.
 *
 * Takes raw data from tide_predict.h and surf_spots.h and produces
 * formatted, human-readable display strings for the UI.
 * Serves the surfer user persona: "when and where should I surf today?"
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_SURF_DISPLAY_H
#define TIME_SURF_DISPLAY_H

#define SD_LINE_MAX 256
#define SD_CARD_MAX 1024
#define SD_FORECAST_MAX 2048

/* Display-ready surf forecast for a single spot at a given time */
typedef struct {
    char spot_name[64];              /* "Pipeline" */
    char country[32];                /* "USA" */
    char tide_line[SD_LINE_MAX];     /* "Rising tide | 3.2h to high | Spring tide" */
    char quality_line[SD_LINE_MAX];  /* "Good conditions (72%)" */
    char best_time_line[SD_LINE_MAX]; /* "Best window: ~14:30 today" */
    char advice[SD_LINE_MAX];        /* "Mid-tide rising on reef — prime conditions" */
    double quality;                  /* 0.0-1.0 raw quality */
    int window_count;                /* surf windows in next 24h */
} sd_forecast_t;

/* Display-ready spot info card */
typedef struct {
    char name[64];
    char country[32];
    char wave_type[32];              /* "Reef break" */
    char best_tide[32];              /* "Low tide" */
    char best_season[32];            /* "Winter" */
    char description[SD_LINE_MAX];
    char coordinates[64];            /* "21.67°N, 158.05°W" */
} sd_spot_card_t;

/* Build display-ready forecast for a spot at a given time.
 * spot_index: 0..49. jd: Julian date.
 * Invalid spot: empty strings, quality=0. */
sd_forecast_t sd_forecast(int spot_index, double jd);

/* Build display-ready spot info card.
 * Invalid spot: empty strings. */
sd_spot_card_t sd_spot_card(int spot_index);

/* Format forecast as multi-line display text.
 * Returns chars written (excluding null), or 0 on error. */
int sd_format_forecast(const sd_forecast_t *f, char *buf, int buf_size);

/* Format spot card as multi-line display text.
 * Returns chars written (excluding null), or 0 on error. */
int sd_format_card(const sd_spot_card_t *card, char *buf, int buf_size);

/* Quality description from 0.0-1.0:
 * <0.2 "Poor", <0.4 "Fair", <0.6 "Good", <0.8 "Excellent", >=0.8 "Epic" */
const char *sd_quality_label(double quality);

/* Tide state description from tide_predict data.
 * Format: "{Rising/Falling} tide | {N.N}h to {high/low} | {type} tide"
 * Returns chars written (excluding null), or 0 on error. */
int sd_format_tide(double height, int rising, int is_spring, int is_neap,
                   int is_king, double hours_to_high, double hours_to_low,
                   char *buf, int buf_size);

#endif /* TIME_SURF_DISPLAY_H */
