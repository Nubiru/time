/* time_philosophy.c -- Philosophy of Time Database.
 *
 * 30 philosophical perspectives on time spanning 11 traditions: Greek,
 * Christian, Islamic, Indian, Buddhist, Chinese, Jewish, Persian, Mayan,
 * Modern, and Scientific.
 *
 * All functions are pure: no globals, no malloc, no side effects.
 * Case-insensitive search uses tolower() character-by-character. */

#include "time_philosophy.h"

#include <ctype.h>
#include <string.h>

/* ---------- helpers ---------- */

/* Case-insensitive substring search (C11-portable, no strcasecmp). */
static int tp_ci_contains(const char *haystack, const char *needle)
{
    if (haystack == NULL || needle == NULL) return 0;
    int hlen = (int)strlen(haystack);
    int nlen = (int)strlen(needle);
    if (nlen == 0) return 1;
    if (nlen > hlen) return 0;
    for (int i = 0; i <= hlen - nlen; i++) {
        int match = 1;
        for (int j = 0; j < nlen; j++) {
            if (tolower((unsigned char)haystack[i + j]) !=
                tolower((unsigned char)needle[j])) {
                match = 0;
                break;
            }
        }
        if (match) return 1;
    }
    return 0;
}

/* ---------- name tables ---------- */

static const char *TRADITION_NAMES[TP_TRADITION_COUNT] = {
    "Greek",
    "Roman",
    "Christian",
    "Islamic",
    "Indian",
    "Buddhist",
    "Chinese",
    "Mayan",
    "Jewish",
    "Persian",
    "Modern",
    "Scientific"
};

static const char *THEME_NAMES[TP_THEME_COUNT] = {
    "Nature of Time",
    "Cyclical",
    "Linear",
    "Subjective",
    "Cosmic",
    "Sacred",
    "Measurement",
    "Impermanence"
};

/* ---------- entry database ---------- */

static const tp_entry_t ENTRIES[] = {

    /* ===== GREEK (4 entries, indices 0-3) ===== */
    { 0,
      "Time is the moving image of eternity.",
      "Plato", "Timaeus", -360,
      TP_TRADITION_GREEK,
      { TP_THEME_NATURE_OF_TIME, TP_THEME_COSMIC, 0, 0 }, 2,
      "Plato's most influential statement on time, linking temporal "
      "change to eternal forms." },

    { 1,
      "Time is the number of motion in respect of before and after.",
      "Aristotle", "Physics IV.11", -350,
      TP_TRADITION_GREEK,
      { TP_THEME_MEASUREMENT, TP_THEME_NATURE_OF_TIME, 0, 0 }, 2,
      "Aristotle grounded time in observable change, making it "
      "measurable." },

    { 2,
      "No man ever steps in the same river twice, for it is not the "
      "same river and he is not the same man.",
      "Heraclitus", "Fragments", -500,
      TP_TRADITION_GREEK,
      { TP_THEME_IMPERMANENCE, TP_THEME_CYCLICAL, 0, 0 }, 2,
      "The earliest Greek philosopher of flux and change." },

    { 3,
      "What is, is uncreated and indestructible, alone, complete, "
      "immovable and without end.",
      "Parmenides", "On Nature", -475,
      TP_TRADITION_GREEK,
      { TP_THEME_NATURE_OF_TIME, TP_THEME_LINEAR, 0, 0 }, 2,
      "Parmenides denied change, arguing that time itself is an "
      "illusion." },

    /* ===== CHRISTIAN (2 entries, indices 4-5) ===== */
    { 4,
      "What then is time? If no one asks me, I know what it is. If I "
      "wish to explain it to him who asks, I do not know.",
      "Augustine", "Confessions XI", 400,
      TP_TRADITION_CHRISTIAN,
      { TP_THEME_NATURE_OF_TIME, TP_THEME_SUBJECTIVE, 0, 0 }, 2,
      "The most famous philosophical puzzle about time." },

    { 5,
      "Eternity is the simultaneously-whole and perfect possession of "
      "interminable life.",
      "Thomas Aquinas", "Summa Theologica", 1270,
      TP_TRADITION_CHRISTIAN,
      { TP_THEME_SACRED, TP_THEME_NATURE_OF_TIME, 0, 0 }, 2,
      "Aquinas synthesized Aristotle with Christian theology." },

    /* ===== ISLAMIC (3 entries, indices 6-8) ===== */
    { 6,
      "Time is a duration measured by motion.",
      "Al-Kindi", "On First Philosophy", 850,
      TP_TRADITION_ISLAMIC,
      { TP_THEME_MEASUREMENT, TP_THEME_NATURE_OF_TIME, 0, 0 }, 2,
      "The first Arab philosopher, transmitter of Greek thought to "
      "the Islamic world." },

    { 7,
      "Time is the breath of the Merciful.",
      "Ibn Arabi", "Fusus al-Hikam", 1200,
      TP_TRADITION_ISLAMIC,
      { TP_THEME_SACRED, TP_THEME_COSMIC, 0, 0 }, 2,
      "Ibn Arabi saw creation as continuously renewed at each "
      "moment." },

    { 8,
      "The Moving Finger writes; and, having writ, moves on.",
      "Omar Khayyam", "Rubaiyat", 1120,
      TP_TRADITION_ISLAMIC,
      { TP_THEME_LINEAR, TP_THEME_IMPERMANENCE, 0, 0 }, 2,
      "Khayyam was also a mathematician who reformed the Persian "
      "calendar." },

    /* ===== INDIAN (3 entries, indices 9-11) ===== */
    { 9,
      "I am Time, the great destroyer of worlds, here to annihilate "
      "the worlds.",
      "Bhagavad Gita", "Chapter 11:32", -200,
      TP_TRADITION_INDIAN,
      { TP_THEME_COSMIC, TP_THEME_SACRED, 0, 0 }, 2,
      "Krishna reveals time as the ultimate cosmic force." },

    { 10,
      "When the mind is stilled, the witness stands in its own "
      "nature.",
      "Patanjali", "Yoga Sutras I.3", -200,
      TP_TRADITION_INDIAN,
      { TP_THEME_SUBJECTIVE, TP_THEME_SACRED, 0, 0 }, 2,
      "Transcending time through present-moment awareness." },

    { 11,
      "Without a thing, of what will there be duration? With no "
      "duration, there will be no time.",
      "Nagarjuna", "Mulamadhyamakakarika XIX.6", 150,
      TP_TRADITION_INDIAN,
      { TP_THEME_NATURE_OF_TIME, TP_THEME_IMPERMANENCE, 0, 0 }, 2,
      "Time has no independent existence -- it depends on events." },

    /* ===== BUDDHIST (3 entries, indices 12-14) ===== */
    { 12,
      "The past is already gone, the future is not yet here. There is "
      "only one moment for you to live -- this present moment.",
      "Buddha", NULL, -500,
      TP_TRADITION_BUDDHIST,
      { TP_THEME_SUBJECTIVE, TP_THEME_IMPERMANENCE, 0, 0 }, 2,
      "The foundation of mindfulness practice." },

    { 13,
      "Being-time means that time is being. Every being in the entire "
      "world is each moment of time.",
      "Dogen", "Uji", 1240,
      TP_TRADITION_BUDDHIST,
      { TP_THEME_NATURE_OF_TIME, TP_THEME_CYCLICAL, 0, 0 }, 2,
      "Dogen dissolved the boundary between being and time." },

    { 14,
      "The present moment is the only moment available to us, and it "
      "is the door to all moments.",
      "Thich Nhat Hanh", NULL, 1966,
      TP_TRADITION_BUDDHIST,
      { TP_THEME_SUBJECTIVE, TP_THEME_SACRED, 0, 0 }, 2,
      "Modern Buddhist teacher who made mindfulness accessible to "
      "the West." },

    /* ===== CHINESE (3 entries, indices 15-17) ===== */
    { 15,
      "Time is a created thing. To say 'I don't have time' is like "
      "saying 'I don't want to.'",
      "Lao Tzu", NULL, -600,
      TP_TRADITION_CHINESE,
      { TP_THEME_SUBJECTIVE, TP_THEME_NATURE_OF_TIME, 0, 0 }, 2,
      "Taoist philosophy sees time as inseparable from the flow of "
      "the Tao." },

    { 16,
      "Flow with whatever may happen and let your mind be free. Stay "
      "centered by accepting whatever you are doing.",
      "Zhuangzi", "Zhuangzi", -300,
      TP_TRADITION_CHINESE,
      { TP_THEME_CYCLICAL, TP_THEME_SUBJECTIVE, 0, 0 }, 2,
      "Zhuangzi's philosophy of effortless action (wu wei) applied "
      "to time." },

    { 17,
      "Study the past if you would define the future.",
      "Confucius", "Analects", -500,
      TP_TRADITION_CHINESE,
      { TP_THEME_LINEAR, TP_THEME_MEASUREMENT, 0, 0 }, 2,
      "Confucian emphasis on learning from history to shape what "
      "comes next." },

    /* ===== JEWISH (2 entries, indices 18-19) ===== */
    { 18,
      "For everything there is a season, and a time for every purpose "
      "under heaven.",
      "Ecclesiastes", "Ecclesiastes 3:1", -300,
      TP_TRADITION_JEWISH,
      { TP_THEME_CYCLICAL, TP_THEME_SACRED, 0, 0 }, 2,
      "The most famous biblical meditation on cyclical time." },

    { 19,
      "He created His universe with three books: with text, with "
      "number, and with communication.",
      "Sefer Yetzirah", "Sefer Yetzirah", 200,
      TP_TRADITION_JEWISH,
      { TP_THEME_SACRED, TP_THEME_COSMIC, 0, 0 }, 2,
      "The foundational text of Kabbalah, which sees time as one of "
      "three dimensions of creation." },

    /* ===== PERSIAN (1 entry, index 20) ===== */
    { 20,
      "Time in the long dominion cleaves the world in two: the good "
      "and the evil.",
      "Zarathustra", "Bundahishn", -1000,
      TP_TRADITION_PERSIAN,
      { TP_THEME_COSMIC, TP_THEME_LINEAR, 0, 0 }, 2,
      "Zoroastrian dualism cast time as the arena of cosmic "
      "struggle." },

    /* ===== MAYAN (1 entry, index 21) ===== */
    { 21,
      "Time is not money. Time is art.",
      "Jose Arguelles", "The Mayan Factor", 1987,
      TP_TRADITION_MAYAN,
      { TP_THEME_SACRED, TP_THEME_NATURE_OF_TIME, 0, 0 }, 2,
      "Arguelles challenged industrial time, proposing return to "
      "natural 13:20 frequency." },

    /* ===== MODERN (4 entries, indices 22-25) ===== */
    { 22,
      "Time is nothing but the ghost of space haunting the reflective "
      "consciousness.",
      "Henri Bergson", "Time and Free Will", 1889,
      TP_TRADITION_MODERN,
      { TP_THEME_SUBJECTIVE, TP_THEME_NATURE_OF_TIME, 0, 0 }, 2,
      "Bergson distinguished lived duration from clock time." },

    { 23,
      "Temporality temporalizes as a future which makes present in "
      "the process of having been.",
      "Martin Heidegger", "Being and Time", 1927,
      TP_TRADITION_MODERN,
      { TP_THEME_NATURE_OF_TIME, TP_THEME_SUBJECTIVE, 0, 0 }, 2,
      "Heidegger made temporal existence the foundation of human "
      "being." },

    { 24,
      "Absolute, true, and mathematical time, of itself and from its "
      "own nature, flows equably without relation to anything "
      "external.",
      "Isaac Newton", "Principia", 1687,
      TP_TRADITION_MODERN,
      { TP_THEME_MEASUREMENT, TP_THEME_LINEAR, 0, 0 }, 2,
      "Newton's absolute time became the foundation of classical "
      "physics." },

    { 25,
      "I hold space to be something merely relative, as time is -- "
      "an order of coexistences, as time is an order of successions.",
      "Gottfried Leibniz", "Correspondence with Clarke", 1716,
      TP_TRADITION_MODERN,
      { TP_THEME_NATURE_OF_TIME, TP_THEME_MEASUREMENT, 0, 0 }, 2,
      "Leibniz challenged Newton: time is relational, not "
      "absolute." },

    /* ===== SCIENTIFIC (4 entries, indices 26-29) ===== */
    { 26,
      "The distinction between the past, present, and future is only "
      "a stubbornly persistent illusion.",
      "Albert Einstein", NULL, 1955,
      TP_TRADITION_SCIENTIFIC,
      { TP_THEME_NATURE_OF_TIME, TP_THEME_COSMIC, 0, 0 }, 2,
      "Einstein's relativity showed time is not universal." },

    { 27,
      "Time is not completely separate from and independent of space, "
      "but is combined with it to form spacetime.",
      "Stephen Hawking", "A Brief History of Time", 1988,
      TP_TRADITION_SCIENTIFIC,
      { TP_THEME_COSMIC, TP_THEME_MEASUREMENT, 0, 0 }, 2,
      "Hawking popularized the unity of space and time." },

    { 28,
      "We are a way for the cosmos to know itself.",
      "Carl Sagan", "Cosmos", 1980,
      TP_TRADITION_SCIENTIFIC,
      { TP_THEME_COSMIC, TP_THEME_SACRED, 0, 0 }, 2,
      "Sagan framed human consciousness as the universe "
      "contemplating its own history." },

    { 29,
      "The future is not given. It is under perpetual construction.",
      "Ilya Prigogine", "The End of Certainty", 1997,
      TP_TRADITION_SCIENTIFIC,
      { TP_THEME_LINEAR, TP_THEME_NATURE_OF_TIME, 0, 0 }, 2,
      "Nobel laureate who showed irreversibility and creativity are "
      "fundamental to time." },
};

#define ENTRY_COUNT ((int)(sizeof(ENTRIES) / sizeof(ENTRIES[0])))

/* ---------- public API ---------- */

int tp_entry_count(void)
{
    return ENTRY_COUNT;
}

tp_entry_t tp_entry_get(int index)
{
    if (index < 0 || index >= ENTRY_COUNT) {
        tp_entry_t invalid = { -1, NULL, NULL, NULL, 0,
                               (tp_tradition_t)0,
                               { 0, 0, 0, 0 }, 0, NULL };
        return invalid;
    }
    return ENTRIES[index];
}

int tp_by_tradition(tp_tradition_t tradition, int *results, int max_results)
{
    if (results == NULL || max_results <= 0) return 0;
    if (tradition < 0 || tradition >= TP_TRADITION_COUNT) return 0;
    int count = 0;
    for (int i = 0; i < ENTRY_COUNT && count < max_results; i++) {
        if (ENTRIES[i].tradition == tradition) {
            results[count++] = i;
        }
    }
    return count;
}

int tp_by_theme(tp_theme_t theme, int *results, int max_results)
{
    if (results == NULL || max_results <= 0) return 0;
    if (theme < 0 || theme >= TP_THEME_COUNT) return 0;
    int count = 0;
    for (int i = 0; i < ENTRY_COUNT && count < max_results; i++) {
        for (int j = 0; j < ENTRIES[i].theme_count; j++) {
            if (ENTRIES[i].themes[j] == theme) {
                results[count++] = i;
                break;
            }
        }
    }
    return count;
}

int tp_by_author(const char *author)
{
    if (author == NULL) return -1;
    for (int i = 0; i < ENTRY_COUNT; i++) {
        if (tp_ci_contains(ENTRIES[i].author, author)) {
            return i;
        }
    }
    return -1;
}

const char *tp_tradition_name(tp_tradition_t tradition)
{
    if (tradition < 0 || tradition >= TP_TRADITION_COUNT) return "Unknown";
    return TRADITION_NAMES[tradition];
}

const char *tp_theme_name(tp_theme_t theme)
{
    if (theme < 0 || theme >= TP_THEME_COUNT) return "Unknown";
    return THEME_NAMES[theme];
}

int tp_tradition_count(tp_tradition_t tradition)
{
    if (tradition < 0 || tradition >= TP_TRADITION_COUNT) return 0;
    int count = 0;
    for (int i = 0; i < ENTRY_COUNT; i++) {
        if (ENTRIES[i].tradition == tradition) {
            count++;
        }
    }
    return count;
}

int tp_distinct_traditions(void)
{
    int seen[TP_TRADITION_COUNT];
    int n_seen = 0;
    for (int i = 0; i < TP_TRADITION_COUNT; i++) {
        seen[i] = 0;
    }
    for (int i = 0; i < ENTRY_COUNT; i++) {
        int t = (int)ENTRIES[i].tradition;
        if (t >= 0 && t < TP_TRADITION_COUNT && !seen[t]) {
            seen[t] = 1;
            n_seen++;
        }
    }
    return n_seen;
}
