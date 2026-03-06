#include "chinese.h"
#include "../../math/julian.h"

/* Reference: 4 CE = Jia-Zi (stem 0, branch 0, cycle year 1). */
#define CYCLE_EPOCH 4

static const char *STEM_NAMES[] = {
    "Jia", "Yi", "Bing", "Ding", "Wu",
    "Ji", "Geng", "Xin", "Ren", "Gui"
};

static const char *BRANCH_NAMES[] = {
    "Zi", "Chou", "Yin", "Mao", "Chen", "Si",
    "Wu", "Wei", "Shen", "You", "Xu", "Hai"
};

static const char *ANIMAL_NAMES[] = {
    "Rat", "Ox", "Tiger", "Rabbit", "Dragon", "Snake",
    "Horse", "Goat", "Monkey", "Rooster", "Dog", "Pig"
};

static const char *ELEMENT_NAMES[] = {
    "Wood", "Fire", "Earth", "Metal", "Water"
};

static const char *ANIMAL_SYMBOLS[] = {
    "\xf0\x9f\x90\x80", /* Rat     U+1F400 */
    "\xf0\x9f\x90\x82", /* Ox      U+1F402 */
    "\xf0\x9f\x90\x85", /* Tiger   U+1F405 */
    "\xf0\x9f\x90\x87", /* Rabbit  U+1F407 */
    "\xf0\x9f\x90\x89", /* Dragon  U+1F409 */
    "\xf0\x9f\x90\x8d", /* Snake   U+1F40D */
    "\xf0\x9f\x90\x8e", /* Horse   U+1F40E */
    "\xf0\x9f\x90\x90", /* Goat    U+1F410 */
    "\xf0\x9f\x90\x92", /* Monkey  U+1F412 */
    "\xf0\x9f\x90\x93", /* Rooster U+1F413 */
    "\xf0\x9f\x90\x95", /* Dog     U+1F415 */
    "\xf0\x9f\x90\x96"  /* Pig     U+1F416 */
};

/* C modulo can be negative; this always returns 0..mod-1. */
static int pos_mod(int val, int mod) {
    int r = val % mod;
    if (r < 0) r += mod;
    return r;
}

chinese_year_t chinese_year_from_gregorian(int year) {
    chinese_year_t cy;
    int offset = year - CYCLE_EPOCH;

    cy.stem       = pos_mod(offset, 10);
    cy.branch     = pos_mod(offset, 12);
    cy.animal     = cy.branch;
    cy.element    = cy.stem / 2;
    cy.polarity   = cy.stem % 2;
    cy.cycle_year = pos_mod(offset, 60) + 1;

    return cy;
}

chinese_year_t chinese_year_from_jd(double jd) {
    int year, month;
    jd_to_gregorian(jd, &year, &month);
    return chinese_year_from_gregorian(year);
}

const char *chinese_stem_name(int stem) {
    if (stem < 0 || stem > 9) return "?";
    return STEM_NAMES[stem];
}

const char *chinese_branch_name(int branch) {
    if (branch < 0 || branch > 11) return "?";
    return BRANCH_NAMES[branch];
}

const char *chinese_animal_name(int animal) {
    if (animal < 0 || animal > 11) return "?";
    return ANIMAL_NAMES[animal];
}

const char *chinese_element_name(int element) {
    if (element < 0 || element > 4) return "?";
    return ELEMENT_NAMES[element];
}

const char *chinese_polarity_name(int polarity) {
    if (polarity < 0 || polarity > 1) return "?";
    return polarity == 0 ? "Yang" : "Yin";
}

const char *chinese_animal_symbol(int animal) {
    if (animal < 0 || animal > 11) return "?";
    return ANIMAL_SYMBOLS[animal];
}
