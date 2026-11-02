// Project Euler 47: Distinct Primes Factors.
import euler from "../../support/goose";

fn solve() -> i64 {
    var counts = [0; 200000];
    for p in 2..200000 {
        if counts[p] == 0 {
            var n = p;
            while n < 200000 {
                counts[n]++;
                n += p;
            }
        }
    }
    var run = 0;
    for n in 2..200000 {
        run = if counts[n] == 4 {
            run + 1
        } else {
            0
        };
        if run == 4 {
            return n - 3;
        }
    }
    abort("increase sieve");
}

fn main() {
    print(solve());
}
