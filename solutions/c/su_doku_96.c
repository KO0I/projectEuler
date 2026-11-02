/* Project Euler 96: https://projecteuler.net/problem=96
 * Bit masks enforce rows, columns and boxes. Backtracking chooses the
 * empty cell with the fewest candidates and checks solution uniqueness. */
#include "euler_extra.h"

typedef struct {
    unsigned cells[81], solution[81];
    unsigned row[9], column[9], box[9], solutions;
} Puzzle;

static unsigned bit_count(unsigned mask) {
    unsigned count = 0;
    while (mask) { mask &= mask - 1; ++count; }
    return count;
}
static void solve(Puzzle *p) {
    if (p->solutions >= 2) return;
    unsigned chosen = 81, options = 0, minimum = 10;
    for (unsigned i = 0; i < 81; ++i) if (!p->cells[i]) {
        unsigned r = i / 9, c = i % 9, b = r / 3 * 3 + c / 3;
        unsigned mask = 0x3fe & ~(p->row[r] | p->column[c] | p->box[b]);
        unsigned count = bit_count(mask);
        if (!count) return;
        if (count < minimum) { chosen = i; options = mask; minimum = count; }
    }
    if (chosen == 81) {
        if (++p->solutions == 1) memcpy(p->solution, p->cells, sizeof p->cells);
        return;
    }
    unsigned r = chosen / 9, c = chosen % 9, b = r / 3 * 3 + c / 3;
    for (unsigned digit = 1; digit <= 9; ++digit) if (options & (1U << digit)) {
        unsigned bit = 1U << digit;
        p->cells[chosen] = digit;
        p->row[r] |= bit; p->column[c] |= bit; p->box[b] |= bit;
        solve(p);
        p->row[r] &= ~bit; p->column[c] &= ~bit; p->box[b] &= ~bit;
        p->cells[chosen] = 0;
    }
}
static void read_line(FILE *file, char *line, size_t capacity) {
    if (!fgets(line, (int)capacity, file)) euler_fail("Truncated Sudoku input");
    line[strcspn(line, "\r\n")] = '\0';
}
int main(void) {
    FILE *file = euler_open("tests/data/p096_sudoku.txt");
    unsigned total = 0;
    for (unsigned index = 1; index <= 50; ++index) {
        Puzzle puzzle = {0};
        char line[64], expected[32];
        read_line(file, line, sizeof line);
        snprintf(expected, sizeof expected, "Grid %02u", index);
        if (strcmp(line, expected)) euler_fail("Invalid Sudoku grid header");
        for (unsigned r = 0; r < 9; ++r) {
            read_line(file, line, sizeof line);
            if (strlen(line) != 9) euler_fail("Invalid Sudoku row length");
            for (unsigned c = 0; c < 9; ++c) {
                if (line[c] < '0' || line[c] > '9') euler_fail("Invalid Sudoku digit");
                unsigned digit = line[c] - '0', b = r / 3 * 3 + c / 3;
                puzzle.cells[9 * r + c] = digit;
                if (!digit) continue;
                unsigned bit = 1U << digit;
                if ((puzzle.row[r] | puzzle.column[c] | puzzle.box[b]) & bit)
                    euler_fail("Conflicting Sudoku clues");
                puzzle.row[r] |= bit; puzzle.column[c] |= bit; puzzle.box[b] |= bit;
            }
        }
        solve(&puzzle);
        if (puzzle.solutions != 1) euler_fail("Sudoku does not have exactly one solution");
        total += 100 * puzzle.solution[0] + 10 * puzzle.solution[1] + puzzle.solution[2];
    }
    int ch;
    while ((ch = fgetc(file)) != EOF) if (!isspace((unsigned char)ch))
        euler_fail("Extra Sudoku input");
    if (ferror(file)) euler_fail("Sudoku input read failed");
    fclose(file);
    printf("%u\n", total);
    return 0;
}
