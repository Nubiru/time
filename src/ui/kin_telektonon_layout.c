#include "kin_telektonon_layout.h"
#include "../systems/tzolkin/telektonon_board.h"
#include "../systems/tzolkin/thirteen_moon.h"
#include <string.h>

/* Earth Walk positions: days 1-6, bottom row. */
#define EW_Y        0.85f
#define EW_X_START  0.05f
#define EW_X_STEP   0.09f

/* Heaven Walk positions: days 23-28, top row. */
#define HW_Y        0.15f
#define HW_X_START  0.50f
#define HW_X_STEP   0.09f

/* Warrior's Cube: days 7-22, 4x4 grid in center. */
#define CUBE_X_START  0.20f
#define CUBE_X_STEP   0.20f
#define CUBE_Y_START  0.30f
#define CUBE_Y_STEP   0.15f

kin_telektonon_layout_t kin_telektonon_compute(double jd)
{
    kin_telektonon_layout_t layout;
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

    /* Build all 28 day positions */
    for (int i = 1; i <= KIN_TK_DAYS; i++) {
        int idx = i - 1;
        tk_day_info_t info = tk_day_info(i);

        layout.days[idx].day           = info.day;
        layout.days[idx].phase         = (int)info.phase;
        layout.days[idx].cube_position = info.cube_position;
        layout.days[idx].is_tower      = info.is_tower_day;
        layout.days[idx].is_reunion    = info.is_reunion_day;
        layout.days[idx].highlighted   = (i == tm.day) ? 1 : 0;

        /* Position based on phase */
        if (i >= 1 && i <= 6) {
            /* Earth Walk: bottom row */
            layout.days[idx].x = EW_X_START + (float)(i - 1) * EW_X_STEP;
            layout.days[idx].y = EW_Y;
        } else if (i >= 7 && i <= 22) {
            /* Warrior's Cube: 4x4 grid */
            int cube_pos = i - 6;  /* 1-16 */
            int row = (cube_pos - 1) / 4;
            int col = (cube_pos - 1) % 4;
            layout.days[idx].x = CUBE_X_START + (float)col * CUBE_X_STEP;
            layout.days[idx].y = CUBE_Y_START + (float)row * CUBE_Y_STEP;
        } else {
            /* Heaven Walk: top row */
            layout.days[idx].x = HW_X_START + (float)(i - 23) * HW_X_STEP;
            layout.days[idx].y = HW_Y;
        }
    }

    /* Today's phase info */
    layout.today_phase = (int)tk_phase_for_day(tm.day);
    layout.phase_name  = tk_phase_name(tk_phase_for_day(tm.day));
    layout.cube_day    = tk_cube_position(tm.day);

    return layout;
}
