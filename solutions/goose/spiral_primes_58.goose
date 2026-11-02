// Project Euler 58: Spiral Primes.
import euler from "../../support/goose";

fn solve() -> i64 {
    var count = 0;
    var side = 3;
    loop {
        let sq = side * side;
        for k in 1..4 {
            if prime(sq - k * (side - 1)) {
                count++;
            }
        }
        if count * 10 < 2 * side - 1 {
            return side;
        }
        side += 2;
    }
}

fn main() {
    print(solve());
}
