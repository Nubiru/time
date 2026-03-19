/* audio_event.c -- Audio event detection implementation.
 *
 * Combines planetary aspect detection and calendar convergence
 * scanning to produce audio-relevant event parameters.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "audio_event.h"
#include "../systems/astronomy/planets.h"
#include "../systems/astrology/aspects.h"
#include "../systems/unified/audio_data.h"
#include "../systems/unified/convergence_detect.h"

float audio_event_consonance(double jd, double max_orb)
{
    if (max_orb <= 0.0) {
        return 0.5f;
    }

    /* Get planet positions */
    solar_system_t ss = planets_at(jd);
    double lons[8];
    for (int i = 0; i < 8; i++) {
        lons[i] = ss.positions[i].longitude;
    }

    /* Find all aspects */
    aspect_list_t al = aspects_find_all(lons, max_orb);

    if (al.count == 0) {
        return 0.5f; /* no aspects = neutral */
    }

    /* Weighted average consonance: tighter orbs count more */
    double total_weight = 0.0;
    double weighted_sum = 0.0;
    for (int i = 0; i < al.count; i++) {
        /* Map aspect_type_t to audio_aspect_t (same 0-4 enum order) */
        audio_aspect_t aa = (audio_aspect_t)al.aspects[i].type;
        double consonance = audio_aspect_consonance(aa);
        double weight = 1.0 - al.aspects[i].orb / max_orb;
        if (weight < 0.0) {
            weight = 0.0;
        }
        weighted_sum += consonance * weight;
        total_weight += weight;
    }

    if (total_weight <= 0.0) {
        return 0.5f;
    }

    float result = (float)(weighted_sum / total_weight);
    if (result < 0.0f) {
        result = 0.0f;
    }
    if (result > 1.0f) {
        result = 1.0f;
    }
    return result;
}

float audio_event_density(double jd)
{
    double sig = cd_significance(jd);
    if (sig < 0.0) {
        sig = 0.0;
    }
    if (sig > 1.0) {
        sig = 1.0;
    }
    return (float)sig;
}

float audio_event_intensity(double jd, double max_orb)
{
    float c = audio_event_consonance(jd, max_orb);
    float d = audio_event_density(jd);
    float intensity = 0.6f * c + 0.4f * d;
    if (intensity < 0.0f) {
        intensity = 0.0f;
    }
    if (intensity > 1.0f) {
        intensity = 1.0f;
    }
    return intensity;
}

audio_event_summary_t audio_event_scan(double jd, double max_orb)
{
    audio_event_summary_t result;

    /* Aspects */
    double effective_orb = (max_orb > 0.0) ? max_orb : 8.0;
    solar_system_t ss = planets_at(jd);
    double lons[8];
    for (int i = 0; i < 8; i++) {
        lons[i] = ss.positions[i].longitude;
    }
    aspect_list_t al = aspects_find_all(lons, effective_orb);

    result.aspect_count = al.count;
    result.consonance = audio_event_consonance(jd, max_orb);

    /* Find tightest orb */
    result.tightest_orb = 999.0f;
    for (int i = 0; i < al.count; i++) {
        if ((float)al.aspects[i].orb < result.tightest_orb) {
            result.tightest_orb = (float)al.aspects[i].orb;
        }
    }
    if (al.count == 0) {
        result.tightest_orb = 0.0f;
    }

    /* Convergences */
    result.convergence_density = audio_event_density(jd);
    cd_result_t cr = cd_scan(jd);
    result.convergence_count = cr.count;

    /* Combined intensity */
    result.event_intensity =
        0.6f * result.consonance + 0.4f * result.convergence_density;
    if (result.event_intensity > 1.0f) {
        result.event_intensity = 1.0f;
    }

    return result;
}
