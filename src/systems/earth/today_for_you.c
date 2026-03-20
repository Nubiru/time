/* today_for_you.c — Daily briefing card composer.
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "today_for_you.h"

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

/* --- Helper: check if today is the birthday --- */

static int is_birthday(int days_until_birthday) {
    return days_until_birthday == 0 ||
           days_until_birthday == 365 ||
           days_until_birthday == 366;
}

/* --- Helper: compute energy level --- */

static tfy_energy_t compute_energy(const tfy_input_t *input) {
    int rc = input->resonance_count;
    int sr = input->strongest_resonance;
    int mc = input->milestone_count;

    // Birthday is extraordinary
    if (is_birthday(input->days_until_birthday)) {
        return TFY_ENERGY_EXTRAORDINARY;
    }

    // Strength 5 = kin return = extraordinary
    if (sr >= 5) {
        return TFY_ENERGY_EXTRAORDINARY;
    }

    // Strength 4 OR 2+ milestones = intense
    if (sr >= 4 || mc >= 2) {
        return TFY_ENERGY_INTENSE;
    }

    // 2+ resonances OR 1 resonance with strength 3 = active
    if (rc >= 2 || (rc >= 1 && sr >= 3)) {
        return TFY_ENERGY_ACTIVE;
    }

    // 1 resonance (strength <= 2) OR 0 resonances but 1 milestone = gentle
    if ((rc == 1 && sr <= 2) || (rc == 0 && mc >= 1)) {
        return TFY_ENERGY_GENTLE;
    }

    return TFY_ENERGY_QUIET;
}

/* --- Helper: find best milestone index (highest score), or -1 --- */

static int best_milestone(const tfy_input_t *input) {
    int best = -1;
    double best_score = -1.0;
    for (int i = 0; i < input->milestone_count && i < 4; i++) {
        if (input->milestones[i].score > best_score) {
            best_score = input->milestones[i].score;
            best = i;
        }
    }
    return best;
}

/* --- Helper: find best resonance index (highest strength), or -1 --- */

static int best_resonance(const tfy_input_t *input) {
    int best = -1;
    int best_str = 0;
    for (int i = 0; i < input->resonance_count && i < TFY_MAX_HIGHLIGHTS; i++) {
        if (input->resonances[i].strength > best_str) {
            best_str = input->resonances[i].strength;
            best = i;
        }
    }
    return best;
}

/* --- Helper: find soonest transitioning cycle index, or -1 --- */

static int soonest_cycle(const tfy_input_t *input) {
    int best = -1;
    int best_days = 9999;
    for (int i = 0; i < input->cycle_count && i < TFY_MAX_CYCLES; i++) {
        if (input->cycles[i].days_remaining <= 3 &&
            input->cycles[i].days_remaining < best_days) {
            best_days = input->cycles[i].days_remaining;
            best = i;
        }
    }
    return best;
}

/* --- Helper: set headline --- */

static void set_headline(tfy_briefing_t *b, const tfy_input_t *input) {
    // Priority: milestone > resonance > cycle transition > quiet
    int mi = best_milestone(input);
    if (mi >= 0) {
        safe_copy(b->headline, input->milestones[mi].headline,
                  TFY_HEADLINE_MAX);
        return;
    }

    int ri = best_resonance(input);
    if (ri >= 0) {
        safe_copy(b->headline, input->resonances[ri].description,
                  TFY_HEADLINE_MAX);
        return;
    }

    int ci = soonest_cycle(input);
    if (ci >= 0) {
        snprintf(b->headline, TFY_HEADLINE_MAX,
                 "%s: %s — %d day%s left",
                 input->cycles[ci].name,
                 input->cycles[ci].value,
                 input->cycles[ci].days_remaining,
                 input->cycles[ci].days_remaining == 1 ? "" : "s");
        return;
    }

    safe_copy(b->headline, "A quiet day in the cycles.", TFY_HEADLINE_MAX);
}

/* --- Helper: build highlights (merge milestones + resonances, sort desc) --- */

static void build_highlights(tfy_briefing_t *b, const tfy_input_t *input) {
    int count = 0;

    // Add milestones (type=1)
    for (int i = 0; i < input->milestone_count && i < 4 &&
         count < TFY_MAX_HIGHLIGHTS; i++) {
        tfy_highlight_t *h = &b->highlights[count];
        safe_copy(h->text, input->milestones[i].headline, TFY_HEADLINE_MAX);
        // Map score 0.0-1.0 to strength 1-5
        int s = (int)(input->milestones[i].score * 5.0);
        if (s < 1) s = 1;
        if (s > 5) s = 5;
        h->strength = s;
        h->type = 1;
        count++;
    }

    // Add resonances (type=0)
    for (int i = 0; i < input->resonance_count && i < TFY_MAX_HIGHLIGHTS &&
         count < TFY_MAX_HIGHLIGHTS; i++) {
        tfy_highlight_t *h = &b->highlights[count];
        safe_copy(h->text, input->resonances[i].description, TFY_HEADLINE_MAX);
        h->strength = input->resonances[i].strength;
        h->type = 0;
        count++;
    }

    // Simple insertion sort by strength descending
    for (int i = 1; i < count; i++) {
        tfy_highlight_t tmp = b->highlights[i];
        int j = i - 1;
        while (j >= 0 && b->highlights[j].strength < tmp.strength) {
            b->highlights[j + 1] = b->highlights[j];
            j--;
        }
        b->highlights[j + 1] = tmp;
    }

    b->highlight_count = count;
}

/* --- Helper: build cycles --- */

static void build_cycles(tfy_briefing_t *b, const tfy_input_t *input) {
    int count = 0;
    for (int i = 0; i < input->cycle_count && i < TFY_MAX_CYCLES; i++) {
        tfy_cycle_t *c = &b->cycles[count];
        safe_copy(c->label, input->cycles[i].name, TFY_LABEL_MAX);
        safe_copy(c->value, input->cycles[i].value, TFY_VALUE_MAX);
        c->progress = input->cycles[i].progress;
        c->days_remaining = input->cycles[i].days_remaining;
        c->transition_soon = (input->cycles[i].days_remaining <= 3) ? 1 : 0;
        count++;
    }
    b->cycle_count = count;
}

/* --- Public API --- */

tfy_input_t tfy_default_input(void) {
    tfy_input_t input;
    memset(&input, 0, sizeof(input));
    return input;
}

tfy_briefing_t tfy_compose(const tfy_input_t *input) {
    tfy_briefing_t b;
    memset(&b, 0, sizeof(b));

    if (!input) {
        return b;
    }

    // Greeting
    if (is_birthday(input->days_until_birthday)) {
        safe_copy(b.greeting, "Happy birthday!", TFY_GREETING_MAX);
    } else {
        safe_copy(b.greeting, tfy_greeting_for_hour(input->hour),
                  TFY_GREETING_MAX);
    }

    // Energy
    b.energy = compute_energy(input);

    // Headline
    set_headline(&b, input);

    // Highlights
    build_highlights(&b, input);

    // Cycles
    build_cycles(&b, input);

    // Birthday countdown
    b.days_until_birthday = input->days_until_birthday;
    b.show_birthday_countdown = (input->days_until_birthday > 0 &&
                                 input->days_until_birthday <= 30) ? 1 : 0;

    return b;
}

const char *tfy_greeting_for_hour(int hour) {
    if (hour >= 5 && hour <= 11) {
        return "Good morning";
    }
    if (hour >= 12 && hour <= 17) {
        return "Good afternoon";
    }
    return "Good evening";
}

const char *tfy_energy_name(tfy_energy_t energy) {
    switch (energy) {
        case TFY_ENERGY_QUIET:         return "Quiet";
        case TFY_ENERGY_GENTLE:        return "Gentle";
        case TFY_ENERGY_ACTIVE:        return "Active";
        case TFY_ENERGY_INTENSE:       return "Intense";
        case TFY_ENERGY_EXTRAORDINARY: return "Extraordinary";
        default:                       return "?";
    }
}

int tfy_highlight_count(const tfy_briefing_t *b) {
    return b->highlight_count;
}

int tfy_cycle_count(const tfy_briefing_t *b) {
    return b->cycle_count;
}

int tfy_format(const tfy_briefing_t *b, char *buf, int buf_size) {
    if (buf_size <= 0) return 0;

    int written = snprintf(buf, (size_t)buf_size,
        "%s\n\n"
        "%s\n\n"
        "Energy: %s",
        b->greeting,
        b->headline,
        tfy_energy_name(b->energy));

    if (written < 0) {
        buf[0] = '\0';
        return 0;
    }

    int pos = (written >= buf_size) ? buf_size - 1 : written;

    // Append highlights
    for (int i = 0; i < b->highlight_count && pos < buf_size - 1; i++) {
        int n = snprintf(buf + pos, (size_t)(buf_size - pos),
                         "\n- %s", b->highlights[i].text);
        if (n < 0) break;
        if (n >= buf_size - pos) {
            pos = buf_size - 1;
            break;
        }
        pos += n;
    }

    // Append cycles
    for (int i = 0; i < b->cycle_count && pos < buf_size - 1; i++) {
        int n = snprintf(buf + pos, (size_t)(buf_size - pos),
                         "\n[%s] %s (%d days left%s)",
                         b->cycles[i].label,
                         b->cycles[i].value,
                         b->cycles[i].days_remaining,
                         b->cycles[i].transition_soon ? " !" : "");
        if (n < 0) break;
        if (n >= buf_size - pos) {
            pos = buf_size - 1;
            break;
        }
        pos += n;
    }

    // Birthday countdown
    if (b->show_birthday_countdown && pos < buf_size - 1) {
        int n = snprintf(buf + pos, (size_t)(buf_size - pos),
                         "\nBirthday in %d day%s!",
                         b->days_until_birthday,
                         b->days_until_birthday == 1 ? "" : "s");
        if (n > 0 && n < buf_size - pos) {
            pos += n;
        } else if (n >= buf_size - pos) {
            pos = buf_size - 1;
        }
    }

    if (written >= buf_size) {
        return buf_size - 1;
    }
    return pos;
}
