#include "kin_board_layout.h"
#include "../systems/tzolkin/tzolkin_board.h"
#include <string.h>

int kin_board_kin_at(int row, int col)
{
    if (row < 0 || row > 19 || col < 0 || col > 12)
        return 0;
    return tzolkin_board_kin_number(col + 1, row);
}

int kin_board_cell_index(int kin)
{
    if (kin < 1 || kin > 260)
        return -1;

    int seal = (kin - 1) % 20;
    int tone = ((kin - 1) % 13) + 1;
    return seal * KIN_BOARD_COLS + (tone - 1);
}

kin_board_layout_t kin_board_compute(int today_kin)
{
    kin_board_layout_t board;
    memset(&board, 0, sizeof(board));

    /* Reject out-of-range (but 0 is valid = no highlight) */
    if (today_kin < 0 || today_kin > 260)
        return board;

    board.today_kin = today_kin;
    board.portal_count = 0;

    /* Cell dimensions in normalized coords */
    float cw = 1.0f / (float)KIN_BOARD_COLS;
    float ch = 1.0f / (float)KIN_BOARD_ROWS;

    for (int row = 0; row < KIN_BOARD_ROWS; row++) {
        for (int col = 0; col < KIN_BOARD_COLS; col++) {
            int idx = row * KIN_BOARD_COLS + col;
            int kin = tzolkin_board_kin_number(col + 1, row);

            float cx = (float)col * cw;
            float cy = (float)row * ch;

            board.cells[idx] = kin_cell_create(kin, cx, cy, cw, ch);

            if (kin == today_kin && today_kin > 0) {
                board.cells[idx].highlighted = 1;
                board.today_row = row;
                board.today_col = col;
            }

            if (tzolkin_board_is_portal(kin))
                board.portal_count++;
        }
    }

    return board;
}
