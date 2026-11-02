// Project Euler 3: Largest Prime Factor.
import euler from "../../support/goose";

fn solve() -> i64 {
    var n = 600851475143;
    var p = 2;
    var best = 1;
    while p <= n / p {
        while n % p == 0 {
            best = p;
            n /= p;
        }
        p++;
    }
    return max(best, n);
}

fn main() {
    print(solve());
}
