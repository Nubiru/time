/* codon_hexagram.c — DNA codon to I Ching hexagram structural map implementation.
 * Pure data + lookup. All data is static const. No malloc, no globals.
 * Source: Martin Schonberger, "The I Ching and the Genetic Code" (1973). */

#include "codon_hexagram.h"
#include <stddef.h>
#include <string.h>

/* ===== RNA base data ===== */

static const char *const BASE_NAMES[RNA_BASE_COUNT] = {
    "Uracil", "Cytosine", "Adenine", "Guanine"
};

static const char BASE_LETTERS[RNA_BASE_COUNT] = {
    'U', 'C', 'A', 'G'
};

/* ===== Binary to King Wen lookup =====
 * Index = 6-bit binary value (upper_trigram * 8 + lower_trigram).
 * Value = King Wen hexagram number (1-64).
 * Trigram encoding: Earth=0, Mountain=1, Water=2, Wind=3,
 *                   Thunder=4, Fire=5, Lake=6, Heaven=7. */

static const int BINARY_TO_KING_WEN[64] = {
    /*  0 Earth/Earth   */  2,
    /*  1 Earth/Mountain*/  23,
    /*  2 Earth/Water   */  8,
    /*  3 Earth/Wind    */  20,
    /*  4 Earth/Thunder */  16,
    /*  5 Earth/Fire    */  35,
    /*  6 Earth/Lake    */  45,
    /*  7 Earth/Heaven  */  12,
    /*  8 Mtn/Earth     */  15,
    /*  9 Mtn/Mountain  */  52,
    /* 10 Mtn/Water     */  39,
    /* 11 Mtn/Wind      */  53,
    /* 12 Mtn/Thunder   */  62,
    /* 13 Mtn/Fire      */  56,
    /* 14 Mtn/Lake      */  31,
    /* 15 Mtn/Heaven    */  33,
    /* 16 Water/Earth   */  7,
    /* 17 Water/Mountain*/  4,
    /* 18 Water/Water   */  29,
    /* 19 Water/Wind    */  59,
    /* 20 Water/Thunder */  40,
    /* 21 Water/Fire    */  64,
    /* 22 Water/Lake    */  47,
    /* 23 Water/Heaven  */  6,
    /* 24 Wind/Earth    */  46,
    /* 25 Wind/Mountain */  18,
    /* 26 Wind/Water    */  48,
    /* 27 Wind/Wind     */  57,
    /* 28 Wind/Thunder  */  32,
    /* 29 Wind/Fire     */  50,
    /* 30 Wind/Lake     */  28,
    /* 31 Wind/Heaven   */  44,
    /* 32 Thunder/Earth */  24,
    /* 33 Thunder/Mtn   */  27,
    /* 34 Thunder/Water */  3,
    /* 35 Thunder/Wind  */  42,
    /* 36 Thunder/Thunder*/ 51,
    /* 37 Thunder/Fire  */  21,
    /* 38 Thunder/Lake  */  17,
    /* 39 Thunder/Heaven*/  25,
    /* 40 Fire/Earth    */  36,
    /* 41 Fire/Mountain */  22,
    /* 42 Fire/Water    */  63,
    /* 43 Fire/Wind     */  37,
    /* 44 Fire/Thunder  */  55,
    /* 45 Fire/Fire     */  30,
    /* 46 Fire/Lake     */  49,
    /* 47 Fire/Heaven   */  13,
    /* 48 Lake/Earth    */  19,
    /* 49 Lake/Mountain */  41,
    /* 50 Lake/Water    */  60,
    /* 51 Lake/Wind     */  61,
    /* 52 Lake/Thunder  */  54,
    /* 53 Lake/Fire     */  38,
    /* 54 Lake/Lake     */  58,
    /* 55 Lake/Heaven   */  10,
    /* 56 Heaven/Earth  */  11,
    /* 57 Heaven/Mtn    */  26,
    /* 58 Heaven/Water  */  5,
    /* 59 Heaven/Wind   */  9,
    /* 60 Heaven/Thunder*/  34,
    /* 61 Heaven/Fire   */  14,
    /* 62 Heaven/Lake   */  43,
    /* 63 Heaven/Heaven */  1
};

/* ===== Codon data table =====
 * Each entry: { codon_str, amino_acid, amino_abbrev, amino_letter, is_stop, is_start }
 * Indexed by codon_index = base1*16 + base2*4 + base3. */

typedef struct {
    const char *codon_str;
    const char *amino_acid;
    const char *amino_abbrev;
    char amino_letter;
    int is_stop;
    int is_start;
} codon_data_t;

static const codon_data_t CODON_TABLE[64] = {
    /* U-first (0-15) */
    /*  0 */ { "UUU", "Phenylalanine",  "Phe",  'F', 0, 0 },
    /*  1 */ { "UUC", "Phenylalanine",  "Phe",  'F', 0, 0 },
    /*  2 */ { "UUA", "Leucine",        "Leu",  'L', 0, 0 },
    /*  3 */ { "UUG", "Leucine",        "Leu",  'L', 0, 0 },
    /*  4 */ { "UCU", "Serine",         "Ser",  'S', 0, 0 },
    /*  5 */ { "UCC", "Serine",         "Ser",  'S', 0, 0 },
    /*  6 */ { "UCA", "Serine",         "Ser",  'S', 0, 0 },
    /*  7 */ { "UCG", "Serine",         "Ser",  'S', 0, 0 },
    /*  8 */ { "UAU", "Tyrosine",       "Tyr",  'Y', 0, 0 },
    /*  9 */ { "UAC", "Tyrosine",       "Tyr",  'Y', 0, 0 },
    /* 10 */ { "UAA", "Stop",           "Stop", '*', 1, 0 },
    /* 11 */ { "UAG", "Stop",           "Stop", '*', 1, 0 },
    /* 12 */ { "UGU", "Cysteine",       "Cys",  'C', 0, 0 },
    /* 13 */ { "UGC", "Cysteine",       "Cys",  'C', 0, 0 },
    /* 14 */ { "UGA", "Stop",           "Stop", '*', 1, 0 },
    /* 15 */ { "UGG", "Tryptophan",     "Trp",  'W', 0, 0 },

    /* C-first (16-31) */
    /* 16 */ { "CUU", "Leucine",        "Leu",  'L', 0, 0 },
    /* 17 */ { "CUC", "Leucine",        "Leu",  'L', 0, 0 },
    /* 18 */ { "CUA", "Leucine",        "Leu",  'L', 0, 0 },
    /* 19 */ { "CUG", "Leucine",        "Leu",  'L', 0, 0 },
    /* 20 */ { "CCU", "Proline",        "Pro",  'P', 0, 0 },
    /* 21 */ { "CCC", "Proline",        "Pro",  'P', 0, 0 },
    /* 22 */ { "CCA", "Proline",        "Pro",  'P', 0, 0 },
    /* 23 */ { "CCG", "Proline",        "Pro",  'P', 0, 0 },
    /* 24 */ { "CAU", "Histidine",      "His",  'H', 0, 0 },
    /* 25 */ { "CAC", "Histidine",      "His",  'H', 0, 0 },
    /* 26 */ { "CAA", "Glutamine",      "Gln",  'Q', 0, 0 },
    /* 27 */ { "CAG", "Glutamine",      "Gln",  'Q', 0, 0 },
    /* 28 */ { "CGU", "Arginine",       "Arg",  'R', 0, 0 },
    /* 29 */ { "CGC", "Arginine",       "Arg",  'R', 0, 0 },
    /* 30 */ { "CGA", "Arginine",       "Arg",  'R', 0, 0 },
    /* 31 */ { "CGG", "Arginine",       "Arg",  'R', 0, 0 },

    /* A-first (32-47) */
    /* 32 */ { "AUU", "Isoleucine",     "Ile",  'I', 0, 0 },
    /* 33 */ { "AUC", "Isoleucine",     "Ile",  'I', 0, 0 },
    /* 34 */ { "AUA", "Isoleucine",     "Ile",  'I', 0, 0 },
    /* 35 */ { "AUG", "Methionine",     "Met",  'M', 0, 1 },
    /* 36 */ { "ACU", "Threonine",      "Thr",  'T', 0, 0 },
    /* 37 */ { "ACC", "Threonine",      "Thr",  'T', 0, 0 },
    /* 38 */ { "ACA", "Threonine",      "Thr",  'T', 0, 0 },
    /* 39 */ { "ACG", "Threonine",      "Thr",  'T', 0, 0 },
    /* 40 */ { "AAU", "Asparagine",     "Asn",  'N', 0, 0 },
    /* 41 */ { "AAC", "Asparagine",     "Asn",  'N', 0, 0 },
    /* 42 */ { "AAA", "Lysine",         "Lys",  'K', 0, 0 },
    /* 43 */ { "AAG", "Lysine",         "Lys",  'K', 0, 0 },
    /* 44 */ { "AGU", "Serine",         "Ser",  'S', 0, 0 },
    /* 45 */ { "AGC", "Serine",         "Ser",  'S', 0, 0 },
    /* 46 */ { "AGA", "Arginine",       "Arg",  'R', 0, 0 },
    /* 47 */ { "AGG", "Arginine",       "Arg",  'R', 0, 0 },

    /* G-first (48-63) */
    /* 48 */ { "GUU", "Valine",         "Val",  'V', 0, 0 },
    /* 49 */ { "GUC", "Valine",         "Val",  'V', 0, 0 },
    /* 50 */ { "GUA", "Valine",         "Val",  'V', 0, 0 },
    /* 51 */ { "GUG", "Valine",         "Val",  'V', 0, 0 },
    /* 52 */ { "GCU", "Alanine",        "Ala",  'A', 0, 0 },
    /* 53 */ { "GCC", "Alanine",        "Ala",  'A', 0, 0 },
    /* 54 */ { "GCA", "Alanine",        "Ala",  'A', 0, 0 },
    /* 55 */ { "GCG", "Alanine",        "Ala",  'A', 0, 0 },
    /* 56 */ { "GAU", "Aspartic acid",  "Asp",  'D', 0, 0 },
    /* 57 */ { "GAC", "Aspartic acid",  "Asp",  'D', 0, 0 },
    /* 58 */ { "GAA", "Glutamic acid",  "Glu",  'E', 0, 0 },
    /* 59 */ { "GAG", "Glutamic acid",  "Glu",  'E', 0, 0 },
    /* 60 */ { "GGU", "Glycine",        "Gly",  'G', 0, 0 },
    /* 61 */ { "GGC", "Glycine",        "Gly",  'G', 0, 0 },
    /* 62 */ { "GGA", "Glycine",        "Gly",  'G', 0, 0 },
    /* 63 */ { "GGG", "Glycine",        "Gly",  'G', 0, 0 }
};

/* ===== Helper: compute binary value from three bases ===== */

static int bases_to_binary(rna_base_t b1, rna_base_t b2, rna_base_t b3)
{
    return ((int)b1 << 4) | ((int)b2 << 2) | (int)b3;
}

/* ===== Helper: extract three bases from codon index ===== */

static void index_to_bases(int idx, rna_base_t *b1, rna_base_t *b2, rna_base_t *b3)
{
    *b1 = (rna_base_t)(idx / 16);
    *b2 = (rna_base_t)((idx / 4) % 4);
    *b3 = (rna_base_t)(idx % 4);
}

/* ===== Public API ===== */

codon_hexagram_t codon_hexagram_get(int codon_index)
{
    codon_hexagram_t result;

    if (codon_index < 0 || codon_index > 63) {
        result.codon_index = -1;
        result.base1 = RNA_U;
        result.base2 = RNA_U;
        result.base3 = RNA_U;
        result.codon_str = "?";
        result.amino_acid = "?";
        result.amino_abbrev = "?";
        result.amino_letter = '?';
        result.is_stop = 0;
        result.is_start = 0;
        result.binary_value = -1;
        result.hexagram_king_wen = -1;
        for (int i = 0; i < 6; i++) {
            result.hexagram_lines[i] = 0;
        }
        return result;
    }

    const codon_data_t *d = &CODON_TABLE[codon_index];

    rna_base_t b1, b2, b3;
    index_to_bases(codon_index, &b1, &b2, &b3);

    int binary = bases_to_binary(b1, b2, b3);

    result.codon_index = codon_index;
    result.base1 = b1;
    result.base2 = b2;
    result.base3 = b3;
    result.codon_str = d->codon_str;
    result.amino_acid = d->amino_acid;
    result.amino_abbrev = d->amino_abbrev;
    result.amino_letter = d->amino_letter;
    result.is_stop = d->is_stop;
    result.is_start = d->is_start;
    result.binary_value = binary;
    result.hexagram_king_wen = BINARY_TO_KING_WEN[binary];

    for (int i = 0; i < 6; i++) {
        result.hexagram_lines[i] = (binary >> i) & 1;
    }

    return result;
}

int codon_from_bases(rna_base_t b1, rna_base_t b2, rna_base_t b3)
{
    if (b1 < 0 || b1 >= RNA_BASE_COUNT) return -1;
    if (b2 < 0 || b2 >= RNA_BASE_COUNT) return -1;
    if (b3 < 0 || b3 >= RNA_BASE_COUNT) return -1;
    return (int)b1 * 16 + (int)b2 * 4 + (int)b3;
}

int codon_to_hexagram(int codon_index)
{
    if (codon_index < 0 || codon_index > 63) return -1;

    rna_base_t b1, b2, b3;
    index_to_bases(codon_index, &b1, &b2, &b3);
    int binary = bases_to_binary(b1, b2, b3);

    return BINARY_TO_KING_WEN[binary];
}

const char *hexagram_to_amino_acid(int hexagram_number)
{
    if (hexagram_number < 1 || hexagram_number > 64) return "?";

    /* Linear scan: find the codon whose King Wen number matches */
    for (int i = 0; i < 64; i++) {
        rna_base_t b1, b2, b3;
        index_to_bases(i, &b1, &b2, &b3);
        int binary = bases_to_binary(b1, b2, b3);
        if (BINARY_TO_KING_WEN[binary] == hexagram_number) {
            return CODON_TABLE[i].amino_acid;
        }
    }

    return "?";
}

const char *codon_amino_acid(int codon_index)
{
    if (codon_index < 0 || codon_index > 63) return "?";
    return CODON_TABLE[codon_index].amino_acid;
}

char codon_amino_letter(int codon_index)
{
    if (codon_index < 0 || codon_index > 63) return '?';
    return CODON_TABLE[codon_index].amino_letter;
}

int codon_to_binary(int codon_index)
{
    if (codon_index < 0 || codon_index > 63) return -1;

    rna_base_t b1, b2, b3;
    index_to_bases(codon_index, &b1, &b2, &b3);
    return bases_to_binary(b1, b2, b3);
}

int binary_to_codon(int binary_value)
{
    if (binary_value < 0 || binary_value > 63) return -1;

    /* Reverse the Schonberger mapping:
     * binary = (b1 << 4) | (b2 << 2) | b3
     * codon_index = b1*16 + b2*4 + b3
     * These are identical, so binary_value == codon_index */
    return binary_value;
}

int codon_count(void)
{
    return 64;
}

int amino_acid_distinct_count(void)
{
    /* Count distinct amino acid names, excluding "Stop" */
    const char *seen[64];
    int count = 0;

    for (int i = 0; i < 64; i++) {
        if (CODON_TABLE[i].is_stop) continue;

        int found = 0;
        for (int j = 0; j < count; j++) {
            if (strcmp(seen[j], CODON_TABLE[i].amino_acid) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            seen[count++] = CODON_TABLE[i].amino_acid;
        }
    }

    return count;
}

int amino_acid_degeneracy(const char *amino_acid)
{
    if (amino_acid == NULL) return 0;

    int count = 0;
    for (int i = 0; i < 64; i++) {
        if (strcmp(CODON_TABLE[i].amino_acid, amino_acid) == 0) {
            count++;
        }
    }
    return count;
}

const char *rna_base_name(rna_base_t base)
{
    if (base < 0 || base >= RNA_BASE_COUNT) return "?";
    return BASE_NAMES[base];
}

char rna_base_letter(rna_base_t base)
{
    if (base < 0 || base >= RNA_BASE_COUNT) return '?';
    return BASE_LETTERS[base];
}

int codon_stop_count(void)
{
    int count = 0;
    for (int i = 0; i < 64; i++) {
        if (CODON_TABLE[i].is_stop) count++;
    }
    return count;
}

int codon_start_count(void)
{
    int count = 0;
    for (int i = 0; i < 64; i++) {
        if (CODON_TABLE[i].is_start) count++;
    }
    return count;
}
