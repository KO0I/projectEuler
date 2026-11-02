// Project Euler 4: Largest Palindrome Product.
import euler from "../../support/goose";

fn solve() -> i64 {
    var best = 0;
    for a in 100..1000 {
        for b in a..1000 {
            let n = a * b;
            if n > best && n == reverse(n) {
                best = n;
            }
        }
    }
    return best;
}

fn main() {
    print(solve());
}
