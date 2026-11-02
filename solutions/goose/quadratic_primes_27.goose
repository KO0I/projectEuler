// Project Euler 27: Quadratic Primes.
import euler from "../../support/goose";

fn solve() -> i64 {
    var best = 0;
    var answer = 0;
    for a in -999..1000 {
        for b in 2..1001 {
            if !prime(b) {
                continue;
            }
            var n = 0;
            while prime(n * n + a * n + b) {
                n++;
            }
            if n > best {
                best = n;
                answer = a * b;
            }
        }
    }
    return answer;
}

fn main() {
    print(solve());
}
