/* wisdom.c -- Curated wisdom quotes from great minds across civilizations.
 *
 * 192 quotes spanning Greek, Eastern, Islamic, Indian, Mayan/Indigenous,
 * Hebrew, Modern Science, and Other traditions (Egyptian, Buddhist,
 * Japanese, African, Celtic, Persian).
 *
 * All functions are pure: no globals, no malloc, no side effects.
 * Case-insensitive search uses tolower() character-by-character. */

#include "wisdom.h"

#include <ctype.h>
#include <string.h>

/* ---------- helpers ---------- */

/* Case-insensitive substring search (C11-portable, no strcasecmp). */
static int ci_contains(const char *haystack, const char *needle)
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

/* Case-insensitive exact string compare. */
static int ci_equals(const char *a, const char *b)
{
    if (a == NULL || b == NULL) return 0;
    int la = (int)strlen(a);
    int lb = (int)strlen(b);
    if (la != lb) return 0;
    for (int i = 0; i < la; i++) {
        if (tolower((unsigned char)a[i]) != tolower((unsigned char)b[i]))
            return 0;
    }
    return 1;
}

/* ---------- quote database ---------- */

static const wisdom_t QUOTES[] = {
    /* ===== GREEK (16 quotes, indices 0-15) ===== */
    { 0, "Time is a moving image of eternity.",
      "Plato", "Timaeus", -360, "Greek",
      { WISDOM_TAG_TIME, WISDOM_TAG_COSMOS, WISDOM_TAG_CONSCIOUSNESS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 1, "The soul takes nothing with her to the next world but her education and culture.",
      "Plato", "Phaedo", -360, "Greek",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, 0,0,0,0 }, 2,
      SENSITIVITY_GENERAL },
    { 2, "Nature does nothing in vain.",
      "Aristotle", "Politics", -350, "Greek",
      { WISDOM_TAG_NATURE, WISDOM_TAG_HARMONY, 0,0,0,0 }, 2,
      SENSITIVITY_GENERAL },
    { 3, "Time is the number of motion with respect to before and after.",
      "Aristotle", "Physics", -350, "Greek",
      { WISDOM_TAG_TIME, WISDOM_TAG_MATHEMATICS, WISDOM_TAG_CYCLES, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 4, "All is number.",
      "Pythagoras", NULL, -530, "Greek",
      { WISDOM_TAG_MATHEMATICS, WISDOM_TAG_UNITY, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 5, "There is geometry in the humming of the strings, there is music in the spacing of the spheres.",
      "Pythagoras", NULL, -530, "Greek",
      { WISDOM_TAG_HARMONY, WISDOM_TAG_MATHEMATICS, WISDOM_TAG_COSMOS, WISDOM_TAG_ASTRONOMY, 0,0 }, 4,
      SENSITIVITY_GENERAL },
    { 6, "Everything flows and nothing abides.",
      "Heraclitus", NULL, -500, "Greek",
      { WISDOM_TAG_CYCLES, WISDOM_TAG_TIME, WISDOM_TAG_NATURE, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 7, "The sun is new each day.",
      "Heraclitus", NULL, -500, "Greek",
      { WISDOM_TAG_ASTRONOMY, WISDOM_TAG_CYCLES, WISDOM_TAG_TIME, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 8, "Time is a river of passing events, and strong is its current.",
      "Marcus Aurelius", "Meditations", 170, "Greek",
      { WISDOM_TAG_TIME, WISDOM_TAG_NATURE, WISDOM_TAG_POWER, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 9, "The universe is change; our life is what our thoughts make it.",
      "Marcus Aurelius", "Meditations", 170, "Greek",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_CYCLES, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 10, "The only true wisdom is in knowing you know nothing.",
      "Socrates", NULL, -400, "Greek",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, 0,0,0,0 }, 2,
      SENSITIVITY_GENERAL },
    { 11, "The unexamined life is not worth living.",
      "Socrates", "Apology", -399, "Greek",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, 0,0,0,0 }, 2,
      SENSITIVITY_GENERAL },
    { 12, "Give me a place to stand and I will move the earth.",
      "Archimedes", NULL, -250, "Greek",
      { WISDOM_TAG_MATHEMATICS, WISDOM_TAG_POWER, WISDOM_TAG_EARTH, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 13, "From what does not exist at all, nothing can come into being; and for what exists to be utterly destroyed is impossible.",
      "Empedocles", NULL, -450, "Greek",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_CYCLES, WISDOM_TAG_NATURE, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 14, "Nothing exists except atoms and empty space; everything else is opinion.",
      "Democritus", NULL, -400, "Greek",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_NATURE, WISDOM_TAG_MATHEMATICS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 15, "Measure what is measurable, and make measurable what is not so.",
      "Eratosthenes", NULL, -240, "Greek",
      { WISDOM_TAG_MATHEMATICS, WISDOM_TAG_ASTRONOMY, WISDOM_TAG_EARTH, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },

    /* ===== EASTERN (16 quotes, indices 16-31) ===== */
    { 16, "The Tao that can be told is not the eternal Tao.",
      "Lao Tzu", "Tao Te Ching", -500, "Eastern",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_UNITY, WISDOM_TAG_COSMOS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 17, "Nature does not hurry, yet everything is accomplished.",
      "Lao Tzu", "Tao Te Ching", -500, "Eastern",
      { WISDOM_TAG_NATURE, WISDOM_TAG_TIME, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 18, "The journey of a thousand miles begins with a single step.",
      "Lao Tzu", "Tao Te Ching", -500, "Eastern",
      { WISDOM_TAG_TIME, WISDOM_TAG_POWER, WISDOM_TAG_CYCLES, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 19, "Study the past if you would define the future.",
      "Confucius", "Analects", -500, "Eastern",
      { WISDOM_TAG_TIME, WISDOM_TAG_CYCLES, WISDOM_TAG_CALENDAR, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 20, "Everything has beauty, but not everyone sees it.",
      "Confucius", "Analects", -500, "Eastern",
      { WISDOM_TAG_NATURE, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 21, "By three methods we may learn wisdom: by reflection, which is noblest; by imitation, which is easiest; and by experience, which is bitterest.",
      "Confucius", "Analects", -500, "Eastern",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, WISDOM_TAG_CYCLES, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 22, "Flow with whatever may happen and let your mind be free.",
      "Zhuangzi", "Zhuangzi", -300, "Eastern",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_NATURE, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 23, "The fish trap exists because of the fish. Once you have gotten the fish you can forget the trap.",
      "Zhuangzi", "Zhuangzi", -300, "Eastern",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, WISDOM_TAG_UNITY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 24, "Heaven and earth and I are of the same root.",
      "Zhuangzi", "Zhuangzi", -300, "Eastern",
      { WISDOM_TAG_UNITY, WISDOM_TAG_COSMOS, WISDOM_TAG_NATURE, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 25, "In the pursuit of learning, every day something is acquired. In the pursuit of Tao, every day something is dropped.",
      "Lao Tzu", "Tao Te Ching", -500, "Eastern",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_CYCLES, WISDOM_TAG_TIME, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 26, "When I let go of what I am, I become what I might be.",
      "Lao Tzu", "Tao Te Ching", -500, "Eastern",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, WISDOM_TAG_CYCLES, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 27, "If you understand, things are just as they are. If you do not understand, things are just as they are.",
      "Zen Proverb", NULL, 800, "Eastern",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_UNITY, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 28, "The Great Way is not difficult for those who have no preferences.",
      "Sengcan", "Xinxin Ming", 600, "Eastern",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_HARMONY, WISDOM_TAG_UNITY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 29, "Before enlightenment, chop wood, carry water. After enlightenment, chop wood, carry water.",
      "Zen Proverb", NULL, 800, "Eastern",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_CYCLES, WISDOM_TAG_TIME, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 30, "The ten thousand things rise and fall while the Self watches their return.",
      "Lao Tzu", "Tao Te Ching", -500, "Eastern",
      { WISDOM_TAG_CYCLES, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_COSMOS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 31, "Do not dwell in the past, do not dream of the future, concentrate the mind on the present moment.",
      "Siddhartha Gautama", NULL, -500, "Eastern",
      { WISDOM_TAG_TIME, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },

    /* ===== ISLAMIC (16 quotes, indices 32-47) ===== */
    { 32, "Let the beauty of what you love be what you do.",
      "Rumi", "Masnavi", 1250, "Islamic",
      { WISDOM_TAG_HARMONY, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 33, "The wound is the place where the Light enters you.",
      "Rumi", "Masnavi", 1250, "Islamic",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, WISDOM_TAG_UNITY, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 34, "Out beyond ideas of wrongdoing and rightdoing, there is a field. I will meet you there.",
      "Rumi", "Masnavi", 1250, "Islamic",
      { WISDOM_TAG_UNITY, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 35, "You are not a drop in the ocean. You are the entire ocean in a drop.",
      "Rumi", "Masnavi", 1250, "Islamic",
      { WISDOM_TAG_UNITY, WISDOM_TAG_COSMOS, WISDOM_TAG_CONSCIOUSNESS, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 36, "Yesterday I was clever, so I wanted to change the world. Today I am wise, so I am changing myself.",
      "Rumi", "Masnavi", 1250, "Islamic",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, WISDOM_TAG_TIME, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 37, "Be happy for this moment. This moment is your life.",
      "Omar Khayyam", "Rubaiyat", 1100, "Islamic",
      { WISDOM_TAG_TIME, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 38, "The Moving Finger writes; and, having writ, moves on.",
      "Omar Khayyam", "Rubaiyat", 1100, "Islamic",
      { WISDOM_TAG_TIME, WISDOM_TAG_CYCLES, WISDOM_TAG_POWER, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 39, "A moment of time is a moment of mercy.",
      "Omar Khayyam", "Rubaiyat", 1100, "Islamic",
      { WISDOM_TAG_TIME, WISDOM_TAG_CONSCIOUSNESS, 0,0,0,0 }, 2,
      SENSITIVITY_GENERAL },
    { 40, "The universe is a great book whose letters are the stars.",
      "Ibn Arabi", "Fusus al-Hikam", 1230, "Islamic",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_ASTRONOMY, WISDOM_TAG_UNITY, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 41, "He who knows himself knows his Lord.",
      "Ibn Arabi", "Fusus al-Hikam", 1230, "Islamic",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_UNITY, 0,0,0,0 }, 2,
      SENSITIVITY_RESPECTFUL },
    { 42, "Algebra is the science of restoration and balancing.",
      "Al-Khwarizmi", "Al-Kitab al-Mukhtasar", 820, "Islamic",
      { WISDOM_TAG_MATHEMATICS, WISDOM_TAG_HARMONY, 0,0,0,0 }, 2,
      SENSITIVITY_GENERAL },
    { 43, "The ink of the scholar is more sacred than the blood of the martyr.",
      "Prophet Muhammad", "Hadith", 630, "Islamic",
      { WISDOM_TAG_POWER, WISDOM_TAG_CONSCIOUSNESS, 0,0,0,0 }, 2,
      SENSITIVITY_RESPECTFUL },
    { 44, "Seek knowledge from the cradle to the grave.",
      "Prophet Muhammad", "Hadith", 630, "Islamic",
      { WISDOM_TAG_TIME, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_CYCLES, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 45, "The heavens and the earth contain Me not, but the heart of My faithful servant contains Me.",
      "Hadith Qudsi", NULL, 700, "Islamic",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_UNITY, 0,0,0 }, 3,
      SENSITIVITY_SACRED },
    { 46, "Every soul shall taste death, and only on the Day of Judgment shall you be paid your full recompense.",
      "Quran", "3:185", 630, "Islamic",
      { WISDOM_TAG_TIME, WISDOM_TAG_CYCLES, WISDOM_TAG_CONSCIOUSNESS, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 47, "Do you not see that God drives clouds, then joins them together, then makes them into a mass?",
      "Quran", "24:43", 630, "Islamic",
      { WISDOM_TAG_NATURE, WISDOM_TAG_EARTH, WISDOM_TAG_COSMOS, 0,0,0 }, 3,
      SENSITIVITY_SACRED },

    /* ===== INDIAN (16 quotes, indices 48-63) ===== */
    { 48, "As is the human body, so is the cosmic body. As is the human mind, so is the cosmic mind.",
      "Upanishads", "Chandogya", -800, "Indian",
      { WISDOM_TAG_UNITY, WISDOM_TAG_COSMOS, WISDOM_TAG_CONSCIOUSNESS, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 49, "Time I am, the great destroyer of the worlds, and I have come here to destroy all people.",
      "Bhagavad Gita", "11:32", -200, "Indian",
      { WISDOM_TAG_TIME, WISDOM_TAG_POWER, WISDOM_TAG_CYCLES, WISDOM_TAG_COSMOS, 0,0 }, 4,
      SENSITIVITY_SACRED },
    { 50, "Yoga is the cessation of the fluctuations of the mind.",
      "Patanjali", "Yoga Sutras", -200, "Indian",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_HARMONY, 0,0,0,0 }, 2,
      SENSITIVITY_GENERAL },
    { 51, "The earth has enough for everyone's need, but not enough for everyone's greed.",
      "Mahatma Gandhi", NULL, 1930, "Indian",
      { WISDOM_TAG_EARTH, WISDOM_TAG_NATURE, WISDOM_TAG_POWER, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 52, "You must be the change you wish to see in the world.",
      "Mahatma Gandhi", NULL, 1930, "Indian",
      { WISDOM_TAG_POWER, WISDOM_TAG_CONSCIOUSNESS, 0,0,0,0 }, 2,
      SENSITIVITY_GENERAL },
    { 53, "In the beginning was Brahman, with whom was the Word.",
      "Rig Veda", "10:129", -1500, "Indian",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_TIME, WISDOM_TAG_UNITY, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 54, "From the unreal lead me to the real. From darkness lead me to light. From death lead me to immortality.",
      "Upanishads", "Brihadaranyaka", -800, "Indian",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, WISDOM_TAG_CYCLES, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 55, "The moon marks off the seasons, and the sun knows when to go down.",
      "Aryabhata", "Aryabhatiya", 499, "Indian",
      { WISDOM_TAG_ASTRONOMY, WISDOM_TAG_CALENDAR, WISDOM_TAG_CYCLES, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 56, "As a man casts off worn-out garments and puts on new ones, so the embodied self casts off worn-out bodies.",
      "Bhagavad Gita", "2:22", -200, "Indian",
      { WISDOM_TAG_CYCLES, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_TIME, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 57, "When meditation is mastered, the mind is unwavering like the flame of a candle in a windless place.",
      "Bhagavad Gita", "6:19", -200, "Indian",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_HARMONY, WISDOM_TAG_POWER, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 58, "There is nothing higher than dharma. The weak overcome the strong through dharma.",
      "Upanishads", "Brihadaranyaka", -800, "Indian",
      { WISDOM_TAG_POWER, WISDOM_TAG_HARMONY, WISDOM_TAG_UNITY, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 59, "The wise see knowledge and action as one.",
      "Bhagavad Gita", "5:4", -200, "Indian",
      { WISDOM_TAG_UNITY, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 60, "The Self is the friend of the self, and the Self is the enemy of the self.",
      "Bhagavad Gita", "6:6", -200, "Indian",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_UNITY, 0,0,0,0 }, 2,
      SENSITIVITY_RESPECTFUL },
    { 61, "Just as in space the planets revolve around the sun, in time the ages revolve in cycles.",
      "Surya Siddhanta", NULL, 400, "Indian",
      { WISDOM_TAG_ASTRONOMY, WISDOM_TAG_CYCLES, WISDOM_TAG_TIME, WISDOM_TAG_CALENDAR, 0,0 }, 4,
      SENSITIVITY_GENERAL },
    { 62, "Truth is one; sages call it by various names.",
      "Rig Veda", "1:164:46", -1500, "Indian",
      { WISDOM_TAG_UNITY, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 63, "An atom is not the smallest unit. Within it are worlds upon worlds.",
      "Vaisheshika Sutra", NULL, -600, "Indian",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_MATHEMATICS, WISDOM_TAG_NATURE, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },

    /* ===== MAYAN / INDIGENOUS (12 quotes, indices 64-75) ===== */
    { 64, "Time is not money. Time is art.",
      "Jose Arguelles", "The Call of Pacal Votan", 1996, "Mayan",
      { WISDOM_TAG_TIME, WISDOM_TAG_CALENDAR, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 65, "We are the ones we have been waiting for.",
      "Hopi Elders", NULL, 2000, "Mayan",
      { WISDOM_TAG_TIME, WISDOM_TAG_POWER, WISDOM_TAG_CONSCIOUSNESS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 66, "Before the world was created, Tepeu and Gucumatz existed in the darkness and the water.",
      "Popol Vuh", NULL, -200, "Mayan",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_TIME, WISDOM_TAG_EARTH, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 67, "The days are the sacred burden carried by the calendar priests.",
      "Chilam Balam", "Book of Chumayel", 1500, "Mayan",
      { WISDOM_TAG_CALENDAR, WISDOM_TAG_TIME, WISDOM_TAG_CYCLES, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 68, "In Lak'ech — I am another yourself.",
      "Mayan Greeting", NULL, -500, "Mayan",
      { WISDOM_TAG_UNITY, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 69, "The thirteen intentions of creation cycle through the twenty aspects of existence.",
      "Jose Arguelles", "The Mayan Factor", 1987, "Mayan",
      { WISDOM_TAG_CYCLES, WISDOM_TAG_CALENDAR, WISDOM_TAG_MATHEMATICS, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 70, "Every 52 years the Calendar Round returns to its beginning. Time is a spiral, not a line.",
      "Mayan Wisdom", NULL, -500, "Mayan",
      { WISDOM_TAG_CYCLES, WISDOM_TAG_CALENDAR, WISDOM_TAG_TIME, WISDOM_TAG_MATHEMATICS, 0,0 }, 4,
      SENSITIVITY_GENERAL },
    { 71, "The earth has music for those who listen.",
      "Apache Proverb", NULL, 0, "Mayan",
      { WISDOM_TAG_EARTH, WISDOM_TAG_NATURE, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 72, "We do not inherit the earth from our ancestors; we borrow it from our children.",
      "Indigenous Proverb", NULL, 0, "Mayan",
      { WISDOM_TAG_EARTH, WISDOM_TAG_TIME, WISDOM_TAG_NATURE, WISDOM_TAG_CYCLES, 0,0 }, 4,
      SENSITIVITY_GENERAL },
    { 73, "Hunab Ku — the one giver of movement and measure.",
      "Mayan Teaching", NULL, -500, "Mayan",
      { WISDOM_TAG_UNITY, WISDOM_TAG_MATHEMATICS, WISDOM_TAG_COSMOS, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 74, "The synchronic order is the natural order of time as frequency.",
      "Jose Arguelles", "Time and the Technosphere", 2002, "Mayan",
      { WISDOM_TAG_TIME, WISDOM_TAG_HARMONY, WISDOM_TAG_CYCLES, WISDOM_TAG_CALENDAR, 0,0 }, 4,
      SENSITIVITY_RESPECTFUL },
    { 75, "When the last tree is cut down, the last fish eaten, and the last stream poisoned, you will realize you cannot eat money.",
      "Cree Prophecy", NULL, 0, "Mayan",
      { WISDOM_TAG_EARTH, WISDOM_TAG_NATURE, WISDOM_TAG_POWER, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },

    /* ===== HEBREW (12 quotes, indices 76-87) ===== */
    { 76, "To everything there is a season, and a time to every purpose under the heaven.",
      "Ecclesiastes", "3:1", -300, "Hebrew",
      { WISDOM_TAG_TIME, WISDOM_TAG_CYCLES, WISDOM_TAG_CALENDAR, WISDOM_TAG_NATURE, 0,0 }, 4,
      SENSITIVITY_RESPECTFUL },
    { 77, "He created His universe by three forms of expression: Numbers, Letters, and Words.",
      "Sefer Yetzirah", "1:1", -200, "Hebrew",
      { WISDOM_TAG_MATHEMATICS, WISDOM_TAG_COSMOS, WISDOM_TAG_UNITY, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 78, "The beginning of wisdom is awe of the Lord.",
      "Proverbs", "9:10", -600, "Hebrew",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, 0,0,0,0 }, 2,
      SENSITIVITY_RESPECTFUL },
    { 79, "As above, so below; as below, so above.",
      "Zohar", NULL, 1280, "Hebrew",
      { WISDOM_TAG_UNITY, WISDOM_TAG_COSMOS, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 80, "God created the world with ten utterances.",
      "Pirkei Avot", "5:1", -200, "Hebrew",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_MATHEMATICS, WISDOM_TAG_POWER, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 81, "Teach us to number our days, that we may gain a heart of wisdom.",
      "Psalms", "90:12", -500, "Hebrew",
      { WISDOM_TAG_TIME, WISDOM_TAG_CALENDAR, WISDOM_TAG_CONSCIOUSNESS, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 82, "The heavens declare the glory of God; the firmament shows His handiwork.",
      "Psalms", "19:1", -500, "Hebrew",
      { WISDOM_TAG_ASTRONOMY, WISDOM_TAG_COSMOS, WISDOM_TAG_NATURE, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 83, "For a thousand years in Your sight are but as yesterday when it is past.",
      "Psalms", "90:4", -500, "Hebrew",
      { WISDOM_TAG_TIME, WISDOM_TAG_COSMOS, WISDOM_TAG_CYCLES, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 84, "The thirty-two wondrous paths of wisdom are engraved in three mothers, seven doubles, and twelve simples.",
      "Sefer Yetzirah", "1:2", -200, "Hebrew",
      { WISDOM_TAG_MATHEMATICS, WISDOM_TAG_HARMONY, WISDOM_TAG_COSMOS, WISDOM_TAG_UNITY, 0,0 }, 4,
      SENSITIVITY_RESPECTFUL },
    { 85, "What has been will be again, what has been done will be done again; there is nothing new under the sun.",
      "Ecclesiastes", "1:9", -300, "Hebrew",
      { WISDOM_TAG_CYCLES, WISDOM_TAG_TIME, WISDOM_TAG_ASTRONOMY, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 86, "He has made everything beautiful in its time. He has also set eternity in the human heart.",
      "Ecclesiastes", "3:11", -300, "Hebrew",
      { WISDOM_TAG_TIME, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 87, "The Torah was created two thousand years before the world.",
      "Midrash", "Bereishit Rabbah", 400, "Hebrew",
      { WISDOM_TAG_TIME, WISDOM_TAG_COSMOS, WISDOM_TAG_MATHEMATICS, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },

    /* ===== MODERN SCIENCE (16 quotes, indices 88-103) ===== */
    { 88, "The distinction between past, present and future is only a stubbornly persistent illusion.",
      "Albert Einstein", NULL, 1955, "Modern Science",
      { WISDOM_TAG_TIME, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_COSMOS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 89, "Imagination is more important than knowledge.",
      "Albert Einstein", NULL, 1929, "Modern Science",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, 0,0,0,0 }, 2,
      SENSITIVITY_GENERAL },
    { 90, "God does not play dice with the universe.",
      "Albert Einstein", NULL, 1926, "Modern Science",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_MATHEMATICS, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 91, "We are a way for the cosmos to know itself.",
      "Carl Sagan", "Cosmos", 1980, "Modern Science",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_ASTRONOMY, WISDOM_TAG_UNITY, 0,0 }, 4,
      SENSITIVITY_GENERAL },
    { 92, "The nitrogen in our DNA, the calcium in our teeth, the iron in our blood were made in the interiors of collapsing stars.",
      "Carl Sagan", "Cosmos", 1980, "Modern Science",
      { WISDOM_TAG_ASTRONOMY, WISDOM_TAG_COSMOS, WISDOM_TAG_EARTH, WISDOM_TAG_UNITY, 0,0 }, 4,
      SENSITIVITY_GENERAL },
    { 93, "Somewhere, something incredible is waiting to be known.",
      "Carl Sagan", NULL, 1980, "Modern Science",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_CONSCIOUSNESS, 0,0,0,0 }, 2,
      SENSITIVITY_GENERAL },
    { 94, "Nature uses only the longest threads to weave her patterns, so each small piece of her fabric reveals the organization of the entire tapestry.",
      "Richard Feynman", "The Character of Physical Law", 1965, "Modern Science",
      { WISDOM_TAG_NATURE, WISDOM_TAG_UNITY, WISDOM_TAG_HARMONY, WISDOM_TAG_MATHEMATICS, 0,0 }, 4,
      SENSITIVITY_GENERAL },
    { 95, "If you think you understand quantum mechanics, you do not understand quantum mechanics.",
      "Richard Feynman", NULL, 1965, "Modern Science",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_COSMOS, WISDOM_TAG_MATHEMATICS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 96, "If I have seen further, it is by standing on the shoulders of giants.",
      "Isaac Newton", "Letter to Robert Hooke", 1676, "Modern Science",
      { WISDOM_TAG_POWER, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_ASTRONOMY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 97, "I do not know what I may appear to the world, but to myself I seem to have been only like a boy playing on the seashore.",
      "Isaac Newton", NULL, 1727, "Modern Science",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_NATURE, WISDOM_TAG_COSMOS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 98, "The most beautiful thing we can experience is the mysterious.",
      "Albert Einstein", NULL, 1930, "Modern Science",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_COSMOS, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 99, "Not only is the universe stranger than we imagine, it is stranger than we can imagine.",
      "Arthur Eddington", "The Nature of the Physical World", 1928, "Modern Science",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_CONSCIOUSNESS, 0,0,0,0 }, 2,
      SENSITIVITY_GENERAL },
    { 100, "We are the universe experiencing itself.",
      "Alan Watts", "The Book", 1966, "Modern Science",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_UNITY, WISDOM_TAG_CONSCIOUSNESS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 101, "Look again at that dot. That's here. That's home. That's us.",
      "Carl Sagan", "Pale Blue Dot", 1994, "Modern Science",
      { WISDOM_TAG_EARTH, WISDOM_TAG_ASTRONOMY, WISDOM_TAG_COSMOS, WISDOM_TAG_CONSCIOUSNESS, 0,0 }, 4,
      SENSITIVITY_GENERAL },
    { 102, "The cosmos is all that is or was or ever will be.",
      "Carl Sagan", "Cosmos", 1980, "Modern Science",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_TIME, WISDOM_TAG_ASTRONOMY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 103, "A human being is a part of the whole called by us universe.",
      "Albert Einstein", NULL, 1950, "Modern Science",
      { WISDOM_TAG_UNITY, WISDOM_TAG_COSMOS, WISDOM_TAG_CONSCIOUSNESS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },

    /* ===== OTHER TRADITIONS (21 quotes, indices 104-124) ===== */

    /* Egyptian */
    { 104, "As for man, his days are numbered. Whatever he may do, it is but wind.",
      "Egyptian Proverb", "Instruction of Ptahhotep", -2400, "Egyptian",
      { WISDOM_TAG_TIME, WISDOM_TAG_CYCLES, WISDOM_TAG_NATURE, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 105, "Know thyself. This is the beginning of all wisdom.",
      "Temple of Luxor", NULL, -1400, "Egyptian",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, WISDOM_TAG_UNITY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 106, "That which is Below corresponds to that which is Above, and that which is Above corresponds to that which is Below.",
      "Hermes Trismegistus", "Emerald Tablet", -200, "Egyptian",
      { WISDOM_TAG_UNITY, WISDOM_TAG_COSMOS, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },

    /* Buddhist */
    { 107, "Three things cannot be long hidden: the sun, the moon, and the truth.",
      "Buddhist Teaching", "Dhammapada", -400, "Buddhist",
      { WISDOM_TAG_ASTRONOMY, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_NATURE, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 108, "An idea that is developed and put into action is more important than an idea that exists only as an idea.",
      "Buddhist Teaching", "Sutta Nipata", -400, "Buddhist",
      { WISDOM_TAG_POWER, WISDOM_TAG_CONSCIOUSNESS, 0,0,0,0 }, 2,
      SENSITIVITY_RESPECTFUL },
    { 109, "The trouble is, you think you have time.",
      "Buddhist Teaching", NULL, -400, "Buddhist",
      { WISDOM_TAG_TIME, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 110, "In the sky, there is no distinction of east and west; people create distinctions out of their own minds.",
      "Buddhist Teaching", "Dhammapada", -400, "Buddhist",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_COSMOS, WISDOM_TAG_UNITY, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },

    /* Japanese */
    { 111, "The bamboo that bends is stronger than the oak that resists.",
      "Japanese Proverb", NULL, 0, "Japanese",
      { WISDOM_TAG_NATURE, WISDOM_TAG_POWER, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 112, "Fall seven times, stand up eight.",
      "Japanese Proverb", NULL, 0, "Japanese",
      { WISDOM_TAG_CYCLES, WISDOM_TAG_POWER, WISDOM_TAG_TIME, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 113, "Even monkeys fall from trees.",
      "Japanese Proverb", NULL, 0, "Japanese",
      { WISDOM_TAG_NATURE, WISDOM_TAG_CONSCIOUSNESS, 0,0,0,0 }, 2,
      SENSITIVITY_GENERAL },

    /* African */
    { 114, "However long the night, the dawn will break.",
      "African Proverb", NULL, 0, "African",
      { WISDOM_TAG_TIME, WISDOM_TAG_CYCLES, WISDOM_TAG_ASTRONOMY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 115, "If you want to go fast, go alone. If you want to go far, go together.",
      "African Proverb", NULL, 0, "African",
      { WISDOM_TAG_UNITY, WISDOM_TAG_POWER, WISDOM_TAG_TIME, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 116, "The earth is not ours; it is a treasure we hold in trust for future generations.",
      "African Proverb", NULL, 0, "African",
      { WISDOM_TAG_EARTH, WISDOM_TAG_TIME, WISDOM_TAG_NATURE, WISDOM_TAG_CYCLES, 0,0 }, 4,
      SENSITIVITY_GENERAL },

    /* Celtic */
    { 117, "The three candles that illuminate every darkness: truth, nature, and knowledge.",
      "Celtic Triad", NULL, -200, "Celtic",
      { WISDOM_TAG_NATURE, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 118, "The world is but a bridge; pass over it, but build no house upon it.",
      "Celtic Wisdom", NULL, -200, "Celtic",
      { WISDOM_TAG_TIME, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_EARTH, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },

    /* Persian */
    { 119, "I am the servant of the Quran as long as I have life. I am the dust on the path of Muhammad.",
      "Hafiz", "Divan", 1370, "Persian",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 120, "Even after all this time, the sun never says to the earth, you owe me. Look what happens with a love like that: it lights the whole sky.",
      "Hafiz", "Divan", 1370, "Persian",
      { WISDOM_TAG_ASTRONOMY, WISDOM_TAG_NATURE, WISDOM_TAG_HARMONY, WISDOM_TAG_EARTH, 0,0 }, 4,
      SENSITIVITY_RESPECTFUL },

    /* More diverse traditions */
    { 121, "When you arise in the morning, think of what a precious privilege it is to be alive.",
      "Marcus Aurelius", "Meditations", 170, "Greek",
      { WISDOM_TAG_TIME, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_NATURE, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 122, "The whole moon and the entire sky are reflected in dewdrops on the grass.",
      "Dogen", "Genjokoan", 1233, "Japanese",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_UNITY, WISDOM_TAG_NATURE, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 123, "The calendar is the most fundamental technology of civilization.",
      "Duncan Steel", "Marking Time", 2000, "Modern Science",
      { WISDOM_TAG_CALENDAR, WISDOM_TAG_TIME, WISDOM_TAG_CYCLES, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 124, "Look at the stars. See their beauty. And in that beauty see yourself.",
      "Druidic Wisdom", NULL, -200, "Celtic",
      { WISDOM_TAG_ASTRONOMY, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_COSMOS, WISDOM_TAG_HARMONY, 0,0 }, 4,
      SENSITIVITY_GENERAL },

    /* ===== LAO TZU — TAO TE CHING (13 quotes, indices 125-137) ===== */
    { 125, "The dark beyond dark, the door to all beginnings.",
      "Lao Tzu", "Tao Te Ching", -500, "Eastern",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_COSMOS, WISDOM_TAG_UNITY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 126, "Have and have not create each other; hard and easy produce each other; long and short shape each other.",
      "Lao Tzu", "Tao Te Ching", -500, "Eastern",
      { WISDOM_TAG_CYCLES, WISDOM_TAG_HARMONY, WISDOM_TAG_UNITY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 127, "Between Heaven and Earth, how like a bellows — empty but inexhaustible, each stroke produces more.",
      "Lao Tzu", "Tao Te Ching", -500, "Eastern",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_NATURE, WISDOM_TAG_CYCLES, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 128, "When your work is done retire — this is the Way of Heaven.",
      "Lao Tzu", "Tao Te Ching", -500, "Eastern",
      { WISDOM_TAG_CYCLES, WISDOM_TAG_TIME, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 129, "Existence makes a thing useful, but nonexistence makes it work.",
      "Lao Tzu", "Tao Te Ching", -500, "Eastern",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_UNITY, WISDOM_TAG_NATURE, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 130, "We look but don't see it and call it indistinct; we listen but don't hear it and call it faint; we reach but don't grasp it and call it ethereal.",
      "Lao Tzu", "Tao Te Ching", -500, "Eastern",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_NATURE, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 131, "The incomplete become whole; the crooked become straight; the hollow become full; the worn-out become new.",
      "Lao Tzu", "Tao Te Ching", -500, "Eastern",
      { WISDOM_TAG_CYCLES, WISDOM_TAG_HARMONY, WISDOM_TAG_NATURE, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 132, "Great means ever-flowing; ever-flowing means far-reaching; far-reaching means returning.",
      "Lao Tzu", "Tao Te Ching", -500, "Eastern",
      { WISDOM_TAG_CYCLES, WISDOM_TAG_TIME, WISDOM_TAG_COSMOS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 133, "What you would shorten you first should lengthen; what you would weaken you first should strengthen — this is called hiding the light.",
      "Lao Tzu", "Tao Te Ching", -500, "Eastern",
      { WISDOM_TAG_CYCLES, WISDOM_TAG_POWER, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 134, "The Tao makes no effort at all, yet there is nothing it doesn't do.",
      "Lao Tzu", "Tao Te Ching", -500, "Eastern",
      { WISDOM_TAG_NATURE, WISDOM_TAG_HARMONY, WISDOM_TAG_POWER, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 135, "The Tao moves the other way; the Tao works through weakness.",
      "Lao Tzu", "Tao Te Ching", -500, "Eastern",
      { WISDOM_TAG_CYCLES, WISDOM_TAG_NATURE, WISDOM_TAG_CONSCIOUSNESS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 136, "When superior people hear of the Way, they follow it with devotion. When inferior people hear of the Way, they laugh out loud — if they didn't laugh, it wouldn't be the Way.",
      "Lao Tzu", "Tao Te Ching", -500, "Eastern",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, WISDOM_TAG_UNITY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 137, "The Tao gives birth to one; one gives birth to two; two gives birth to three; three gives birth to ten thousand things.",
      "Lao Tzu", "Tao Te Ching", -500, "Eastern",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_UNITY, WISDOM_TAG_MATHEMATICS, WISDOM_TAG_CYCLES, 0,0 }, 4,
      SENSITIVITY_GENERAL },

    /* ===== MARCUS AURELIUS — MEDITATIONS (18 quotes, indices 138-155) ===== */
    { 138, "The duration of human life is a point; its substance perpetually flowing.",
      "Marcus Aurelius", "Meditations", 170, "Greek",
      { WISDOM_TAG_TIME, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_NATURE, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 139, "Each man lives only the present moment. The rest of time is either spent and gone, or is quite unknown.",
      "Marcus Aurelius", "Meditations", 170, "Greek",
      { WISDOM_TAG_TIME, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 140, "The longest life, and the shortest, come to one effect.",
      "Marcus Aurelius", "Meditations", 170, "Greek",
      { WISDOM_TAG_TIME, WISDOM_TAG_CYCLES, WISDOM_TAG_UNITY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 141, "Stand firm like a promontory, upon which the waves are always breaking.",
      "Marcus Aurelius", "Meditations", 170, "Greek",
      { WISDOM_TAG_POWER, WISDOM_TAG_NATURE, WISDOM_TAG_EARTH, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 142, "The nature of the universe delights in nothing more than in changing the things now existing.",
      "Marcus Aurelius", "Meditations", 170, "Greek",
      { WISDOM_TAG_CYCLES, WISDOM_TAG_COSMOS, WISDOM_TAG_NATURE, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 143, "Look backward on the immense antecedent eternity, and forward into another immensity.",
      "Marcus Aurelius", "Meditations", 170, "Greek",
      { WISDOM_TAG_TIME, WISDOM_TAG_COSMOS, WISDOM_TAG_CONSCIOUSNESS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 144, "Whatever thy seasons bear, shall be joyful fruits to me, O nature!",
      "Marcus Aurelius", "Meditations", 170, "Greek",
      { WISDOM_TAG_NATURE, WISDOM_TAG_CYCLES, WISDOM_TAG_TIME, WISDOM_TAG_HARMONY, 0,0 }, 4,
      SENSITIVITY_GENERAL },
    { 145, "From thee are all things; in thee they subsist; to thee they return.",
      "Marcus Aurelius", "Meditations", 170, "Greek",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_UNITY, WISDOM_TAG_CYCLES, WISDOM_TAG_NATURE, 0,0 }, 4,
      SENSITIVITY_GENERAL },
    { 146, "Many pieces of frankincense are laid on the altar: One falls, then another. And there's no difference.",
      "Marcus Aurelius", "Meditations", 170, "Greek",
      { WISDOM_TAG_TIME, WISDOM_TAG_CYCLES, WISDOM_TAG_CONSCIOUSNESS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 147, "You have arisen as a part in the universe, you shall disappear again, returning into your source.",
      "Marcus Aurelius", "Meditations", 170, "Greek",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_CYCLES, WISDOM_TAG_UNITY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 148, "There is a certain time appointed for you. If you don't employ it in making all calm and serene within you, it will pass away, and you along with it.",
      "Marcus Aurelius", "Meditations", 170, "Greek",
      { WISDOM_TAG_TIME, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 149, "All things hasten to an end, shall speedily seem old fables, and then be buried in oblivion.",
      "Marcus Aurelius", "Meditations", 170, "Greek",
      { WISDOM_TAG_TIME, WISDOM_TAG_CYCLES, WISDOM_TAG_CONSCIOUSNESS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 150, "What yesterday was a trifling embryo, to morrow shall be an embalmed carcase, or ashes.",
      "Marcus Aurelius", "Meditations", 170, "Greek",
      { WISDOM_TAG_TIME, WISDOM_TAG_CYCLES, WISDOM_TAG_NATURE, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 151, "Pass this short moment of time according to nature, and depart contentedly; as the full ripe olive falls of its own accord.",
      "Marcus Aurelius", "Meditations", 170, "Greek",
      { WISDOM_TAG_TIME, WISDOM_TAG_NATURE, WISDOM_TAG_HARMONY, WISDOM_TAG_CONSCIOUSNESS, 0,0 }, 4,
      SENSITIVITY_GENERAL },
    { 152, "All things are transitory, and, as it were, but for a day; both those who remember, and the things, and persons remembered.",
      "Marcus Aurelius", "Meditations", 170, "Greek",
      { WISDOM_TAG_TIME, WISDOM_TAG_CYCLES, WISDOM_TAG_CONSCIOUSNESS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 153, "Consider always this universe as one living being, with one material substance and one spirit.",
      "Marcus Aurelius", "Meditations", 170, "Greek",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_UNITY, WISDOM_TAG_CONSCIOUSNESS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 154, "Recollect the times of Vespasian: you will see all the same things. Men marrying, bringing up children, sickening, dying.",
      "Marcus Aurelius", "Meditations", 170, "Greek",
      { WISDOM_TAG_CYCLES, WISDOM_TAG_TIME, WISDOM_TAG_CONSCIOUSNESS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 155, "'Tis the office of our rational power, to apprehend how swiftly all things vanish.",
      "Marcus Aurelius", "Meditations", 170, "Greek",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_TIME, WISDOM_TAG_POWER, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },

    /* ===== HERMANN HESSE — SIDDHARTHA (16 quotes, indices 156-171) ===== */
    { 156, "Time is a fictitious thing. I have proven this many times.",
      "Hermann Hesse", "Siddhartha", 1922, "Eastern",
      { WISDOM_TAG_TIME, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_UNITY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 157, "This water ran and ran, ran without ceasing, and yet it was always there, always the same, and yet — always new!",
      "Hermann Hesse", "Siddhartha", 1922, "Eastern",
      { WISDOM_TAG_TIME, WISDOM_TAG_CYCLES, WISDOM_TAG_NATURE, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 158, "Every sin contains grace within itself, every child already carries the old man within.",
      "Hermann Hesse", "Siddhartha", 1922, "Eastern",
      { WISDOM_TAG_UNITY, WISDOM_TAG_CYCLES, WISDOM_TAG_CONSCIOUSNESS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 159, "Seeking means having a goal. But finding means being free, open to everything, having no goal.",
      "Hermann Hesse", "Siddhartha", 1922, "Eastern",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 160, "Knowledge can be transmitted, but wisdom cannot.",
      "Hermann Hesse", "Siddhartha", 1922, "Eastern",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, 0,0,0,0 }, 2,
      SENSITIVITY_GENERAL },
    { 161, "Love seems to me the motive of everything.",
      "Hermann Hesse", "Siddhartha", 1922, "Eastern",
      { WISDOM_TAG_UNITY, WISDOM_TAG_HARMONY, WISDOM_TAG_CONSCIOUSNESS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 162, "The great song of a thousand voices concentrated into a single word: Om — perfection.",
      "Hermann Hesse", "Siddhartha", 1922, "Eastern",
      { WISDOM_TAG_UNITY, WISDOM_TAG_HARMONY, WISDOM_TAG_COSMOS, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 163, "We do not walk in circles, we go upward; the circle is a spiral, we have already climbed many steps.",
      "Hermann Hesse", "Siddhartha", 1922, "Eastern",
      { WISDOM_TAG_CYCLES, WISDOM_TAG_TIME, WISDOM_TAG_CONSCIOUSNESS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 164, "The opposite of every truth is equally true.",
      "Hermann Hesse", "Siddhartha", 1922, "Eastern",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_UNITY, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 165, "This stone is also God, is also Buddha — it is everything, forever.",
      "Hermann Hesse", "Siddhartha", 1922, "Eastern",
      { WISDOM_TAG_UNITY, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_COSMOS, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 166, "Surrendered to the current, belonging to unity.",
      "Hermann Hesse", "Siddhartha", 1922, "Eastern",
      { WISDOM_TAG_UNITY, WISDOM_TAG_HARMONY, WISDOM_TAG_NATURE, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 167, "Love this water! Stay by it! Learn from it!",
      "Hermann Hesse", "Siddhartha", 1922, "Eastern",
      { WISDOM_TAG_NATURE, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_EARTH, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 168, "The river is what taught me to listen. It knows everything; everything can be learned from it.",
      "Hermann Hesse", "Siddhartha", 1922, "Eastern",
      { WISDOM_TAG_NATURE, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 169, "In deep meditation there is the possibility of annulling time, of seeing past life, present and future, simultaneously.",
      "Hermann Hesse", "Siddhartha", 1922, "Eastern",
      { WISDOM_TAG_TIME, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_CYCLES, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 170, "He could no longer distinguish the many voices. All was one, all intertwined a thousand times. All of it formed the river of happening, the music of life.",
      "Hermann Hesse", "Siddhartha", 1922, "Eastern",
      { WISDOM_TAG_UNITY, WISDOM_TAG_HARMONY, WISDOM_TAG_CYCLES, WISDOM_TAG_CONSCIOUSNESS, 0,0 }, 4,
      SENSITIVITY_GENERAL },
    { 171, "It was the ancient beginning and end of all Brahman prayers, the sacred Om, meaning the Perfect, the Consummation.",
      "Hermann Hesse", "Siddhartha", 1922, "Eastern",
      { WISDOM_TAG_UNITY, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_COSMOS, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },

    /* ===== SEFER YETZIRAH (3 quotes, indices 172-174) ===== */
    { 172, "He permuted them, weighed them, and transformed them. Alef with them all and all of them with Alef.",
      "Sefer Yetzirah", "2:5", -200, "Hebrew",
      { WISDOM_TAG_MATHEMATICS, WISDOM_TAG_COSMOS, WISDOM_TAG_UNITY, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 173, "Two stones build two houses. Three stones build six houses. From here on, go out and calculate that which the mouth cannot speak and the ear cannot hear.",
      "Sefer Yetzirah", "4:16", -200, "Hebrew",
      { WISDOM_TAG_MATHEMATICS, WISDOM_TAG_COSMOS, WISDOM_TAG_POWER, 0,0,0 }, 3,
      SENSITIVITY_RESPECTFUL },
    { 174, "Seven Universes, seven firmaments, seven lands, seven seas, seven rivers, seven deserts, seven days, seven weeks, seven years.",
      "Sefer Yetzirah", "4:15", -200, "Hebrew",
      { WISDOM_TAG_MATHEMATICS, WISDOM_TAG_COSMOS, WISDOM_TAG_CYCLES, WISDOM_TAG_HARMONY, 0,0 }, 4,
      SENSITIVITY_RESPECTFUL },

    /* ===== KEPLER — HARMONICES MUNDI (2 quotes, indices 175-176) ===== */
    { 175, "The movements of the heavens are nothing except a certain everlasting polyphony, perceived by the intellect, not by the ear.",
      "Johannes Kepler", "Harmonices Mundi", 1619, "Modern Science",
      { WISDOM_TAG_HARMONY, WISDOM_TAG_ASTRONOMY, WISDOM_TAG_COSMOS, WISDOM_TAG_MATHEMATICS, 0,0 }, 4,
      SENSITIVITY_GENERAL },
    { 176, "Let us despise the barbaric neighings which echo through these noble lands, and awaken our understanding and longing for the harmonies.",
      "Johannes Kepler", "Harmonices Mundi", 1619, "Modern Science",
      { WISDOM_TAG_HARMONY, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_COSMOS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },

    /* ===== MACROBIUS (1 quote, index 177) ===== */
    { 177, "Apollo's Lyre of seven strings provides understanding of all the celestial spheres over which nature has set the Sun as moderator.",
      "Macrobius", "Commentary on Dream of Scipio", 400, "Greek",
      { WISDOM_TAG_HARMONY, WISDOM_TAG_ASTRONOMY, WISDOM_TAG_COSMOS, WISDOM_TAG_MATHEMATICS, 0,0 }, 4,
      SENSITIVITY_GENERAL },

    /* ===== TAOTECHING COMMENTATORS + EXTRA LAO TZU (4 quotes, indices 178-181) ===== */
    { 178, "Once the sun reaches the zenith, it descends. Once the moon becomes full, it wanes.",
      "Ho-shang Kung", "Commentary on Tao Te Ching", -200, "Eastern",
      { WISDOM_TAG_CYCLES, WISDOM_TAG_ASTRONOMY, WISDOM_TAG_TIME, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 179, "Emptiness is the Way of Heaven. Stillness is the Way of Earth. There is nothing that is not endowed with these.",
      "Sung Ch'ang-hsing", "Commentary on Tao Te Ching", 1700, "Eastern",
      { WISDOM_TAG_HARMONY, WISDOM_TAG_NATURE, WISDOM_TAG_COSMOS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 180, "The yang we embrace is one. The yin we turn away from is two. Where yin and yang meet and merge is three.",
      "Li Hsi-chai", "Commentary on Tao Te Ching", 1100, "Eastern",
      { WISDOM_TAG_UNITY, WISDOM_TAG_MATHEMATICS, WISDOM_TAG_HARMONY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 181, "Returning to their roots they are still; being still they revive; reviving they endure; knowing how to endure is wisdom.",
      "Lao Tzu", "Tao Te Ching", -500, "Eastern",
      { WISDOM_TAG_CYCLES, WISDOM_TAG_TIME, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_NATURE, 0,0 }, 4,
      SENSITIVITY_GENERAL },

    /* ===== YUVAL NOAH HARARI — SAPIENS (10 quotes, indices 182-191) ===== */
    { 182, "About 13.5 billion years ago, matter, energy, time and space came into being in what is known as the Big Bang.",
      "Yuval Noah Harari", "Sapiens", 2011, "Modern Science",
      { WISDOM_TAG_COSMOS, WISDOM_TAG_TIME, WISDOM_TAG_ASTRONOMY, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 183, "Large numbers of strangers can cooperate successfully by believing in common myths.",
      "Yuval Noah Harari", "Sapiens", 2011, "Modern Science",
      { WISDOM_TAG_UNITY, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 184, "Ever since the Cognitive Revolution, Sapiens have been living in a dual reality. The objective reality of rivers, trees and lions; and the imagined reality of gods, nations and corporations.",
      "Yuval Noah Harari", "Sapiens", 2011, "Modern Science",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_UNITY, WISDOM_TAG_COSMOS, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 185, "The world went about its business without clocks and timetables, subject only to the movements of the sun and the growth cycles of plants.",
      "Yuval Noah Harari", "Sapiens", 2011, "Modern Science",
      { WISDOM_TAG_TIME, WISDOM_TAG_CALENDAR, WISDOM_TAG_NATURE, WISDOM_TAG_CYCLES, 0,0 }, 4,
      SENSITIVITY_GENERAL },
    { 186, "For the first time in history, a country adopted a national time and obliged its population to live according to an artificial clock.",
      "Yuval Noah Harari", "Sapiens", 2011, "Modern Science",
      { WISDOM_TAG_TIME, WISDOM_TAG_CALENDAR, WISDOM_TAG_POWER, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 187, "The typical person consults these clocks several dozen times a day, because almost everything we do has to be done on time.",
      "Yuval Noah Harari", "Sapiens", 2011, "Modern Science",
      { WISDOM_TAG_TIME, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_CYCLES, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 188, "Three important revolutions shaped the course of history: the Cognitive Revolution, the Agricultural Revolution, and the Scientific Revolution.",
      "Yuval Noah Harari", "Sapiens", 2011, "Modern Science",
      { WISDOM_TAG_TIME, WISDOM_TAG_CYCLES, WISDOM_TAG_POWER, WISDOM_TAG_CONSCIOUSNESS, 0,0 }, 4,
      SENSITIVITY_GENERAL },
    { 189, "Is there anything more dangerous than dissatisfied and irresponsible gods who don't know what they want?",
      "Yuval Noah Harari", "Sapiens", 2011, "Modern Science",
      { WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_POWER, WISDOM_TAG_EARTH, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 190, "We are more powerful than ever before, but have very little idea what to do with all that power.",
      "Yuval Noah Harari", "Sapiens", 2011, "Modern Science",
      { WISDOM_TAG_POWER, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_EARTH, 0,0,0 }, 3,
      SENSITIVITY_GENERAL },
    { 191, "Today it stands on the verge of becoming a god, poised to acquire not only eternal youth, but also the divine abilities of creation and destruction.",
      "Yuval Noah Harari", "Sapiens", 2011, "Modern Science",
      { WISDOM_TAG_POWER, WISDOM_TAG_TIME, WISDOM_TAG_CONSCIOUSNESS, WISDOM_TAG_COSMOS, 0,0 }, 4,
      SENSITIVITY_GENERAL },
};

static const int QUOTE_COUNT = sizeof(QUOTES) / sizeof(QUOTES[0]);

/* ---------- tag names ---------- */

static const char *TAG_NAMES[] = {
    "Astronomy",
    "Time",
    "Cycles",
    "Unity",
    "Mathematics",
    "Nature",
    "Consciousness",
    "Calendar",
    "Cosmos",
    "Earth",
    "Harmony",
    "Power"
};

/* ---------- public API ---------- */

int wisdom_quote_count(void)
{
    return QUOTE_COUNT;
}

wisdom_t wisdom_quote_get(int index)
{
    if (index < 0 || index >= QUOTE_COUNT) {
        wisdom_t invalid = { .id = -1, .text = NULL, .author = NULL,
                             .work = NULL, .year = 0, .culture = NULL,
                             .tags = {0,0,0,0,0,0}, .tag_count = 0,
                             .sensitivity = SENSITIVITY_GENERAL };
        return invalid;
    }
    return QUOTES[index];
}

int wisdom_by_tag(wisdom_tag_t tag, int *results, int max_results)
{
    if (results == NULL || max_results <= 0) return 0;
    if (tag < 0 || tag >= WISDOM_TAG_COUNT) return 0;
    int count = 0;
    for (int i = 0; i < QUOTE_COUNT && count < max_results; i++) {
        for (int j = 0; j < QUOTES[i].tag_count; j++) {
            if (QUOTES[i].tags[j] == tag) {
                results[count++] = i;
                break;
            }
        }
    }
    return count;
}

int wisdom_by_culture(const char *culture, int *results, int max_results)
{
    if (culture == NULL || results == NULL || max_results <= 0) return 0;
    int count = 0;
    for (int i = 0; i < QUOTE_COUNT && count < max_results; i++) {
        if (ci_contains(QUOTES[i].culture, culture)) {
            results[count++] = i;
        }
    }
    return count;
}

int wisdom_by_author(const char *author)
{
    if (author == NULL) return -1;
    for (int i = 0; i < QUOTE_COUNT; i++) {
        if (ci_contains(QUOTES[i].author, author)) {
            return i;
        }
    }
    return -1;
}

const char *wisdom_tag_name(wisdom_tag_t tag)
{
    if (tag < 0 || tag >= WISDOM_TAG_COUNT) return "Unknown";
    return TAG_NAMES[tag];
}

int wisdom_culture_count(const char *culture)
{
    if (culture == NULL) return 0;
    int count = 0;
    for (int i = 0; i < QUOTE_COUNT; i++) {
        if (ci_contains(QUOTES[i].culture, culture)) {
            count++;
        }
    }
    return count;
}

int wisdom_distinct_cultures(void)
{
    /* Collect unique culture strings (case-insensitive exact match). */
    const char *seen[32];
    int n_seen = 0;
    for (int i = 0; i < QUOTE_COUNT; i++) {
        int found = 0;
        for (int j = 0; j < n_seen; j++) {
            if (ci_equals(seen[j], QUOTES[i].culture)) {
                found = 1;
                break;
            }
        }
        if (!found && n_seen < 32) {
            seen[n_seen++] = QUOTES[i].culture;
        }
    }
    return n_seen;
}

int wisdom_by_sensitivity(sensitivity_t level, int *results, int max_results)
{
    if (results == NULL || max_results <= 0) return 0;
    int count = 0;
    int total = (int)(sizeof(QUOTES) / sizeof(QUOTES[0]));
    for (int i = 0; i < total && count < max_results; i++) {
        if (QUOTES[i].sensitivity == level) {
            results[count++] = i;
        }
    }
    return count;
}
