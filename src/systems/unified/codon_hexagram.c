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

/* ===== Binary to King Wen lookup (Schonberger/Leibniz convention) =====
 * Index = 6-bit binary value where bit 0 = line 1 (bottom), bit 5 = line 6 (top).
 * Lower trigram = bits 0-2, upper trigram = bits 3-5.
 * Value = King Wen hexagram number (1-64).
 * Trigram encoding (natural binary, bottom-to-top):
 *   Earth=0, Thunder=1, Water=2, Lake=3,
 *   Mountain=4, Fire=5, Wind=6, Heaven=7.
 * Derived from verified codon-to-hexagram correspondences (Schonberger pp. 63-67, 142). */

static const int BINARY_TO_KING_WEN[64] = {
    /*  0  000000  Earth/Earth     */  2,
    /*  1  000001  Earth/Thunder   */ 24,
    /*  2  000010  Earth/Water     */  7,
    /*  3  000011  Earth/Lake      */ 19,
    /*  4  000100  Earth/Mountain  */ 15,
    /*  5  000101  Earth/Fire      */ 36,
    /*  6  000110  Earth/Wind      */ 46,
    /*  7  000111  Earth/Heaven    */ 11,
    /*  8  001000  Thunder/Earth   */ 16,
    /*  9  001001  Thunder/Thunder */ 51,
    /* 10  001010  Thunder/Water   */ 40,
    /* 11  001011  Thunder/Lake    */ 54,
    /* 12  001100  Thunder/Mountain*/ 62,
    /* 13  001101  Thunder/Fire    */ 55,
    /* 14  001110  Thunder/Wind    */ 32,
    /* 15  001111  Thunder/Heaven  */ 34,
    /* 16  010000  Water/Earth     */  8,
    /* 17  010001  Water/Thunder   */  3,
    /* 18  010010  Water/Water     */ 29,
    /* 19  010011  Water/Lake      */ 60,
    /* 20  010100  Water/Mountain  */ 39,
    /* 21  010101  Water/Fire      */ 63,
    /* 22  010110  Water/Wind      */ 48,
    /* 23  010111  Water/Heaven    */  5,
    /* 24  011000  Lake/Earth      */ 45,
    /* 25  011001  Lake/Thunder    */ 17,
    /* 26  011010  Lake/Water      */ 47,
    /* 27  011011  Lake/Lake       */ 58,
    /* 28  011100  Lake/Mountain   */ 31,
    /* 29  011101  Lake/Fire       */ 49,
    /* 30  011110  Lake/Wind       */ 28,
    /* 31  011111  Lake/Heaven     */ 43,
    /* 32  100000  Mountain/Earth  */ 23,
    /* 33  100001  Mountain/Thunder*/ 27,
    /* 34  100010  Mountain/Water  */  4,
    /* 35  100011  Mountain/Lake   */ 41,
    /* 36  100100  Mountain/Mountain*/ 52,
    /* 37  100101  Mountain/Fire   */ 22,
    /* 38  100110  Mountain/Wind   */ 18,
    /* 39  100111  Mountain/Heaven */ 26,
    /* 40  101000  Fire/Earth      */ 35,
    /* 41  101001  Fire/Thunder    */ 21,
    /* 42  101010  Fire/Water      */ 64,
    /* 43  101011  Fire/Lake       */ 38,
    /* 44  101100  Fire/Mountain   */ 56,
    /* 45  101101  Fire/Fire       */ 30,
    /* 46  101110  Fire/Wind       */ 50,
    /* 47  101111  Fire/Heaven     */ 14,
    /* 48  110000  Wind/Earth      */ 20,
    /* 49  110001  Wind/Thunder    */ 42,
    /* 50  110010  Wind/Water      */ 59,
    /* 51  110011  Wind/Lake       */ 61,
    /* 52  110100  Wind/Mountain   */ 53,
    /* 53  110101  Wind/Fire       */ 37,
    /* 54  110110  Wind/Wind       */ 57,
    /* 55  110111  Wind/Heaven     */  9,
    /* 56  111000  Heaven/Earth    */ 12,
    /* 57  111001  Heaven/Thunder  */ 25,
    /* 58  111010  Heaven/Water    */  6,
    /* 59  111011  Heaven/Lake     */ 10,
    /* 60  111100  Heaven/Mountain */ 33,
    /* 61  111101  Heaven/Fire     */ 13,
    /* 62  111110  Heaven/Wind     */ 44,
    /* 63  111111  Heaven/Heaven   */  1
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

/* ===== Helper: compute Schonberger binary value from three bases =====
 * Maps our enum (U=0,C=1,A=2,G=3) to Schonberger's binary (U=00,C=01,G=10,A=11).
 * Bit order: base3 is most significant (top of hexagram), base1 is least significant.
 * Source: Schonberger pp. 70-72. */

static int bases_to_binary(rna_base_t b1, rna_base_t b2, rna_base_t b3)
{
    /* Our enum -> Schonberger binary: U=0, C=1, A=3, G=2 (swap A<->G) */
    static const int SCHON[4] = {0, 1, 3, 2};
    int sb1 = SCHON[(int)b1];
    int sb2 = SCHON[(int)b2];
    int sb3 = SCHON[(int)b3];
    /* base3 = most significant bits (top of hexagram) */
    return (sb3 << 4) | (sb2 << 2) | sb1;
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
     * binary = (SCHON[b3] << 4) | (SCHON[b2] << 2) | SCHON[b1]
     * Extract Schonberger values from bits, then map back to our enum */
    static const int REVERSE[4] = {0, 1, 3, 2}; /* Schon->enum: 0->U, 1->C, 2->G, 3->A */
    int sb1 = binary_value & 3;
    int sb2 = (binary_value >> 2) & 3;
    int sb3 = (binary_value >> 4) & 3;
    int b1 = REVERSE[sb1];
    int b2 = REVERSE[sb2];
    int b3 = REVERSE[sb3];
    return b1 * 16 + b2 * 4 + b3;
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
