#include "kin_telektonon_path.h"
#include "../systems/tzolkin/telektonon_journey.h"
#include "../systems/tzolkin/telektonon_board.h"
#include "../systems/tzolkin/thirteen_moon.h"
#include <string.h>

/* Earth Walk positions: days 1-6, bottom row. */
#define PATH_EW_Y        0.90f
#define PATH_EW_X_START  0.05f
#define PATH_EW_X_STEP   0.18f

/* Heaven Walk positions: days 23-28, top row. */
#define PATH_HW_Y        0.10f
#define PATH_HW_X_START  0.05f
#define PATH_HW_X_STEP   0.18f

/* Warrior's Cube: days 7-22, 4x4 zigzag grid. */
#define PATH_CUBE_X_START  0.10f
#define PATH_CUBE_X_STEP   0.27f
#define PATH_CUBE_Y_START  0.70f
#define PATH_CUBE_Y_STEP   0.15f

kin_tk_path_layout_t kin_tk_path_compute(double jd)
{
    kin_tk_path_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    if (jd < 0.0)
        return layout;

    thirteen_moon_t tm = thirteen_moon_from_jd(jd);

    /* Day Out of Time has no moon day -> return zeroed */
    if (tm.is_day_out)
        return layout;

    /* Invalid result (moon=0 and not day_out) -> zeroed */
    if (tm.moon == 0)
        return layout;

    layout.today_day = tm.day;
    layout.today_phase = (int)tk_phase_for_day(tm.day);

    /* Build all 28 day positions */
    for (int i = 1; i <= KIN_TK_PATH_DAYS; i++) {
        int idx = i - 1;
        tk_journey_t journey = tk_journey_for_day(i);
        tk_day_info_t info = tk_day_info(i);

        layout.days[idx].day            = i;
        layout.days[idx].phase          = (int)info.phase;
        layout.days[idx].phase_name     = journey.phase_name;
        layout.days[idx].yellow_pos     = journey.yellow_tube_pos;
        layout.days[idx].white_pos      = journey.white_tube_pos;
        layout.days[idx].green_pos      = journey.green_cube_pos;
        layout.days[idx].is_tower       = info.is_tower_day;
        layout.days[idx].is_reunion     = journey.is_reunion;
        layout.days[idx].lovers_together = journey.lovers_walking_together;
        layout.days[idx].cube_position  = info.cube_position;
        layout.days[idx].highlighted    = (i == tm.day) ? 1 : 0;

        /* Position based on phase: S-curve path */
        if (i >= 1 && i <= 6) {
            /* Earth Walk: bottom row, left to right */
            layout.days[idx].x = PATH_EW_X_START + (float)(i - 1) * PATH_EW_X_STEP;
            layout.days[idx].y = PATH_EW_Y;
        } else if (i >= 7 && i <= 22) {
            /* Warrior's Cube: boustrophedon zigzag, 4 rows of 4 */
            int row = (i - 7) / 4;
            int col = (i - 7) % 4;

            /* Odd rows reverse direction */
            if (row % 2 == 1)
                col = 3 - col;

            layout.days[idx].x = PATH_CUBE_X_START + (float)col * PATH_CUBE_X_STEP;
            layout.days[idx].y = PATH_CUBE_Y_START - (float)row * PATH_CUBE_Y_STEP;
        } else {
            /* Heaven Walk: top row, left to right */
            layout.days[idx].x = PATH_HW_X_START + (float)(i - 23) * PATH_HW_X_STEP;
            layout.days[idx].y = PATH_HW_Y;
        }
    }

    return layout;
}
