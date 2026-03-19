/* welcome_text.h -- Welcome Journey text content generator.
 *
 * Generates personalized onboarding text for each screen of the
 * welcome journey, using birth_profile_t data. Text follows the
 * Time voice guidelines: cosmic wonder, personal discovery, not
 * clinical data entry.
 *
 * All text uses static buffers (not thread-safe). Each function
 * returns a pointer to a static internal buffer.
 *
 * Pure module: no globals, no malloc, no GL. */

#ifndef TIME_WELCOME_TEXT_H
#define TIME_WELCOME_TEXT_H

#include "../systems/unified/birth_profile.h"

#define WT_BUF_SIZE 512

/* Screen identifiers for the welcome journey */
typedef enum {
    WT_SCREEN_INTRO = 0,       /* "When did you arrive on this planet?" */
    WT_SCREEN_LOCATION,        /* "Where on Earth were you standing?" */
    WT_SCREEN_REVEAL_TZOLKIN,  /* Per-system birth reveal */
    WT_SCREEN_REVEAL_CHINESE,
    WT_SCREEN_REVEAL_HEBREW,
    WT_SCREEN_REVEAL_ISLAMIC,
    WT_SCREEN_REVEAL_BUDDHIST,
    WT_SCREEN_REVEAL_ICHING,
    WT_SCREEN_REVEAL_ASTROLOGY,
    WT_SCREEN_SUMMARY,         /* Final: "Welcome to Time" */
    WT_SCREEN_COUNT
} wt_screen_t;

/* Get the title text for a welcome screen.
 * Returns pointer to static string. */
const char *wt_screen_title(wt_screen_t screen);

/* Get the subtitle/prompt text for a welcome screen.
 * Returns pointer to static string. */
const char *wt_screen_subtitle(wt_screen_t screen);

/* Generate a personalized reveal sentence for one system.
 * Example: "In the Tzolkin, you are Kin 144: Yellow Magnetic Seed."
 * Returns pointer to static buffer (overwritten each call).
 * Returns empty string for NULL profile or invalid screen. */
const char *wt_reveal_text(wt_screen_t screen, const birth_profile_t *profile);

/* Generate a multi-line welcome summary combining all systems.
 * Returns pointer to static buffer (overwritten each call).
 * Uses a larger buffer (WT_BUF_SIZE * 4). */
const char *wt_summary_text(const birth_profile_t *profile);

/* How many reveal screens are there? (7: tzolkin through astrology) */
int wt_reveal_screen_count(void);

/* Is this screen a reveal screen? */
int wt_is_reveal_screen(wt_screen_t screen);

/* Get the screen name for display/debug. Returns "Unknown" for invalid. */
const char *wt_screen_name(wt_screen_t screen);

#endif /* TIME_WELCOME_TEXT_H */
