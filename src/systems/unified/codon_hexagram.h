/* codon_hexagram — DNA codon to I Ching hexagram structural map.
 * Pure lookup module: static const data, no malloc, no globals, no side effects.
 * Maps 64 DNA/RNA codons to 64 hexagrams via the Schonberger binary correspondence.
 * Source: Martin Schonberger, "The I Ching and the Genetic Code" (1973). */

#ifndef TIME_CODON_HEXAGRAM_H
#define TIME_CODON_HEXAGRAM_H

/* RNA bases (also represents DNA with U->T) */
typedef enum {
    RNA_U = 0,  /* Uracil (RNA) / Thymine (DNA) -- binary 00 */
    RNA_C = 1,  /* Cytosine -- binary 01 */
    RNA_A = 2,  /* Adenine -- binary 10 */
    RNA_G = 3,  /* Guanine -- binary 11 */
    RNA_BASE_COUNT = 4
} rna_base_t;

/* Complete codon-hexagram mapping entry */
typedef struct {
    int codon_index;           /* 0-63 (base1*16 + base2*4 + base3) */
    rna_base_t base1;          /* First base of triplet */
    rna_base_t base2;          /* Second base */
    rna_base_t base3;          /* Third base */
    const char *codon_str;     /* "UUU", "UUC", etc. */
    const char *amino_acid;    /* "Phenylalanine", "Stop", etc. */
    const char *amino_abbrev;  /* "Phe", "Stop", etc. */
    char amino_letter;         /* 'F', '*', etc. (single-letter code) */
    int is_stop;               /* 1 if stop codon (UAA, UAG, UGA) */
    int is_start;              /* 1 if start codon (AUG) */
    int binary_value;          /* 6-bit binary from base mapping */
    int hexagram_king_wen;     /* King Wen number (1-64) */
    int hexagram_lines[6];     /* 0=yin, 1=yang, [0]=bottom */
} codon_hexagram_t;

/* Get mapping entry by codon index (0-63). Returns entry with codon_index=-1 if invalid. */
codon_hexagram_t codon_hexagram_get(int codon_index);

/* Get codon index from three RNA bases. Returns -1 if invalid. */
int codon_from_bases(rna_base_t b1, rna_base_t b2, rna_base_t b3);

/* Map codon index (0-63) to King Wen hexagram number (1-64). Returns -1 if invalid. */
int codon_to_hexagram(int codon_index);

/* Map King Wen hexagram (1-64) to amino acid name. Returns "?" if invalid. */
const char *hexagram_to_amino_acid(int hexagram_number);

/* Get amino acid name from codon index. Returns "?" if invalid. */
const char *codon_amino_acid(int codon_index);

/* Get amino acid single-letter code from codon index. Returns '?' if invalid. */
char codon_amino_letter(int codon_index);

/* Convert codon index to 6-bit binary value via Schonberger mapping. Returns -1 if invalid. */
int codon_to_binary(int codon_index);

/* Convert 6-bit binary to codon index. Returns -1 if invalid (>63). */
int binary_to_codon(int binary_value);

/* Total codons (always 64). */
int codon_count(void);

/* Number of distinct amino acids encoded (20, not counting stop). */
int amino_acid_distinct_count(void);

/* Degeneracy: how many codons encode the same amino acid? Returns 0 if not found. */
int amino_acid_degeneracy(const char *amino_acid);

/* RNA base name string. Returns "?" for invalid. */
const char *rna_base_name(rna_base_t base);

/* RNA base letter. Returns '?' for invalid. */
char rna_base_letter(rna_base_t base);

/* How many stop codons? (always 3) */
int codon_stop_count(void);

/* How many start codons? (always 1: AUG) */
int codon_start_count(void);

#endif /* TIME_CODON_HEXAGRAM_H */
