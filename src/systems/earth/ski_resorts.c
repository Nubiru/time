/* ski_resorts.c — 50 world ski resorts with coordinates, elevation,
 * seasonal windows, and snow quality scoring. Pure data module. */

#include "ski_resorts.h"
#include <math.h>
#include <stddef.h>

#define PI 3.14159265358979323846

static const ski_resort_t RESORTS[SKI_RESORT_COUNT] = {
    /* North America */
    { 0, "Whistler Blackcomb", "Canada", 50.116, -122.957, 675, 2284, 11, 5, "Largest ski area in North America, BC"},
    { 1, "Jackson Hole", "USA", 43.588, -110.828, 1924, 3185, 12, 4, "Steep terrain, Teton Village, Wyoming"},
    { 2, "Vail", "USA", 39.640, -106.374, 2457, 3527, 11, 4, "Colorado, largest single-mountain ski area in USA"},
    { 3, "Aspen Snowmass", "USA", 39.209, -106.950, 2399, 3813, 11, 4, "Four mountains, iconic Colorado resort"},
    { 4, "Park City", "USA", 40.651, -111.508, 2080, 3049, 11, 4, "Utah, largest ski resort in USA"},
    { 5, "Mammoth Mountain", "USA", 37.631, -119.033, 2424, 3369, 11, 6, "California, Sierra Nevada, long season"},
    { 6, "Squaw Valley (Palisades)", "USA", 39.197, -120.235, 1890, 2760, 11, 5, "Lake Tahoe, 1960 Olympics"},
    { 7, "Big Sky", "USA", 45.284, -111.401, 2072, 3403, 11, 4, "Montana, Lone Mountain, uncrowded"},
    { 8, "Telluride", "USA", 37.937, -107.846, 2659, 4010, 11, 4, "Colorado, historic mining town"},
    { 9, "Banff Sunshine", "Canada", 51.115, -115.762, 1660, 2730, 11, 5, "Alberta, Canadian Rockies"},
    {10, "Revelstoke", "Canada", 51.027, -118.160, 512, 2225, 12, 4, "BC, biggest vertical in North America"},
    {11, "Taos Ski Valley", "USA", 36.596, -105.454, 2804, 3804, 11, 4, "New Mexico, steep and deep"},
    {12, "Steamboat Springs", "USA", 40.457, -106.804, 2103, 3221, 11, 4, "Colorado Champagne Powder"},
    /* Europe */
    {13, "Chamonix Mont-Blanc", "France", 45.924, 6.870, 1035, 3842, 12, 4, "Birthplace of alpinism, Vallee Blanche"},
    {14, "Val d'Isere", "France", 45.448, 6.980, 1550, 3456, 11, 5, "Espace Killy, French Alps"},
    {15, "Courchevel", "France", 45.415, 6.635, 1300, 2738, 12, 4, "Three Valleys, luxury French resort"},
    {16, "Verbier", "Switzerland", 46.096, 7.228, 1500, 3330, 11, 4, "4 Vallees, Swiss freeride capital"},
    {17, "Zermatt", "Switzerland", 46.020, 7.749, 1620, 3883, 11, 4, "Matterhorn views, highest ski in Alps"},
    {18, "St. Anton", "Austria", 47.130, 10.268, 1304, 2811, 12, 4, "Arlberg, birthplace of alpine skiing"},
    {19, "Kitzbuhel", "Austria", 47.444, 12.392, 800, 2000, 12, 4, "Hahnenkamm downhill, medieval town"},
    {20, "Lech-Zurs", "Austria", 47.208, 10.142, 1450, 2811, 12, 4, "Arlberg luxury, Ski Arlberg connection"},
    {21, "Cortina d'Ampezzo", "Italy", 46.537, 12.135, 1224, 3243, 12, 4, "Dolomites, 2026 Olympics"},
    {22, "Val Gardena", "Italy", 46.558, 11.760, 1236, 2518, 12, 4, "Dolomiti Superski, Sellaronda circuit"},
    {23, "Andermatt", "Switzerland", 46.636, 8.594, 1444, 2963, 11, 5, "Central Swiss Alps, deep snow"},
    {24, "Are", "Sweden", 63.399, 13.074, 380, 1274, 11, 5, "Scandinavia's largest ski area"},
    /* Japan */
    {25, "Niseko", "Japan", 42.863, 140.699, 260, 1308, 12, 5, "Hokkaido, legendary powder snow"},
    {26, "Hakuba", "Japan", 36.698, 137.832, 760, 1831, 12, 4, "Nagano, 1998 Olympics"},
    {27, "Furano", "Japan", 43.338, 142.383, 245, 1074, 12, 3, "Hokkaido, dry champagne powder"},
    {28, "Myoko Kogen", "Japan", 36.867, 138.862, 730, 1510, 12, 4, "Niigata, heavy snowfall record-holder"},
    /* South America (Southern Hemisphere: season Jun-Oct) */
    {29, "Portillo", "Chile", -32.838, -70.126, 2590, 3348, 6, 10, "Legendary Chilean resort, Andes"},
    {30, "Valle Nevado", "Chile", -33.376, -70.254, 2860, 3670, 6, 10, "Largest ski area in South America"},
    {31, "Las Lenas", "Argentina", -35.136, -70.068, 2240, 3430, 6, 10, "Off-piste mecca, Mendoza"},
    {32, "Cerro Catedral", "Argentina", -41.168, -71.459, 1030, 2100, 6, 10, "Bariloche, Patagonia views"},
    {33, "Cerro Castor", "Argentina", -54.755, -68.332, 195, 1057, 6, 10, "Southernmost ski resort in the world, Ushuaia"},
    /* Oceania (Southern Hemisphere) */
    {34, "Perisher", "Australia", -36.402, 148.409, 1605, 2034, 6, 10, "Largest ski area in Australia, NSW"},
    {35, "Thredbo", "Australia", -36.505, 148.304, 1365, 2037, 6, 10, "Longest runs in Australia, NSW"},
    {36, "Mount Hutt", "New Zealand", -43.483, 171.547, 1363, 2086, 6, 10, "Canterbury, closest to city"},
    {37, "Treble Cone", "New Zealand", -44.633, 168.897, 1260, 2088, 6, 10, "Wanaka, South Island steeps"},
    {38, "Remarkables", "New Zealand", -45.049, 168.815, 1603, 1943, 6, 10, "Queenstown, accessible terrain"},
    /* Asia & Middle East */
    {39, "Gulmarg", "India", 34.049, 74.380, 2690, 3980, 12, 3, "Kashmir, Himalayan powder"},
    {40, "Yongpyong", "South Korea", 37.644, 128.680, 700, 1458, 11, 3, "Gangwon, 2018 Olympics venue"},
    {41, "Shiga Kogen", "Japan", 36.774, 138.529, 1340, 2307, 12, 5, "Nagano, largest ski area in Japan"},
    /* Scandinavia & Northern Europe */
    {42, "Levi", "Finland", 67.800, 24.817, 205, 531, 11, 5, "Lapland, midnight sun skiing in spring"},
    {43, "Hemsedal", "Norway", 60.864, 8.554, 620, 1450, 11, 5, "Scandinavian Alps"},
    {44, "Trysil", "Norway", 61.318, 12.264, 415, 1132, 11, 4, "Norway's largest ski resort"},
    /* More Americas */
    {45, "Sun Valley", "USA", 43.697, -114.351, 1752, 2789, 11, 4, "Idaho, first destination ski resort"},
    {46, "Alta", "USA", 40.588, -111.638, 2600, 3216, 11, 4, "Utah, skiers only, legendary snow"},
    {47, "Mont Tremblant", "Canada", 46.210, -74.586, 265, 875, 11, 4, "Quebec, eastern Canada's best"},
    /* Africa */
    {48, "Tiffindell", "South Africa", -30.650, 27.933, 2720, 3001, 6, 9, "Only ski resort in South Africa"},
    /* More Europe */
    {49, "Sierra Nevada", "Spain", 37.096, -3.398, 2100, 3300, 12, 4, "Southernmost major ski resort in Europe"},
};

ski_resort_t ski_resort_get(int index) {
    if (index < 0 || index >= SKI_RESORT_COUNT) {
        ski_resort_t invalid = {0};
        invalid.id = -1;
        return invalid;
    }
    return RESORTS[index];
}

int ski_resort_count(void) {
    return SKI_RESORT_COUNT;
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

int ski_resort_nearest(double lat, double lon) {
    int best = 0;
    double best_dist = haversine_dist_sq(lat, lon, RESORTS[0].lat, RESORTS[0].lon);
    for (int i = 1; i < SKI_RESORT_COUNT; i++) {
        double d = haversine_dist_sq(lat, lon, RESORTS[i].lat, RESORTS[i].lon);
        if (d < best_dist) {
            best_dist = d;
            best = i;
        }
    }
    return best;
}

bool ski_resort_in_season(int index, int month) {
    if (index < 0 || index >= SKI_RESORT_COUNT) return false;
    if (month < 1 || month > 12) return false;
    int start = RESORTS[index].season_start;
    int end = RESORTS[index].season_end;
    if (start <= end) {
        return month >= start && month <= end;
    }
    /* Wrap-around (e.g., Nov-May = 11..5) */
    return month >= start || month <= end;
}

double ski_resort_snow_quality(int resort_index, int month) {
    if (resort_index < 0 || resort_index >= SKI_RESORT_COUNT) return 0.0;
    if (month < 1 || month > 12) return 0.0;
    if (!ski_resort_in_season(resort_index, month)) return 0.0;

    int start = RESORTS[resort_index].season_start;
    int end = RESORTS[resort_index].season_end;

    /* Calculate season length and position within season */
    int season_len;
    if (start <= end) {
        season_len = end - start + 1;
    } else {
        season_len = (12 - start + 1) + end;
    }

    /* Month position within season (0-based) */
    int pos;
    if (start <= end) {
        pos = month - start;
    } else {
        if (month >= start) {
            pos = month - start;
        } else {
            pos = (12 - start) + month;
        }
    }

    /* Peak is at middle of season. Score falls off towards edges. */
    double mid = (season_len - 1) / 2.0;
    double dist = fabs(pos - mid);
    double quality = 1.0 - (dist / (mid + 1.0));
    if (quality < 0.0) quality = 0.0;
    if (quality > 1.0) quality = 1.0;
    return quality;
}

const char *ski_resort_name(int index) {
    if (index < 0 || index >= SKI_RESORT_COUNT) return NULL;
    return RESORTS[index].name;
}

int ski_resort_vertical(int index) {
    if (index < 0 || index >= SKI_RESORT_COUNT) return 0;
    return RESORTS[index].summit_elev_m - RESORTS[index].base_elev_m;
}

int ski_resorts_in_region(double lat_min, double lat_max,
                          double lon_min, double lon_max,
                          int *out_indices, int out_max) {
    int count = 0;
    for (int i = 0; i < SKI_RESORT_COUNT && count < out_max; i++) {
        if (RESORTS[i].lat >= lat_min && RESORTS[i].lat <= lat_max &&
            RESORTS[i].lon >= lon_min && RESORTS[i].lon <= lon_max) {
            out_indices[count++] = i;
        }
    }
    return count;
}
