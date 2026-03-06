/* surf_spots.c — 50 world-class surf spots with coordinates, wave type,
 * tidal/swell quality scoring, and geographic search. Pure data module. */

#include "surf_spots.h"
#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846

static const surf_spot_t SPOTS[SURF_SPOT_COUNT] = {
    { 0, "Pipeline", "USA", 21.665, -158.053, 315, 315, TIDE_LOW, SEASON_WINTER, WAVE_REEF, "North Shore Oahu, powerful hollow lefts and rights"},
    { 1, "Sunset Beach", "USA", 21.678, -158.104, 340, 330, TIDE_ALL, SEASON_WINTER, WAVE_REEF, "Big wave spot, North Shore Oahu"},
    { 2, "Waimea Bay", "USA", 21.643, -158.066, 350, 330, TIDE_ALL, SEASON_WINTER, WAVE_BEACH, "Historic big wave bay, Oahu"},
    { 3, "Jaws (Peahi)", "USA", 20.942, -156.221, 350, 330, TIDE_ALL, SEASON_WINTER, WAVE_REEF, "Maui tow-in big wave spot"},
    { 4, "Mavericks", "USA", 37.494, -122.497, 270, 300, TIDE_MID, SEASON_WINTER, WAVE_REEF, "Cold water big wave, Half Moon Bay"},
    { 5, "Trestles", "USA", 33.381, -117.589, 225, 200, TIDE_ALL, SEASON_SUMMER, WAVE_POINT, "Southern California high-performance wave"},
    { 6, "Rincon", "USA", 34.374, -119.479, 180, 270, TIDE_MID, SEASON_WINTER, WAVE_POINT, "Queen of the Coast, Santa Barbara"},
    { 7, "Steamer Lane", "USA", 36.951, -122.026, 225, 290, TIDE_MID, SEASON_WINTER, WAVE_POINT, "Santa Cruz classic point break"},
    { 8, "Teahupo'o", "French Polynesia", -17.857, -149.256, 225, 210, TIDE_MID, SEASON_WINTER, WAVE_REEF, "Heaviest wave on Earth, Tahiti"},
    { 9, "Cloudbreak", "Fiji", -17.869, 177.184, 225, 210, TIDE_MID, SEASON_WINTER, WAVE_REEF, "World-class left off Tavarua Island"},
    {10, "Restaurants (Tavarua)", "Fiji", -17.858, 177.178, 315, 210, TIDE_ALL, SEASON_WINTER, WAVE_REEF, "Perfect right-hand wall, Fiji"},
    {11, "Jeffreys Bay", "South Africa", -33.969, 25.532, 135, 225, TIDE_ALL, SEASON_WINTER, WAVE_POINT, "World's best right-hand point break"},
    {12, "Dungeons", "South Africa", -34.071, 18.319, 270, 225, TIDE_ALL, SEASON_WINTER, WAVE_REEF, "Cape Town big wave reef"},
    {13, "Skeleton Bay", "Namibia", -22.720, 14.525, 270, 225, TIDE_LOW, SEASON_WINTER, WAVE_BEACH, "Longest sand-bottom tube in the world"},
    {14, "Nazare", "Portugal", 39.602, -9.070, 270, 315, TIDE_ALL, SEASON_WINTER, WAVE_BEACH, "World record big wave, Praia do Norte"},
    {15, "Supertubos", "Portugal", 39.348, -9.376, 270, 315, TIDE_LOW, SEASON_AUTUMN, WAVE_BEACH, "European Pipeline, Peniche"},
    {16, "Hossegor", "France", 43.670, -1.447, 270, 290, TIDE_LOW, SEASON_AUTUMN, WAVE_BEACH, "Best beach break in Europe"},
    {17, "Mundaka", "Spain", 43.405, -2.699, 0, 315, TIDE_MID, SEASON_AUTUMN, WAVE_RIVER_MOUTH, "Legendary Basque Country left barrel"},
    {18, "Thurso East", "UK", 58.597, -3.522, 0, 340, TIDE_MID, SEASON_AUTUMN, WAVE_REEF, "Scotland's best reef break"},
    {19, "Bundoran", "Ireland", 54.476, -8.282, 315, 290, TIDE_MID, SEASON_WINTER, WAVE_REEF, "The Peak, Irish big wave spot"},
    {20, "Uluwatu", "Indonesia", -8.815, 115.087, 225, 210, TIDE_MID, SEASON_WINTER, WAVE_REEF, "Bali's premier left-hand reef"},
    {21, "Padang Padang", "Indonesia", -8.813, 115.091, 225, 210, TIDE_MID, SEASON_WINTER, WAVE_REEF, "Balinese Pipeline, heavy barrel"},
    {22, "G-Land", "Indonesia", -8.740, 114.358, 225, 210, TIDE_MID, SEASON_WINTER, WAVE_REEF, "Grajagan, remote jungle left"},
    {23, "Desert Point", "Indonesia", -8.752, 115.826, 225, 210, TIDE_LOW, SEASON_WINTER, WAVE_REEF, "Lombok mechanical barrel"},
    {24, "Mentawai Islands", "Indonesia", -2.083, 99.567, 225, 210, TIDE_MID, SEASON_SPRING, WAVE_REEF, "Sumatra island chain, multiple world-class breaks"},
    {25, "Bells Beach", "Australia", -38.369, 144.281, 180, 225, TIDE_ALL, SEASON_AUTUMN, WAVE_REEF, "Victoria, home of the Rip Curl Pro"},
    {26, "Snapper Rocks", "Australia", -28.161, 153.556, 90, 135, TIDE_ALL, SEASON_AUTUMN, WAVE_POINT, "Superbank sand-bottom point, Gold Coast"},
    {27, "Margaret River", "Australia", -33.960, 115.008, 270, 225, TIDE_ALL, SEASON_WINTER, WAVE_REEF, "Western Australia power wave"},
    {28, "Shipstern Bluff", "Australia", -43.135, 147.837, 180, 180, TIDE_MID, SEASON_WINTER, WAVE_REEF, "Tasmania heavy slab"},
    {29, "Puerto Escondido", "Mexico", 15.862, -97.075, 180, 180, TIDE_MID, SEASON_SUMMER, WAVE_BEACH, "Mexican Pipeline, Oaxaca"},
    {30, "Todos Santos", "Mexico", 31.800, -116.800, 270, 300, TIDE_ALL, SEASON_WINTER, WAVE_REEF, "Baja California big wave"},
    {31, "Chicama", "Peru", -7.842, -79.450, 225, 210, TIDE_ALL, SEASON_AUTUMN, WAVE_POINT, "Longest left-hand wave in the world"},
    {32, "Punta de Lobos", "Chile", -34.418, -72.042, 270, 225, TIDE_ALL, SEASON_WINTER, WAVE_POINT, "Chilean big wave point break"},
    {33, "Fernando de Noronha", "Brazil", -3.854, -32.424, 315, 315, TIDE_ALL, SEASON_WINTER, WAVE_REEF, "Brazilian island paradise wave"},
    {34, "Raglan", "New Zealand", -37.800, 174.870, 270, 225, TIDE_ALL, SEASON_AUTUMN, WAVE_POINT, "Manu Bay, endless left point"},
    {35, "Nias", "Indonesia", 1.108, 97.547, 225, 210, TIDE_MID, SEASON_SPRING, WAVE_REEF, "Lagundri Bay, north Sumatra perfection"},
    {36, "Keramas", "Indonesia", -8.567, 115.372, 135, 90, TIDE_MID, SEASON_WINTER, WAVE_REEF, "Bali night surfing right-hander"},
    {37, "Lances Right", "Indonesia", -2.520, 99.183, 135, 210, TIDE_MID, SEASON_SPRING, WAVE_REEF, "HTs, Mentawai telescoping right"},
    {38, "Macaronis", "Indonesia", -2.227, 99.271, 225, 210, TIDE_MID, SEASON_SPRING, WAVE_REEF, "Mentawai playground, forgiving barrel"},
    {39, "Banzai Pipeline (Backdoor)", "USA", 21.665, -158.053, 315, 315, TIDE_LOW, SEASON_WINTER, WAVE_REEF, "Right-hand barrel at Pipeline reef"},
    {40, "Honolua Bay", "USA", 21.014, -156.638, 315, 315, TIDE_MID, SEASON_WINTER, WAVE_POINT, "Maui right-hand point in winter"},
    {41, "Taghazout", "Morocco", 30.549, -9.714, 315, 315, TIDE_MID, SEASON_WINTER, WAVE_POINT, "Anchor Point, Moroccan right"},
    {42, "Ericeira", "Portugal", 38.967, -9.421, 270, 315, TIDE_MID, SEASON_AUTUMN, WAVE_REEF, "World Surfing Reserve, multiple breaks"},
    {43, "Itacare", "Brazil", -14.282, -38.986, 90, 135, TIDE_ALL, SEASON_WINTER, WAVE_BEACH, "Bahia tropical beach breaks"},
    {44, "Coxos", "Portugal", 39.034, -9.434, 270, 315, TIDE_LOW, SEASON_WINTER, WAVE_REEF, "Ericeira's heavy right tube"},
    {45, "Soup Bowl", "Barbados", 13.172, -59.530, 90, 45, TIDE_MID, SEASON_WINTER, WAVE_REEF, "Caribbean's best wave, Bathsheba"},
    {46, "Siargao (Cloud 9)", "Philippines", 9.816, 126.169, 90, 90, TIDE_MID, SEASON_AUTUMN, WAVE_REEF, "Southeast Asian hollow right"},
    {47, "Sri Lanka (Arugam Bay)", "Sri Lanka", 6.842, 81.835, 135, 180, TIDE_ALL, SEASON_SUMMER, WAVE_POINT, "Bay of Bengal point break"},
    {48, "Maldives (Sultans)", "Maldives", 3.950, 73.475, 180, 180, TIDE_MID, SEASON_SPRING, WAVE_REEF, "North Male Atoll right-hander"},
    {49, "Canggu", "Indonesia", -8.649, 115.123, 270, 225, TIDE_MID, SEASON_WINTER, WAVE_BEACH, "Bali beach break, Echo Beach"},
};

surf_spot_t surf_spot_get(int index) {
    if (index < 0 || index >= SURF_SPOT_COUNT) {
        surf_spot_t invalid = {0};
        invalid.id = -1;
        return invalid;
    }
    return SPOTS[index];
}

int surf_spot_count(void) {
    return SURF_SPOT_COUNT;
}

static double deg_to_rad(double deg) {
    return deg * PI / 180.0;
}

static double haversine_dist_sq(double lat1, double lon1, double lat2, double lon2) {
    double dlat = deg_to_rad(lat2 - lat1);
    double dlon = deg_to_rad(lon2 - lon1);
    double a = sin(dlat / 2.0) * sin(dlat / 2.0) +
               cos(deg_to_rad(lat1)) * cos(deg_to_rad(lat2)) *
               sin(dlon / 2.0) * sin(dlon / 2.0);
    return a;
}

int surf_spot_nearest(double lat, double lon) {
    int best = 0;
    double best_dist = haversine_dist_sq(lat, lon, SPOTS[0].lat, SPOTS[0].lon);
    for (int i = 1; i < SURF_SPOT_COUNT; i++) {
        double d = haversine_dist_sq(lat, lon, SPOTS[i].lat, SPOTS[i].lon);
        if (d < best_dist) {
            best_dist = d;
            best = i;
        }
    }
    return best;
}

static double clamp01(double v) {
    if (v < 0.0) return 0.0;
    if (v > 1.0) return 1.0;
    return v;
}

double surf_spot_tide_quality(int spot_index, double tidal_height) {
    if (spot_index < 0 || spot_index >= SURF_SPOT_COUNT) return 0.0;
    tide_pref_t pref = SPOTS[spot_index].best_tide;
    switch (pref) {
        case TIDE_ALL: return 1.0;
        case TIDE_LOW: return clamp01(1.0 - (tidal_height + 1.0));
        case TIDE_MID: return clamp01(1.0 - fabs(tidal_height));
        case TIDE_HIGH: return clamp01((tidal_height + 1.0) - 1.0);
    }
    return 0.0;
}

double surf_spot_swell_quality(int spot_index, double swell_dir_deg) {
    if (spot_index < 0 || spot_index >= SURF_SPOT_COUNT) return 0.0;
    double best = SPOTS[spot_index].best_swell_dir_deg;
    double diff = fabs(swell_dir_deg - best);
    if (diff > 180.0) diff = 360.0 - diff;
    return clamp01(1.0 - diff / 90.0);
}

const char *surf_spot_name(int index) {
    if (index < 0 || index >= SURF_SPOT_COUNT) return NULL;
    return SPOTS[index].name;
}

int surf_spots_by_type(wave_type_t type, int *out_indices, int out_max) {
    int count = 0;
    for (int i = 0; i < SURF_SPOT_COUNT && count < out_max; i++) {
        if (SPOTS[i].wave_type == type) {
            out_indices[count++] = i;
        }
    }
    return count;
}

int surf_spots_in_region(double lat_min, double lat_max,
                         double lon_min, double lon_max,
                         int *out_indices, int out_max) {
    int count = 0;
    for (int i = 0; i < SURF_SPOT_COUNT && count < out_max; i++) {
        if (SPOTS[i].lat >= lat_min && SPOTS[i].lat <= lat_max &&
            SPOTS[i].lon >= lon_min && SPOTS[i].lon <= lon_max) {
            out_indices[count++] = i;
        }
    }
    return count;
}

const char *surf_tide_name(tide_pref_t tide) {
    switch (tide) {
        case TIDE_LOW:  return "Low";
        case TIDE_MID:  return "Mid";
        case TIDE_HIGH: return "High";
        case TIDE_ALL:  return "All";
    }
    return "Unknown";
}

const char *surf_season_name(season_pref_t season) {
    switch (season) {
        case SEASON_WINTER: return "Winter";
        case SEASON_SPRING: return "Spring";
        case SEASON_SUMMER: return "Summer";
        case SEASON_AUTUMN: return "Autumn";
        case SEASON_ALL:    return "All";
    }
    return "Unknown";
}

const char *surf_wave_type_name(wave_type_t type) {
    switch (type) {
        case WAVE_REEF:       return "Reef";
        case WAVE_BEACH:      return "Beach";
        case WAVE_POINT:      return "Point";
        case WAVE_RIVER_MOUTH: return "River Mouth";
    }
    return "Unknown";
}
