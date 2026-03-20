/* system_correlation.c — Cross-system co-firing analysis (Track 26, S100)
 *
 * Scans N years of convergence data and builds a 21x21 correlation matrix.
 * Uses cd_check_pair(sys, sys, jd) to probe individual system significance
 * (since cd_is_significant is static in convergence_detect.c).
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "system_correlation.h"
#include "../../math/julian.h"
#include <string.h>

int sc_corr_compute(int start_year, int years, sc_corr_result_t *out)
{
    if (!out || years <= 0) return 0;
    memset(out, 0, sizeof(*out));

    /* Raw co-fire counts (integers) — normalize after scan */
    int counts[CD_SYS_COUNT][CD_SYS_COUNT];
    memset(counts, 0, sizeof(counts));

    int total_days = 0;

    for (int y = 0; y < years; y++) {
        double jan1 = gregorian_to_jd(start_year + y, 1, 1.0);
        for (int d = 0; d < 365; d++) {
            double jd = jan1 + (double)d;
            total_days++;

            /* Probe each system for significance via cd_check_pair(s, s, jd).
             * cd_check_pair(a, b) returns cd_is_significant(a) && cd_is_significant(b),
             * so cd_check_pair(s, s) == cd_is_significant(s). */
            int sig[CD_SYS_COUNT];
            int sig_count = 0;
            for (int i = 0; i < CD_SYS_COUNT; i++) {
                sig[i] = cd_check_pair((cd_system_t)i, (cd_system_t)i, jd);
                if (sig[i]) {
                    out->solo_count[i]++;
                    sig_count++;
                }
            }

            /* Accumulate co-firing counts (upper triangle, mirror later) */
            if (sig_count >= 2) {
                for (int i = 0; i < CD_SYS_COUNT; i++) {
                    if (!sig[i]) continue;
                    for (int j = i + 1; j < CD_SYS_COUNT; j++) {
                        if (sig[j]) {
                            counts[i][j]++;
                        }
                    }
                }
            }
        }
    }

    /* Normalize to frequency (0.0-1.0) and mirror to lower triangle */
    if (total_days > 0) {
        double inv = 1.0 / (double)total_days;
        for (int i = 0; i < CD_SYS_COUNT; i++) {
            for (int j = i + 1; j < CD_SYS_COUNT; j++) {
                double freq = (double)counts[i][j] * inv;
                out->matrix[i][j] = freq;
                out->matrix[j][i] = freq;
            }
            /* Diagonal stays 0.0 — a system does not "co-fire" with itself */
        }
    }

    out->days_scanned = total_days;
    out->years_scanned = years;

    return total_days;
}

int sc_corr_top_pairs(const sc_corr_result_t *result,
                      sc_corr_pair_t *out, int max_pairs)
{
    if (!result || !out || max_pairs <= 0) return 0;

    int found = 0;

    /* Insertion sort: scan upper triangle, maintain sorted top-N */
    for (int i = 0; i < CD_SYS_COUNT; i++) {
        for (int j = i + 1; j < CD_SYS_COUNT; j++) {
            double freq = result->matrix[i][j];
            if (freq <= 0.0) continue;

            /* Find insertion position (descending order) */
            int pos = found;
            for (int k = 0; k < found && k < max_pairs; k++) {
                if (freq > out[k].frequency) {
                    pos = k;
                    break;
                }
            }

            if (pos < max_pairs) {
                /* Shift elements down to make room */
                int shift_end = found < max_pairs ? found : max_pairs - 1;
                for (int k = shift_end; k > pos; k--) {
                    out[k] = out[k - 1];
                }
                out[pos].system_a = i;
                out[pos].system_b = j;
                out[pos].frequency = freq;
                out[pos].co_fire_days = (int)(freq * (double)result->days_scanned + 0.5);
                if (found < max_pairs) found++;
            }
        }
    }
    return found;
}

double sc_corr_frequency(const sc_corr_result_t *result,
                         cd_system_t a, cd_system_t b)
{
    if (!result) return 0.0;
    if ((int)a < 0 || (int)a >= CD_SYS_COUNT) return 0.0;
    if ((int)b < 0 || (int)b >= CD_SYS_COUNT) return 0.0;
    return result->matrix[(int)a][(int)b];
}

int sc_corr_solo(const sc_corr_result_t *result, cd_system_t sys)
{
    if (!result) return 0;
    if ((int)sys < 0 || (int)sys >= CD_SYS_COUNT) return 0;
    return result->solo_count[(int)sys];
}
