// Project Euler 24: Lexicographic Permutations.
import euler from "../../support/goose";

fn solve() -> i64 {
    var a = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9];
    for _ in 999999 {
        assert(next_perm(a[..]));
    }
    var n = 0;
    for d in a {
        n = n * 10 + d;
    }
    return n;
}

fn main() {
    print(solve());
}
