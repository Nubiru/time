#include "houses.h"
#include "zodiac.h"

house_system_t houses_whole_sign(double ascendant_lon) {
    house_system_t h;
    h.ascendant = ascendant_lon;

    int first_sign = zodiac_sign(ascendant_lon);
    for (int i = 0; i < 12; i++) {
        h.signs[i] = (first_sign + i) % 12;
        h.cusps[i] = h.signs[i] * 30.0;
    }
    return h;
}
