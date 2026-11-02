// Project Euler 81: Path Sum: Two Ways.
import euler from "../../support/goose";

fn solve() -> i64 {
    var a = numbers("tests/data/p081_matrix.txt");
    assert(a.len == 6400);
    for y in 80 {
        for x in 80 {
            if x == 0 && y == 0 {
                continue;
            }
            let left = if x > 0 {
                a[80 * y + x - 1]
            } else {
                1000000000
            };
            let up = if y > 0 {
                a[80 * (y - 1) + x]
            } else {
                1000000000
            };
            a[y * 80 + x] += min(left, up);
        }
    }
    return a[6399];
}

fn main() {
    print(solve());
}
