/* sensitivity.c -- Cultural sensitivity levels for content modules.
 *
 * Pure module: no globals, no malloc, no side effects. */

#include "sensitivity.h"

const char *sensitivity_name(sensitivity_t level)
{
    switch (level) {
    case SENSITIVITY_GENERAL:    return "General";
    case SENSITIVITY_RESPECTFUL: return "Respectful";
    case SENSITIVITY_SACRED:     return "Sacred";
    default:                     return "Unknown";
    }
}
