// Project Euler 100: Arranged Probability.
import euler from "../../support/goose";

fn solve() -> i64 {
    var blue = 15;
    var total = 21;
    while total <= 1000000000000 {
        let b = 3 * blue + 2 * total - 2;
        total = 4 * blue + 3 * total - 3;
        blue = b;
    }
    return blue;
}

fn main() {
    print(solve());
}
