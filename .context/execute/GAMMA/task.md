# Task: DNA-Hexagram Structural Map

**Agent**: GAMMA
**Roadmap Reference**: Track 27.4 — "Agent: DNA-Hexagram Structural Map (Agent B)"
**Date**: 2026-03-06
**Status**: CLAIMED

## Goal

Pure data+computation module mapping 64 DNA codons to 64 I Ching hexagrams via the Schönberger binary correspondence. Both systems exhaustively enumerate 2^6 = 64 states: a codon is 3 bases × 2 bits = 6 bits; a hexagram is 6 lines × 1 bit = 6 bits. Provides the complete genetic code table (64 codons → 20 amino acids + stop), the binary mapping to hexagram lines, and King Wen number lookup. This is combinatorics, not mysticism.

## READ FIRST

- `src/systems/iching/iching.h` — hexagram_t, king_wen, lines[6], iching_lines_to_king_wen
- `src/systems/unified/structural_map.h` — SYS_DNA_CODONS, concordance system

## Files to Create

- `src/systems/unified/codon_hexagram.h`
- `src/systems/unified/codon_hexagram.c`
- `tests/systems/unified/test_codon_hexagram.c`

## API

```c
#ifndef TIME_CODON_HEXAGRAM_H
#define TIME_CODON_HEXAGRAM_H

/* RNA bases (also represents DNA with U→T) */
typedef enum {
    RNA_U = 0,  /* Uracil (RNA) / Thymine (DNA) — binary 00 */
    RNA_C = 1,  /* Cytosine — binary 01 */
    RNA_A = 2,  /* Adenine — binary 10 */
    RNA_G = 3,  /* Guanine — binary 11 */
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
```

## Physical Data

### Standard Genetic Code (RNA codons → amino acids)

64 codons organized by first base:
- U-starting (16): UUU/UUC→Phe, UUA/UUG→Leu, UCx→Ser, UAU/UAC→Tyr, UAA/UAG→Stop, UGU/UGC→Cys, UGA→Stop, UGG→Trp
- C-starting (16): CUx→Leu, CCx→Pro, CAU/CAC→His, CAA/CAG→Gln, CGx→Arg
- A-starting (16): AUU/AUC/AUA→Ile, AUG→Met(Start), ACx→Thr, AAU/AAC→Asn, AAA/AAG→Lys, AGU/AGC→Ser, AGA/AGG→Arg
- G-starting (16): GUx→Val, GCx→Ala, GAU/GAC→Asp, GAA/GAG→Glu, GGx→Gly

### Schönberger Binary Mapping

Each RNA base → 2 bits (following the yin/yang pairing):
- U → 00 (old yin)
- C → 01 (young yang)
- A → 10 (young yin)
- G → 11 (old yang)

Codon triplet → 6 bits → hexagram lines (bottom to top).
6-bit binary → King Wen number via lookup table (King Wen sequence is non-trivial ordering).

### King Wen Binary Lookup

The 64 binary values (000000 to 111111) map to King Wen numbers. This requires a 64-entry lookup table since the King Wen ordering is traditional, not computational.

## DONE WHEN

- [ ] All 64 codons with correct amino acid assignments
- [ ] 3 stop codons (UAA, UAG, UGA) and 1 start codon (AUG) identified
- [ ] Schönberger binary mapping (base → 2 bits)
- [ ] Binary-to-King-Wen hexagram lookup (64 entries)
- [ ] Degeneracy calculation for amino acids
- [ ] Bidirectional: codon→hexagram and hexagram→amino_acid
- [ ] >= 30 tests
- [ ] All tests pass with zero warnings
- [ ] Purity: no malloc, no globals, no side effects
- [ ] Compiles: `gcc -Wall -Wextra -Werror -std=c11 -pedantic`

## Constraints

- C11, `-Wall -Wextra -Werror -std=c11 -pedantic`
- No malloc, no globals, no side effects
- Standalone module (no compile-time dependencies on iching.c)
- Data source: Standard genetic code (public domain biology), Schönberger mapping (1973)
