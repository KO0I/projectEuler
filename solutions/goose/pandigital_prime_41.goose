// Project Euler 41: Pandigital Prime.
import euler from "../../support/goose";

fn solve() -> i64 {
    var a = [1, 2, 3, 4, 5, 6, 7];
    var best = 0;
    loop {
        var n = 0;
        for d in a {
            n = n * 10 + d;
        }
        if prime(n) {
            best = n;
        }
        if !next_perm(a[..]) {
            break;
        }
    }
    return best;
}

fn main() {
    print(solve());
}
