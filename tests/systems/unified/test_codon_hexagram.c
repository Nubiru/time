/* test_codon_hexagram.c — DNA-Hexagram Structural Map tests
 * TDD RED phase: tests written before implementation.
 * Tests the Schonberger codon-to-hexagram mapping module.
 * Source: Martin Schonberger, "The I Ching and the Genetic Code" (1973). */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/codon_hexagram.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== codon_count ===== */

void test_codon_count_is_64(void)
{
    TEST_ASSERT_EQUAL_INT(64, codon_count());
}

/* ===== codon_from_bases ===== */

void test_codon_from_bases_UUU_is_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, codon_from_bases(RNA_U, RNA_U, RNA_U));
}

void test_codon_from_bases_UUC_is_1(void)
{
    TEST_ASSERT_EQUAL_INT(1, codon_from_bases(RNA_U, RNA_U, RNA_C));
}

void test_codon_from_bases_GGG_is_63(void)
{
    TEST_ASSERT_EQUAL_INT(63, codon_from_bases(RNA_G, RNA_G, RNA_G));
}

void test_codon_from_bases_AUG_is_35(void)
{
    /* Start codon AUG: A=2, U=0, G=3 -> 2*16 + 0*4 + 3 = 35 */
    TEST_ASSERT_EQUAL_INT(35, codon_from_bases(RNA_A, RNA_U, RNA_G));
}

void test_codon_from_bases_invalid_returns_neg1(void)
{
    TEST_ASSERT_EQUAL_INT(-1, codon_from_bases((rna_base_t)4, RNA_U, RNA_U));
    TEST_ASSERT_EQUAL_INT(-1, codon_from_bases(RNA_U, (rna_base_t)5, RNA_U));
    TEST_ASSERT_EQUAL_INT(-1, codon_from_bases(RNA_U, RNA_U, (rna_base_t)99));
}

/* ===== codon_hexagram_get — basic fields ===== */

void test_get_UUU_codon_index(void)
{
    codon_hexagram_t e = codon_hexagram_get(0);
    TEST_ASSERT_EQUAL_INT(0, e.codon_index);
}

void test_get_UUU_codon_string(void)
{
    codon_hexagram_t e = codon_hexagram_get(0);
    TEST_ASSERT_EQUAL_STRING("UUU", e.codon_str);
}

void test_get_UUU_amino_acid(void)
{
    codon_hexagram_t e = codon_hexagram_get(0);
    TEST_ASSERT_EQUAL_STRING("Phenylalanine", e.amino_acid);
}

void test_get_UUU_amino_abbrev(void)
{
    codon_hexagram_t e = codon_hexagram_get(0);
    TEST_ASSERT_EQUAL_STRING("Phe", e.amino_abbrev);
}

void test_get_UUU_amino_letter(void)
{
    codon_hexagram_t e = codon_hexagram_get(0);
    TEST_ASSERT_EQUAL_CHAR('F', e.amino_letter);
}

void test_get_UUU_bases(void)
{
    codon_hexagram_t e = codon_hexagram_get(0);
    TEST_ASSERT_EQUAL_INT(RNA_U, e.base1);
    TEST_ASSERT_EQUAL_INT(RNA_U, e.base2);
    TEST_ASSERT_EQUAL_INT(RNA_U, e.base3);
}

void test_get_GGG_index_63(void)
{
    codon_hexagram_t e = codon_hexagram_get(63);
    TEST_ASSERT_EQUAL_INT(63, e.codon_index);
    TEST_ASSERT_EQUAL_STRING("GGG", e.codon_str);
    TEST_ASSERT_EQUAL_STRING("Glycine", e.amino_acid);
    TEST_ASSERT_EQUAL_CHAR('G', e.amino_letter);
}

void test_get_invalid_returns_neg1_index(void)
{
    codon_hexagram_t e = codon_hexagram_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, e.codon_index);

    codon_hexagram_t e2 = codon_hexagram_get(64);
    TEST_ASSERT_EQUAL_INT(-1, e2.codon_index);
}

/* ===== Stop and Start codons ===== */

void test_UAA_is_stop(void)
{
    /* UAA = index 10 (U=0, A=2, A=2 -> 0*16+2*4+2 = 10) */
    codon_hexagram_t e = codon_hexagram_get(10);
    TEST_ASSERT_EQUAL_INT(1, e.is_stop);
    TEST_ASSERT_EQUAL_INT(0, e.is_start);
    TEST_ASSERT_EQUAL_STRING("Stop", e.amino_acid);
    TEST_ASSERT_EQUAL_CHAR('*', e.amino_letter);
}

void test_UAG_is_stop(void)
{
    /* UAG = index 11 (0*16+2*4+3 = 11) */
    codon_hexagram_t e = codon_hexagram_get(11);
    TEST_ASSERT_EQUAL_INT(1, e.is_stop);
}

void test_UGA_is_stop(void)
{
    /* UGA = index 14 (0*16+3*4+2 = 14) */
    codon_hexagram_t e = codon_hexagram_get(14);
    TEST_ASSERT_EQUAL_INT(1, e.is_stop);
}

void test_AUG_is_start(void)
{
    /* AUG = index 35 (2*16+0*4+3 = 35) */
    codon_hexagram_t e = codon_hexagram_get(35);
    TEST_ASSERT_EQUAL_INT(1, e.is_start);
    TEST_ASSERT_EQUAL_INT(0, e.is_stop);
    TEST_ASSERT_EQUAL_STRING("Methionine", e.amino_acid);
    TEST_ASSERT_EQUAL_STRING("Met", e.amino_abbrev);
    TEST_ASSERT_EQUAL_CHAR('M', e.amino_letter);
}

void test_stop_count_is_3(void)
{
    TEST_ASSERT_EQUAL_INT(3, codon_stop_count());
}

void test_start_count_is_1(void)
{
    TEST_ASSERT_EQUAL_INT(1, codon_start_count());
}

/* ===== Binary mapping (Schonberger) ===== */

void test_codon_to_binary_UUU_is_0(void)
{
    /* UUU: Schon U=00 -> (00<<4)|(00<<2)|00 = 0 */
    TEST_ASSERT_EQUAL_INT(0, codon_to_binary(0));
}

void test_codon_to_binary_GGG_is_42(void)
{
    /* GGG: Schon G=10 -> (10<<4)|(10<<2)|10 = 101010 = 42 */
    TEST_ASSERT_EQUAL_INT(42, codon_to_binary(63));
}

void test_codon_to_binary_AAA_is_63(void)
{
    /* AAA: Schon A=11 -> (11<<4)|(11<<2)|11 = 111111 = 63 */
    TEST_ASSERT_EQUAL_INT(63, codon_to_binary(42));
}

void test_codon_to_binary_AUG_is_35(void)
{
    /* AUG: Schon A=11,U=00,G=10 -> (10<<4)|(00<<2)|11 = 100011 = 35 */
    TEST_ASSERT_EQUAL_INT(35, codon_to_binary(35));
}

void test_codon_to_binary_UUC_is_16(void)
{
    /* UUC: Schon U=00,U=00,C=01 -> (01<<4)|(00<<2)|00 = 010000 = 16 */
    TEST_ASSERT_EQUAL_INT(16, codon_to_binary(1));
}

void test_codon_to_binary_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, codon_to_binary(-1));
    TEST_ASSERT_EQUAL_INT(-1, codon_to_binary(64));
}

void test_binary_to_codon_roundtrip(void)
{
    /* binary_to_codon is inverse of codon_to_binary */
    for (int i = 0; i < 64; i++) {
        int binary = codon_to_binary(i);
        TEST_ASSERT_TRUE(binary >= 0);
        TEST_ASSERT_TRUE(binary < 64);
        int back = binary_to_codon(binary);
        TEST_ASSERT_EQUAL_INT(i, back);
    }
}

void test_binary_to_codon_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, binary_to_codon(-1));
    TEST_ASSERT_EQUAL_INT(-1, binary_to_codon(64));
}

/* ===== Hexagram lines ===== */

void test_UUU_hexagram_lines_all_yin(void)
{
    /* UUU: Schon binary 0 -> all lines are 0 (yin) -> KW#2 K'un */
    codon_hexagram_t e = codon_hexagram_get(0);
    for (int i = 0; i < 6; i++) {
        TEST_ASSERT_EQUAL_INT(0, e.hexagram_lines[i]);
    }
}

void test_AAA_hexagram_lines_all_yang(void)
{
    /* AAA: Schon binary 63 (111111) -> all lines are 1 (yang) -> KW#1 Ch'ien */
    codon_hexagram_t e = codon_hexagram_get(42); /* AAA = index 42 */
    for (int i = 0; i < 6; i++) {
        TEST_ASSERT_EQUAL_INT(1, e.hexagram_lines[i]);
    }
}

void test_GGG_hexagram_lines(void)
{
    /* GGG: Schon binary 42 (101010) -> alternating yin/yang -> KW#64 Wei Chi */
    codon_hexagram_t e = codon_hexagram_get(63); /* GGG = index 63 */
    TEST_ASSERT_EQUAL_INT(0, e.hexagram_lines[0]);
    TEST_ASSERT_EQUAL_INT(1, e.hexagram_lines[1]);
    TEST_ASSERT_EQUAL_INT(0, e.hexagram_lines[2]);
    TEST_ASSERT_EQUAL_INT(1, e.hexagram_lines[3]);
    TEST_ASSERT_EQUAL_INT(0, e.hexagram_lines[4]);
    TEST_ASSERT_EQUAL_INT(1, e.hexagram_lines[5]);
}

void test_AUG_hexagram_lines(void)
{
    /* AUG: Schon binary 35 (100011) -> Mountain/Lake -> KW#41 Sun (Decrease)
     * lines[0] = bit 0 = 1
     * lines[1] = bit 1 = 1
     * lines[2] = bit 2 = 0
     * lines[3] = bit 3 = 0
     * lines[4] = bit 4 = 0
     * lines[5] = bit 5 = 1 */
    codon_hexagram_t e = codon_hexagram_get(35);
    TEST_ASSERT_EQUAL_INT(1, e.hexagram_lines[0]);
    TEST_ASSERT_EQUAL_INT(1, e.hexagram_lines[1]);
    TEST_ASSERT_EQUAL_INT(0, e.hexagram_lines[2]);
    TEST_ASSERT_EQUAL_INT(0, e.hexagram_lines[3]);
    TEST_ASSERT_EQUAL_INT(0, e.hexagram_lines[4]);
    TEST_ASSERT_EQUAL_INT(1, e.hexagram_lines[5]);
}

/* ===== King Wen mapping (Schonberger verified) ===== */

void test_UUU_king_wen_is_2(void)
{
    /* UUU: Schon binary 0 (Earth/Earth) -> KW 2 (K'un/Receptive) */
    codon_hexagram_t e = codon_hexagram_get(0);
    TEST_ASSERT_EQUAL_INT(2, e.hexagram_king_wen);
}

void test_AAA_king_wen_is_1(void)
{
    /* AAA: Schon binary 63 (Heaven/Heaven) -> KW 1 (Ch'ien/Creative) */
    codon_hexagram_t e = codon_hexagram_get(42); /* AAA = index 42 */
    TEST_ASSERT_EQUAL_INT(1, e.hexagram_king_wen);
}

void test_GGG_king_wen_is_64(void)
{
    /* GGG: Schon binary 42 (Fire/Water) -> KW 64 (Wei Chi/Before Completion) */
    codon_hexagram_t e = codon_hexagram_get(63); /* GGG = index 63 */
    TEST_ASSERT_EQUAL_INT(64, e.hexagram_king_wen);
}

void test_codon_to_hexagram_index0(void)
{
    /* UUU -> KW 2 */
    TEST_ASSERT_EQUAL_INT(2, codon_to_hexagram(0));
}

void test_codon_to_hexagram_index63(void)
{
    /* GGG -> KW 64 */
    TEST_ASSERT_EQUAL_INT(64, codon_to_hexagram(63));
}

void test_codon_to_hexagram_AUG(void)
{
    /* AUG = index 35, Schon binary 35 -> Mountain/Lake -> KW 41 (Sun/Decrease) */
    TEST_ASSERT_EQUAL_INT(41, codon_to_hexagram(35));
}

void test_codon_to_hexagram_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, codon_to_hexagram(-1));
    TEST_ASSERT_EQUAL_INT(-1, codon_to_hexagram(64));
}

void test_all_64_king_wen_values_present(void)
{
    /* Every King Wen number 1-64 must appear exactly once across 64 codons */
    int seen[65] = {0}; /* index 0 unused */
    for (int i = 0; i < 64; i++) {
        int kw = codon_to_hexagram(i);
        TEST_ASSERT_TRUE(kw >= 1);
        TEST_ASSERT_TRUE(kw <= 64);
        seen[kw]++;
    }
    for (int kw = 1; kw <= 64; kw++) {
        TEST_ASSERT_EQUAL_INT(1, seen[kw]);
    }
}

/* ===== Schonberger semantic correspondences (pp. 142-143) ===== */

void test_UAA_stop_maps_to_retreat(void)
{
    /* UAA (Stop/Ochre) -> KW 33 (Tun/Retreat) — Schonberger's key example */
    TEST_ASSERT_EQUAL_INT(33, codon_to_hexagram(10)); /* UAA = index 10 */
}

void test_UGA_stop_maps_to_standstill(void)
{
    /* UGA (Stop) -> KW 12 (P'i/Standstill) — stop = standstill */
    TEST_ASSERT_EQUAL_INT(12, codon_to_hexagram(14)); /* UGA = index 14 */
}

void test_UAG_stop_maps_to_wanderer(void)
{
    /* UAG (Stop/Amber) -> KW 56 (Lu/Wanderer) */
    TEST_ASSERT_EQUAL_INT(56, codon_to_hexagram(11)); /* UAG = index 11 */
}

void test_UGG_trp_maps_to_progress(void)
{
    /* UGG (Tryptophan, essential) -> KW 35 (Chin/Progress) */
    TEST_ASSERT_EQUAL_INT(35, codon_to_hexagram(15)); /* UGG = index 15 */
}

/* ===== hexagram_to_amino_acid ===== */

void test_hexagram_to_amino_acid_kw2(void)
{
    /* KW 2 <- Schon binary 0 <- UUU -> Phenylalanine */
    TEST_ASSERT_EQUAL_STRING("Phenylalanine", hexagram_to_amino_acid(2));
}

void test_hexagram_to_amino_acid_kw1(void)
{
    /* KW 1 <- Schon binary 63 <- AAA -> Lysine */
    TEST_ASSERT_EQUAL_STRING("Lysine", hexagram_to_amino_acid(1));
}

void test_hexagram_to_amino_acid_kw64(void)
{
    /* KW 64 <- Schon binary 42 <- GGG -> Glycine */
    TEST_ASSERT_EQUAL_STRING("Glycine", hexagram_to_amino_acid(64));
}

void test_hexagram_to_amino_acid_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", hexagram_to_amino_acid(0));
    TEST_ASSERT_EQUAL_STRING("?", hexagram_to_amino_acid(65));
}

/* ===== codon_amino_acid / codon_amino_letter ===== */

void test_codon_amino_acid_index0(void)
{
    TEST_ASSERT_EQUAL_STRING("Phenylalanine", codon_amino_acid(0));
}

void test_codon_amino_acid_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", codon_amino_acid(-1));
}

void test_codon_amino_letter_index0(void)
{
    TEST_ASSERT_EQUAL_CHAR('F', codon_amino_letter(0));
}

void test_codon_amino_letter_invalid(void)
{
    TEST_ASSERT_EQUAL_CHAR('?', codon_amino_letter(-1));
}

/* ===== amino_acid_distinct_count ===== */

void test_amino_acid_distinct_count_is_20(void)
{
    TEST_ASSERT_EQUAL_INT(20, amino_acid_distinct_count());
}

/* ===== amino_acid_degeneracy ===== */

void test_degeneracy_methionine_is_1(void)
{
    /* Only AUG encodes Met */
    TEST_ASSERT_EQUAL_INT(1, amino_acid_degeneracy("Methionine"));
}

void test_degeneracy_tryptophan_is_1(void)
{
    /* Only UGG encodes Trp */
    TEST_ASSERT_EQUAL_INT(1, amino_acid_degeneracy("Tryptophan"));
}

void test_degeneracy_leucine_is_6(void)
{
    /* UUA, UUG, CUU, CUC, CUA, CUG */
    TEST_ASSERT_EQUAL_INT(6, amino_acid_degeneracy("Leucine"));
}

void test_degeneracy_serine_is_6(void)
{
    /* UCU, UCC, UCA, UCG, AGU, AGC */
    TEST_ASSERT_EQUAL_INT(6, amino_acid_degeneracy("Serine"));
}

void test_degeneracy_stop_is_3(void)
{
    TEST_ASSERT_EQUAL_INT(3, amino_acid_degeneracy("Stop"));
}

void test_degeneracy_unknown_is_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, amino_acid_degeneracy("Nonexistent"));
}

/* ===== rna_base_name / rna_base_letter ===== */

void test_rna_base_name_U(void)
{
    TEST_ASSERT_EQUAL_STRING("Uracil", rna_base_name(RNA_U));
}

void test_rna_base_name_C(void)
{
    TEST_ASSERT_EQUAL_STRING("Cytosine", rna_base_name(RNA_C));
}

void test_rna_base_name_A(void)
{
    TEST_ASSERT_EQUAL_STRING("Adenine", rna_base_name(RNA_A));
}

void test_rna_base_name_G(void)
{
    TEST_ASSERT_EQUAL_STRING("Guanine", rna_base_name(RNA_G));
}

void test_rna_base_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", rna_base_name((rna_base_t)4));
}

void test_rna_base_letter_U(void)
{
    TEST_ASSERT_EQUAL_CHAR('U', rna_base_letter(RNA_U));
}

void test_rna_base_letter_G(void)
{
    TEST_ASSERT_EQUAL_CHAR('G', rna_base_letter(RNA_G));
}

void test_rna_base_letter_invalid(void)
{
    TEST_ASSERT_EQUAL_CHAR('?', rna_base_letter((rna_base_t)10));
}

/* ===== Specific codon spot-checks ===== */

void test_UCU_is_serine(void)
{
    /* UCU = index 4 (0*16+1*4+0 = 4) */
    codon_hexagram_t e = codon_hexagram_get(4);
    TEST_ASSERT_EQUAL_STRING("UCU", e.codon_str);
    TEST_ASSERT_EQUAL_STRING("Serine", e.amino_acid);
    TEST_ASSERT_EQUAL_CHAR('S', e.amino_letter);
}

void test_CAU_is_histidine(void)
{
    /* CAU = index 24 (1*16+2*4+0 = 24) */
    codon_hexagram_t e = codon_hexagram_get(24);
    TEST_ASSERT_EQUAL_STRING("CAU", e.codon_str);
    TEST_ASSERT_EQUAL_STRING("Histidine", e.amino_acid);
    TEST_ASSERT_EQUAL_CHAR('H', e.amino_letter);
}

void test_GAU_is_aspartate(void)
{
    /* GAU = index 56 (3*16+2*4+0 = 56) */
    codon_hexagram_t e = codon_hexagram_get(56);
    TEST_ASSERT_EQUAL_STRING("GAU", e.codon_str);
    TEST_ASSERT_EQUAL_STRING("Aspartic acid", e.amino_acid);
    TEST_ASSERT_EQUAL_CHAR('D', e.amino_letter);
}

void test_UGG_is_tryptophan(void)
{
    /* UGG = index 15 (0*16+3*4+3 = 15) */
    codon_hexagram_t e = codon_hexagram_get(15);
    TEST_ASSERT_EQUAL_STRING("UGG", e.codon_str);
    TEST_ASSERT_EQUAL_STRING("Tryptophan", e.amino_acid);
    TEST_ASSERT_EQUAL_CHAR('W', e.amino_letter);
}

/* ===== King Wen spot-checks (Schonberger verified) ===== */

void test_kw_UAU_is_62(void)
{
    /* UAU (Tyr): Schon U=0,A=3,U=0 -> binary=(0<<4)|(3<<2)|0 = 12
     * KW[12] = 62 (Hsiao Kuo/Small Preponderance) */
    TEST_ASSERT_EQUAL_INT(62, codon_to_hexagram(8)); /* UAU = index 8 */
}

void test_kw_AAA_is_1(void)
{
    /* AAA (Lys): Schon A=3 -> binary=(3<<4)|(3<<2)|3 = 63
     * KW[63] = 1 (Ch'ien/Creative) — all yang = Creative */
    TEST_ASSERT_EQUAL_INT(1, codon_to_hexagram(42)); /* AAA = index 42 */
}

void test_kw_GAU_is_32(void)
{
    /* GAU (Asp): Schon G=2,A=3,U=0 -> binary=(0<<4)|(3<<2)|2 = 14
     * KW[14] = 32 (Heng/Duration) */
    TEST_ASSERT_EQUAL_INT(32, codon_to_hexagram(56)); /* GAU = index 56 */
}

/* ===== Comprehensive binary bijection check ===== */

void test_binary_mapping_is_bijection(void)
{
    /* All 64 binary values 0-63 must appear exactly once */
    int seen[64] = {0};
    for (int i = 0; i < 64; i++) {
        int b = codon_to_binary(i);
        TEST_ASSERT_TRUE(b >= 0);
        TEST_ASSERT_TRUE(b < 64);
        seen[b]++;
    }
    for (int b = 0; b < 64; b++) {
        TEST_ASSERT_EQUAL_INT(1, seen[b]);
    }
}

/* ===== Runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* codon_count */
    RUN_TEST(test_codon_count_is_64);

    /* codon_from_bases */
    RUN_TEST(test_codon_from_bases_UUU_is_0);
    RUN_TEST(test_codon_from_bases_UUC_is_1);
    RUN_TEST(test_codon_from_bases_GGG_is_63);
    RUN_TEST(test_codon_from_bases_AUG_is_35);
    RUN_TEST(test_codon_from_bases_invalid_returns_neg1);

    /* codon_hexagram_get */
    RUN_TEST(test_get_UUU_codon_index);
    RUN_TEST(test_get_UUU_codon_string);
    RUN_TEST(test_get_UUU_amino_acid);
    RUN_TEST(test_get_UUU_amino_abbrev);
    RUN_TEST(test_get_UUU_amino_letter);
    RUN_TEST(test_get_UUU_bases);
    RUN_TEST(test_get_GGG_index_63);
    RUN_TEST(test_get_invalid_returns_neg1_index);

    /* Stop/Start codons */
    RUN_TEST(test_UAA_is_stop);
    RUN_TEST(test_UAG_is_stop);
    RUN_TEST(test_UGA_is_stop);
    RUN_TEST(test_AUG_is_start);
    RUN_TEST(test_stop_count_is_3);
    RUN_TEST(test_start_count_is_1);

    /* Binary mapping */
    RUN_TEST(test_codon_to_binary_UUU_is_0);
    RUN_TEST(test_codon_to_binary_GGG_is_42);
    RUN_TEST(test_codon_to_binary_AAA_is_63);
    RUN_TEST(test_codon_to_binary_AUG_is_35);
    RUN_TEST(test_codon_to_binary_UUC_is_16);
    RUN_TEST(test_codon_to_binary_invalid);
    RUN_TEST(test_binary_to_codon_roundtrip);
    RUN_TEST(test_binary_to_codon_invalid);

    /* Hexagram lines */
    RUN_TEST(test_UUU_hexagram_lines_all_yin);
    RUN_TEST(test_AAA_hexagram_lines_all_yang);
    RUN_TEST(test_GGG_hexagram_lines);
    RUN_TEST(test_AUG_hexagram_lines);

    /* King Wen mapping (Schonberger verified) */
    RUN_TEST(test_UUU_king_wen_is_2);
    RUN_TEST(test_AAA_king_wen_is_1);
    RUN_TEST(test_GGG_king_wen_is_64);
    RUN_TEST(test_codon_to_hexagram_index0);
    RUN_TEST(test_codon_to_hexagram_index63);
    RUN_TEST(test_codon_to_hexagram_AUG);
    RUN_TEST(test_codon_to_hexagram_invalid);
    RUN_TEST(test_all_64_king_wen_values_present);

    /* Schonberger semantic correspondences */
    RUN_TEST(test_UAA_stop_maps_to_retreat);
    RUN_TEST(test_UGA_stop_maps_to_standstill);
    RUN_TEST(test_UAG_stop_maps_to_wanderer);
    RUN_TEST(test_UGG_trp_maps_to_progress);

    /* hexagram_to_amino_acid */
    RUN_TEST(test_hexagram_to_amino_acid_kw2);
    RUN_TEST(test_hexagram_to_amino_acid_kw1);
    RUN_TEST(test_hexagram_to_amino_acid_kw64);
    RUN_TEST(test_hexagram_to_amino_acid_invalid);

    /* codon_amino_acid / codon_amino_letter */
    RUN_TEST(test_codon_amino_acid_index0);
    RUN_TEST(test_codon_amino_acid_invalid);
    RUN_TEST(test_codon_amino_letter_index0);
    RUN_TEST(test_codon_amino_letter_invalid);

    /* amino_acid_distinct_count */
    RUN_TEST(test_amino_acid_distinct_count_is_20);

    /* Degeneracy */
    RUN_TEST(test_degeneracy_methionine_is_1);
    RUN_TEST(test_degeneracy_tryptophan_is_1);
    RUN_TEST(test_degeneracy_leucine_is_6);
    RUN_TEST(test_degeneracy_serine_is_6);
    RUN_TEST(test_degeneracy_stop_is_3);
    RUN_TEST(test_degeneracy_unknown_is_0);

    /* RNA base names */
    RUN_TEST(test_rna_base_name_U);
    RUN_TEST(test_rna_base_name_C);
    RUN_TEST(test_rna_base_name_A);
    RUN_TEST(test_rna_base_name_G);
    RUN_TEST(test_rna_base_name_invalid);
    RUN_TEST(test_rna_base_letter_U);
    RUN_TEST(test_rna_base_letter_G);
    RUN_TEST(test_rna_base_letter_invalid);

    /* Spot-checks */
    RUN_TEST(test_UCU_is_serine);
    RUN_TEST(test_CAU_is_histidine);
    RUN_TEST(test_GAU_is_aspartate);
    RUN_TEST(test_UGG_is_tryptophan);

    /* King Wen spot-checks */
    RUN_TEST(test_kw_UAU_is_62);
    RUN_TEST(test_kw_AAA_is_1);
    RUN_TEST(test_kw_GAU_is_32);

    /* Bijection checks */
    RUN_TEST(test_binary_mapping_is_bijection);

    return UNITY_END();
}
