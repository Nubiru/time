/* author_card.h — Author Card pure data layer.
 *
 * Provides display-ready content for the "Get to Know the Author"
 * profile card: Gabriel's identity, project philosophy, tech stack,
 * and key contributor credits.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_AUTHOR_CARD_H
#define TIME_AUTHOR_CARD_H

#define AC_NAME_MAX 64
#define AC_TITLE_MAX 64
#define AC_STATEMENT_MAX 512
#define AC_TECH_MAX 128
#define AC_LINE_MAX 128
#define AC_MAX_CREDITS 8

/* A credit line (for contributors section) */
typedef struct {
    char role[AC_TITLE_MAX];       /* "AI Collaborator", "Astronomical Algorithms", etc. */
    char name[AC_NAME_MAX];        /* "Claude", "Jean Meeus", etc. */
} ac_credit_t;

/* The author card data */
typedef struct {
    char name[AC_NAME_MAX];              /* "Gabriel" */
    char title[AC_TITLE_MAX];            /* "Creator of Time" */
    char statement[AC_STATEMENT_MAX];    /* Gabriel's personal statement */
    char tech_line[AC_TECH_MAX];         /* "C · WebGL2 · WebAssembly · Every pixel earned" */
    char philosophy[AC_STATEMENT_MAX];   /* The project philosophy */
    ac_credit_t credits[AC_MAX_CREDITS]; /* Key credits */
    int credit_count;
    int contributor_count;               /* Total from contributors.json (passed in) */
} ac_card_t;

/* Build the author card. contributor_count is the total from contributors.json. */
ac_card_t ac_build(int contributor_count);

/* Get author name. */
const char *ac_name(const ac_card_t *card);

/* Get author title. */
const char *ac_title(const ac_card_t *card);

/* Get personal statement. */
const char *ac_statement(const ac_card_t *card);

/* Get tech stack line. */
const char *ac_tech(const ac_card_t *card);

/* Get philosophy text. */
const char *ac_philosophy(const ac_card_t *card);

/* Get credit by index. Returns NULL if out of range. */
const ac_credit_t *ac_credit(const ac_card_t *card, int index);

/* Get credit count. */
int ac_credit_count(const ac_card_t *card);

/* Format contributor acknowledgment line: "Built with contributions from N humans".
 * Returns number of characters written (excluding null), or 0 on error. */
int ac_format_contributors(const ac_card_t *card, char *buf, int buf_size);

/* Format full card as multi-line text for display/sharing.
 * Returns number of characters written (excluding null), or 0 on error. */
int ac_format_full(const ac_card_t *card, char *buf, int buf_size);

#endif /* TIME_AUTHOR_CARD_H */
