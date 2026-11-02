// Project Euler 49: Prime Permutations.
import euler from "../../support/goose";

fn solve() -> i64 {
    for a in 1000..10000 {
        if a == 1487 || !prime(a) {
            continue;
        }
        for b in a + 1..10000 {
            let c = 2 * b - a;
            if c >= 10000 {
                break;
            }
            if signature(a) == signature(b) && signature(a) == signature(c) && prime(b) && prime(c) {
                return a * 100000000 + b * 10000 + c;
            }
        }
    }
    abort("no progression");
}

fn main() {
    print(solve());
}
