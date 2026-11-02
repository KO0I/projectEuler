// Project Euler 9: Special Pythagorean Triplet.
import euler from "../../support/goose";

fn solve() -> i64 {
    for a in 1..334 {
        for b in a + 1..500 {
            let c = 1000 - a - b;
            if a * a + b * b == c * c {
                return a * b * c;
            }
        }
    }
    abort("no triple");
}

fn main() {
    print(solve());
}
