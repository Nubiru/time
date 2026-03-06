#include "iching.h"

/* Trigram lines: [index][line], line 0=bottom, line 2=top */
static const int TRIGRAM_LINES[8][3] = {
    {1, 1, 1},  /* 0: Qian (Heaven)   */
    {1, 1, 0},  /* 1: Dui  (Lake)     */
    {1, 0, 1},  /* 2: Li   (Fire)     */
    {1, 0, 0},  /* 3: Zhen (Thunder)  */
    {0, 1, 1},  /* 4: Xun  (Wind)     */
    {0, 1, 0},  /* 5: Kan  (Water)    */
    {0, 0, 1},  /* 6: Gen  (Mountain) */
    {0, 0, 0},  /* 7: Kun  (Earth)    */
};

/* King Wen sequence: [kw-1] = {lower_trigram, upper_trigram} */
static const int KING_WEN[64][2] = {
    {0, 0}, /* 1  Qian       */ {7, 7}, /* 2  Kun        */
    {3, 5}, /* 3  Zhun       */ {5, 6}, /* 4  Meng       */
    {0, 5}, /* 5  Xu         */ {5, 0}, /* 6  Song       */
    {5, 7}, /* 7  Shi        */ {7, 5}, /* 8  Bi         */
    {0, 4}, /* 9  Xiao Chu   */ {1, 0}, /* 10 Lu         */
    {0, 7}, /* 11 Tai        */ {7, 0}, /* 12 Pi         */
    {2, 0}, /* 13 Tong Ren   */ {0, 2}, /* 14 Da You     */
    {6, 7}, /* 15 Qian       */ {7, 3}, /* 16 Yu         */
    {3, 1}, /* 17 Sui        */ {4, 6}, /* 18 Gu         */
    {1, 7}, /* 19 Lin        */ {7, 4}, /* 20 Guan       */
    {3, 2}, /* 21 Shi He     */ {2, 6}, /* 22 Bi         */
    {7, 6}, /* 23 Bo         */ {3, 7}, /* 24 Fu         */
    {3, 0}, /* 25 Wu Wang    */ {0, 6}, /* 26 Da Chu     */
    {3, 6}, /* 27 Yi         */ {4, 1}, /* 28 Da Guo     */
    {5, 5}, /* 29 Kan        */ {2, 2}, /* 30 Li         */
    {6, 1}, /* 31 Xian       */ {4, 3}, /* 32 Heng       */
    {6, 0}, /* 33 Dun        */ {0, 3}, /* 34 Da Zhuang  */
    {7, 2}, /* 35 Jin        */ {2, 7}, /* 36 Ming Yi    */
    {2, 4}, /* 37 Jia Ren    */ {1, 2}, /* 38 Kui        */
    {6, 5}, /* 39 Jian       */ {5, 3}, /* 40 Xie        */
    {1, 6}, /* 41 Sun        */ {3, 4}, /* 42 Yi         */
    {0, 1}, /* 43 Guai       */ {4, 0}, /* 44 Gou        */
    {7, 1}, /* 45 Cui        */ {4, 7}, /* 46 Sheng      */
    {5, 1}, /* 47 Kun        */ {4, 5}, /* 48 Jing       */
    {2, 1}, /* 49 Ge         */ {4, 2}, /* 50 Ding       */
    {3, 3}, /* 51 Zhen       */ {6, 6}, /* 52 Gen        */
    {6, 4}, /* 53 Jian       */ {1, 3}, /* 54 Gui Mei    */
    {2, 3}, /* 55 Feng       */ {6, 2}, /* 56 Lu         */
    {4, 4}, /* 57 Xun        */ {1, 1}, /* 58 Dui        */
    {5, 4}, /* 59 Huan       */ {1, 5}, /* 60 Jie        */
    {1, 4}, /* 61 Zhong Fu   */ {6, 3}, /* 62 Xiao Guo   */
    {2, 5}, /* 63 Ji Ji      */ {5, 2}, /* 64 Wei Ji     */
};

static const char *HEXAGRAM_NAMES[64] = {
    "Qian",     "Kun",      "Zhun",     "Meng",
    "Xu",       "Song",     "Shi",      "Bi",
    "Xiao Chu", "Lu",       "Tai",      "Pi",
    "Tong Ren", "Da You",   "Qian",     "Yu",
    "Sui",      "Gu",       "Lin",      "Guan",
    "Shi He",   "Bi",       "Bo",       "Fu",
    "Wu Wang",  "Da Chu",   "Yi",       "Da Guo",
    "Kan",      "Li",       "Xian",     "Heng",
    "Dun",      "Da Zhuang","Jin",      "Ming Yi",
    "Jia Ren",  "Kui",      "Jian",     "Xie",
    "Sun",      "Yi",       "Guai",     "Gou",
    "Cui",      "Sheng",    "Kun",      "Jing",
    "Ge",       "Ding",     "Zhen",     "Gen",
    "Jian",     "Gui Mei",  "Feng",     "Lu",
    "Xun",      "Dui",      "Huan",     "Jie",
    "Zhong Fu", "Xiao Guo", "Ji Ji",    "Wei Ji",
};

static const char *TRIGRAM_NAMES[8] = {
    "Qian", "Dui", "Li", "Zhen", "Xun", "Kan", "Gen", "Kun"
};

static const char *TRIGRAM_SYMBOLS[8] = {
    "\u2630", "\u2631", "\u2632", "\u2633",
    "\u2634", "\u2635", "\u2636", "\u2637"
};

static int lines_to_trigram_index(const int lines[3])
{
    return 7 - (lines[0] * 4 + lines[1] * 2 + lines[2]);
}

hexagram_t iching_hexagram(int king_wen_number)
{
    hexagram_t h = {{0, 0, 0, 0, 0, 0}, 0, 0, 0};
    if (king_wen_number < 1 || king_wen_number > 64)
        return h;

    int idx = king_wen_number - 1;
    h.lower_trigram = KING_WEN[idx][0];
    h.upper_trigram = KING_WEN[idx][1];
    h.king_wen = king_wen_number;

    for (int i = 0; i < 3; i++) {
        h.lines[i]     = TRIGRAM_LINES[h.lower_trigram][i];
        h.lines[i + 3] = TRIGRAM_LINES[h.upper_trigram][i];
    }
    return h;
}

hexagram_t iching_from_jd(double jd)
{
    int raw = (int)(jd - 0.5);
    int idx = raw % 64;
    if (idx < 0) idx += 64;
    return iching_hexagram(idx + 1);
}

const char *iching_hexagram_name(int king_wen)
{
    if (king_wen < 1 || king_wen > 64)
        return "?";
    return HEXAGRAM_NAMES[king_wen - 1];
}

const char *iching_trigram_name(int trigram)
{
    if (trigram < 0 || trigram > 7)
        return "?";
    return TRIGRAM_NAMES[trigram];
}

const char *iching_trigram_symbol(int trigram)
{
    if (trigram < 0 || trigram > 7)
        return "?";
    return TRIGRAM_SYMBOLS[trigram];
}

int iching_lines_to_king_wen(const int lines[6])
{
    int lower = lines_to_trigram_index(lines);
    int upper = lines_to_trigram_index(lines + 3);

    for (int i = 0; i < 64; i++) {
        if (KING_WEN[i][0] == lower && KING_WEN[i][1] == upper)
            return i + 1;
    }
    return -1;
}
