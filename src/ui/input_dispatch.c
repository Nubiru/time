/* input_dispatch.c — Keyboard input to action mapping.
 * Pure: no GL, no malloc, no globals, no I/O, no side effects. */

#include "input_dispatch.h"

/* ------------------------------------------------------------------
 * Internal: uppercase a letter key for case-insensitive dispatch
 * ------------------------------------------------------------------ */

static int to_upper(int key)
{
    if (key >= 'a' && key <= 'z') {
        return key - ('a' - 'A');
    }
    return key;
}

/* ------------------------------------------------------------------
 * id_dispatch — map key + shift to action
 * ------------------------------------------------------------------ */

id_result_t id_dispatch(int key, int shift)
{
    id_result_t r = { ID_ACTION_NONE, 0 };

    /* Shift + digit 0-6 => scale jump */
    if (shift && key >= '0' && key <= '6') {
        r.action = ID_ACTION_SCALE_JUMP;
        r.parameter = key - '0';
        return r;
    }

    /* Uppercase letters for case-insensitive matching */
    int k = to_upper(key);

    switch (k) {
    /* View switching */
    case 'E': r.action = ID_ACTION_VIEW_EARTH;       break;

    /* Focus modes */
    case 'A': r.action = ID_ACTION_FOCUS_ASTROLOGY;   break;
    case 'K': r.action = ID_ACTION_FOCUS_KIN;         break;
    case 'I': r.action = ID_ACTION_FOCUS_ICHING;      break;
    case 'C': r.action = ID_ACTION_FOCUS_CHINESE;     break;
    case 'D': r.action = ID_ACTION_FOCUS_HD;          break;

    /* Toggles */
    case 'T': r.action = ID_ACTION_TOGGLE_TRAILS;     break;
    case 'H': r.action = ID_ACTION_TOGGLE_HUD;        break;
    case 'S': r.action = ID_ACTION_TOGGLE_STARS;       break;
    case 'L': r.action = ID_ACTION_TOGGLE_LABELS;     break;
    case 'G': r.action = ID_ACTION_TOGGLE_GRID;       break;

    default:
        break;
    }

    if (r.action != ID_ACTION_NONE) {
        return r;
    }

    /* Non-letter keys (special + digits) */
    switch (key) {
    case 27:  r.action = ID_ACTION_FOCUS_OVERVIEW;    break;  /* Esc */
    case 32:  r.action = ID_ACTION_TOGGLE_PAUSE;      break;  /* Space */
    case '0': r.action = ID_ACTION_TIME_PAUSE;        break;
    case '1': r.action = ID_ACTION_TIME_REALTIME;     break;
    case '2': r.action = ID_ACTION_TIME_FAST;         break;
    case '3': r.action = ID_ACTION_TIME_FASTER;       break;
    case '4': r.action = ID_ACTION_TIME_DAY_SEC;      break;
    case '5': r.action = ID_ACTION_TIME_10DAY_SEC;    break;
    case '-': r.action = ID_ACTION_TIME_REVERSE;      break;
    default:  break;
    }

    return r;
}

/* ------------------------------------------------------------------
 * id_action_name — human-readable action name
 * ------------------------------------------------------------------ */

const char *id_action_name(id_action_t action)
{
    switch (action) {
    case ID_ACTION_NONE:            return "None";
    case ID_ACTION_VIEW_SPACE:      return "Space View";
    case ID_ACTION_VIEW_EARTH:      return "Earth View";
    case ID_ACTION_VIEW_GALAXY:     return "Galaxy View";
    case ID_ACTION_FOCUS_ASTROLOGY: return "Focus Astrology";
    case ID_ACTION_FOCUS_KIN:       return "Focus Kin";
    case ID_ACTION_FOCUS_ICHING:    return "Focus I Ching";
    case ID_ACTION_FOCUS_CHINESE:   return "Focus Chinese";
    case ID_ACTION_FOCUS_HD:        return "Focus Human Design";
    case ID_ACTION_FOCUS_OVERVIEW:  return "Overview";
    case ID_ACTION_TOGGLE_TRAILS:   return "Toggle Trails";
    case ID_ACTION_TOGGLE_HUD:      return "Toggle HUD";
    case ID_ACTION_TOGGLE_STARS:    return "Toggle Stars";
    case ID_ACTION_TOGGLE_LABELS:   return "Toggle Labels";
    case ID_ACTION_TOGGLE_GRID:     return "Toggle Grid";
    case ID_ACTION_TOGGLE_PAUSE:    return "Toggle Pause";
    case ID_ACTION_TIME_REALTIME:   return "Real-time";
    case ID_ACTION_TIME_FAST:       return "Fast (60x)";
    case ID_ACTION_TIME_FASTER:     return "Faster (3600x)";
    case ID_ACTION_TIME_DAY_SEC:    return "Day/sec";
    case ID_ACTION_TIME_10DAY_SEC:  return "10 Days/sec";
    case ID_ACTION_TIME_PAUSE:      return "Pause";
    case ID_ACTION_TIME_REVERSE:    return "Reverse";
    case ID_ACTION_SCALE_JUMP:      return "Scale Jump";
    case ID_ACTION_COUNT:           return "Unknown";
    }
    return "Unknown";
}

/* ------------------------------------------------------------------
 * id_key_label — key label for display
 * ------------------------------------------------------------------ */

const char *id_key_label(id_action_t action)
{
    switch (action) {
    case ID_ACTION_NONE:            return "";
    case ID_ACTION_VIEW_SPACE:      return "";
    case ID_ACTION_VIEW_EARTH:      return "E";
    case ID_ACTION_VIEW_GALAXY:     return "";
    case ID_ACTION_FOCUS_ASTROLOGY: return "A";
    case ID_ACTION_FOCUS_KIN:       return "K";
    case ID_ACTION_FOCUS_ICHING:    return "I";
    case ID_ACTION_FOCUS_CHINESE:   return "C";
    case ID_ACTION_FOCUS_HD:        return "D";
    case ID_ACTION_FOCUS_OVERVIEW:  return "Esc";
    case ID_ACTION_TOGGLE_TRAILS:   return "T";
    case ID_ACTION_TOGGLE_HUD:      return "H";
    case ID_ACTION_TOGGLE_STARS:    return "S";
    case ID_ACTION_TOGGLE_LABELS:   return "L";
    case ID_ACTION_TOGGLE_GRID:     return "G";
    case ID_ACTION_TOGGLE_PAUSE:    return "Space";
    case ID_ACTION_TIME_REALTIME:   return "1";
    case ID_ACTION_TIME_FAST:       return "2";
    case ID_ACTION_TIME_FASTER:     return "3";
    case ID_ACTION_TIME_DAY_SEC:    return "4";
    case ID_ACTION_TIME_10DAY_SEC:  return "5";
    case ID_ACTION_TIME_PAUSE:      return "0";
    case ID_ACTION_TIME_REVERSE:    return "-";
    case ID_ACTION_SCALE_JUMP:      return "Shift+N";
    case ID_ACTION_COUNT:           return "";
    }
    return "";
}

/* ------------------------------------------------------------------
 * Category queries
 * ------------------------------------------------------------------ */

int id_is_focus_action(id_action_t action)
{
    return action == ID_ACTION_FOCUS_ASTROLOGY
        || action == ID_ACTION_FOCUS_KIN
        || action == ID_ACTION_FOCUS_ICHING
        || action == ID_ACTION_FOCUS_CHINESE
        || action == ID_ACTION_FOCUS_HD
        || action == ID_ACTION_FOCUS_OVERVIEW;
}

int id_is_time_action(id_action_t action)
{
    return action == ID_ACTION_TIME_REALTIME
        || action == ID_ACTION_TIME_FAST
        || action == ID_ACTION_TIME_FASTER
        || action == ID_ACTION_TIME_DAY_SEC
        || action == ID_ACTION_TIME_10DAY_SEC
        || action == ID_ACTION_TIME_PAUSE
        || action == ID_ACTION_TIME_REVERSE;
}

int id_is_toggle_action(id_action_t action)
{
    return action == ID_ACTION_TOGGLE_TRAILS
        || action == ID_ACTION_TOGGLE_HUD
        || action == ID_ACTION_TOGGLE_STARS
        || action == ID_ACTION_TOGGLE_LABELS
        || action == ID_ACTION_TOGGLE_GRID
        || action == ID_ACTION_TOGGLE_PAUSE;
}

/* ------------------------------------------------------------------
 * id_time_speed — speed multiplier for time actions
 * ------------------------------------------------------------------ */

int id_time_speed(id_action_t action)
{
    switch (action) {
    case ID_ACTION_TIME_REALTIME:  return 1;
    case ID_ACTION_TIME_FAST:      return 60;
    case ID_ACTION_TIME_FASTER:    return 3600;
    case ID_ACTION_TIME_DAY_SEC:   return 86400;
    case ID_ACTION_TIME_10DAY_SEC: return 864000;
    case ID_ACTION_TIME_PAUSE:     return 0;
    case ID_ACTION_TIME_REVERSE:   return -1;
    default:                       return 0;
    }
}
