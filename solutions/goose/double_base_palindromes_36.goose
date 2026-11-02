// Project Euler 36: Double-base Palindromes.
import euler from "../../support/goose";

fn solve() -> i64 {
    var sum = 0;
    for n in 1..1000000 {
        if n == reverse(n) && n == reverse(n, 2) {
            sum += n;
        }
    }
    return sum;
}

fn main() {
    print(solve());
}
