/* https://projecteuler.net/problem=79
 * Each observed triple contributes two precedence edges. The supplied data
 * admit a topological ordering using each digit once. This reaches the lower
 * bound of one occurrence per observed digit, so the passcode is shortest.
 * Repeated digits or cyclic constraints are rejected, not silently discarded. */
#include "euler_extra.h"

int main(void) {
    bool edges[10][10] = {{false}}, present[10] = {false}, emitted[10] = {false};
    FILE *file = euler_open("tests/data/p079_keylog.txt");
    unsigned lines = 0;
    char token[5];
    while (fscanf(file, "%4s", token) == 1) {
        if (strlen(token) != 3) euler_fail("Expected three-digit login attempts");
        int digit[3];
        for (int i = 0; i < 3; ++i) {
            if (token[i] < '0' || token[i] > '9') euler_fail("Invalid login digit");
            digit[i] = token[i] - '0'; present[digit[i]] = true;
        }
        if (digit[0] == digit[1] || digit[0] == digit[2] || digit[1] == digit[2])
            euler_fail("Repeated digit requires a different passcode model");
        edges[digit[0]][digit[1]] = true; edges[digit[1]][digit[2]] = true;
        ++lines;
    }
    if (ferror(file) || lines != 50) euler_fail("Expected 50 login attempts");
    fclose(file);
    char answer[11]; unsigned length = 0, total = 0;
    for (int i = 0; i < 10; ++i) total += present[i];
    while (length < total) {
        int next = -1;
        for (int candidate = 0; candidate < 10; ++candidate) if (present[candidate] && !emitted[candidate]) {
            bool blocked = false;
            for (int previous = 0; previous < 10; ++previous)
                if (edges[previous][candidate] && !emitted[previous]) blocked = true;
            if (!blocked) { next = candidate; break; }
        }
        if (next < 0) euler_fail("Cyclic login constraints require repeated digits");
        answer[length++] = (char)('0' + next); emitted[next] = true;
    }
    answer[length] = '\0';
    puts(answer);
    return 0;
}
