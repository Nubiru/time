#include "kin_cell.h"

/* Canonical Arguelles directional colors (normalized RGBA). */
static const float COLOR_RGBA[4][4] = {
    {0.85f, 0.20f, 0.20f, 1.0f},  /* Red */
    {0.90f, 0.90f, 0.90f, 1.0f},  /* White */
    {0.20f, 0.40f, 0.80f, 1.0f},  /* Blue */
    {0.90f, 0.80f, 0.20f, 1.0f}   /* Yellow */
};

kin_cell_t kin_cell_from_kin(int kin)
{
    kin_cell_t cell = {0, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0};
    if (kin < 1 || kin > 260)
        return cell;

    cell.kin = kin;
    cell.seal = (kin - 1) % 20;
    cell.tone = (kin - 1) % 13 + 1;
    cell.color = cell.seal % 4;
    return cell;
}

kin_cell_t kin_cell_create(int kin, float x, float y, float w, float h)
{
    kin_cell_t cell = kin_cell_from_kin(kin);
    if (cell.kin == 0)
        return cell;

    cell.x = x;
    cell.y = y;
    cell.w = w;
    cell.h = h;
    return cell;
}

int kin_cell_color(int seal)
{
    if (seal < 0 || seal > 19)
        return -1;
    return seal % 4;
}

int kin_cell_rgba(int color, float rgba[4])
{
    if (color < 0 || color > 3)
        return 0;

    rgba[0] = COLOR_RGBA[color][0];
    rgba[1] = COLOR_RGBA[color][1];
    rgba[2] = COLOR_RGBA[color][2];
    rgba[3] = COLOR_RGBA[color][3];
    return 1;
}
