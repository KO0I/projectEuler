// Project Euler 86: Cuboid Route.
import euler from "../../support/goose";

fn solve() -> i64 {
    var count = 0;
    var m = 0;
    while count <= 1000000 {
        m++;
        for s in 2..2 * m + 1 {
            if is_square(m * m + s * s) {
                count += max(0, s / 2 - max(1, s - m) + 1);
            }
        }
    }
    return m;
}

fn main() {
    print(solve());
}
