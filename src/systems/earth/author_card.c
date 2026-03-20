/* author_card.c — Author Card pure data layer.
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "author_card.h"

#include <stdio.h>
#include <string.h>

/* --- Helper: safe string copy with null termination --- */

static void safe_copy(char *dst, const char *src, int max_len) {
    int i = 0;
    while (i < max_len - 1 && src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

/* --- Helper: add a credit entry --- */

static void add_credit(ac_card_t *card, const char *name, const char *role) {
    if (card->credit_count >= AC_MAX_CREDITS) return;
    ac_credit_t *c = &card->credits[card->credit_count];
    safe_copy(c->name, name, AC_NAME_MAX);
    safe_copy(c->role, role, AC_TITLE_MAX);
    card->credit_count++;
}

/* --- Build --- */

ac_card_t ac_build(int contributor_count) {
    ac_card_t card;
    memset(&card, 0, sizeof(card));

    safe_copy(card.name, "Gabriel", AC_NAME_MAX);
    safe_copy(card.title, "Creator of Time", AC_TITLE_MAX);

    safe_copy(card.statement,
        "Time is not a clock app. It is an artwork that reveals the hidden "
        "connections in the fabric of reality. Every calendar system, every "
        "astronomical cycle, every cultural tradition of measuring time is "
        "a lens on the same underlying truth.",
        AC_STATEMENT_MAX);

    safe_copy(card.tech_line,
        "C \xc2\xb7 WebGL2 \xc2\xb7 WebAssembly \xc2\xb7 Every pixel earned",
        AC_TECH_MAX);

    safe_copy(card.philosophy,
        "No frameworks. No engines. No abstractions we don't understand. "
        "The code is part of the artwork.",
        AC_STATEMENT_MAX);

    card.contributor_count = contributor_count;

    /* Key credits */
    add_credit(&card, "Claude", "AI Collaborator");
    add_credit(&card, "Jean Meeus", "Astronomical Algorithms");
    add_credit(&card, "Jos\xc3\xa9 Arg\xc3\xbc""elles",
               "Dreamspell / 13:20 Frequency");
    add_credit(&card, "Richard Wilhelm", "I Ching Translation");
    add_credit(&card, "Edward Reingold & Nachum Dershowitz",
               "Calendrical Calculations");

    return card;
}

/* --- Accessors --- */

const char *ac_name(const ac_card_t *card) {
    return card->name;
}

const char *ac_title(const ac_card_t *card) {
    return card->title;
}

const char *ac_statement(const ac_card_t *card) {
    return card->statement;
}

const char *ac_tech(const ac_card_t *card) {
    return card->tech_line;
}

const char *ac_philosophy(const ac_card_t *card) {
    return card->philosophy;
}

const ac_credit_t *ac_credit(const ac_card_t *card, int index) {
    if (index < 0 || index >= card->credit_count) return NULL;
    return &card->credits[index];
}

int ac_credit_count(const ac_card_t *card) {
    return card->credit_count;
}

/* --- Formatters --- */

int ac_format_contributors(const ac_card_t *card, char *buf, int buf_size) {
    if (buf_size <= 0) return 0;

    int written = snprintf(buf, (size_t)buf_size,
        "Built with contributions from %d humans",
        card->contributor_count);

    if (written < 0) {
        buf[0] = '\0';
        return 0;
    }
    if (written >= buf_size) {
        return buf_size - 1;
    }
    return written;
}

int ac_format_full(const ac_card_t *card, char *buf, int buf_size) {
    if (buf_size <= 0) return 0;

    int written = snprintf(buf, (size_t)buf_size,
        "%s — %s\n\n"
        "%s\n\n"
        "%s\n\n"
        "%s\n\n"
        "Built with contributions from %d humans",
        card->name, card->title,
        card->statement,
        card->tech_line,
        card->philosophy,
        card->contributor_count);

    if (written < 0) {
        buf[0] = '\0';
        return 0;
    }
    if (written >= buf_size) {
        return buf_size - 1;
    }
    return written;
}
