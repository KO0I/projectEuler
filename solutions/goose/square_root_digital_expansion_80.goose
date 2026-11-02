// Project Euler 80: Square Root Digital Expansion.
import euler from "../../support/goose";

fn solve() -> i64 {
    var total = 0;
    for n in 1..101 {
        if is_square(n) {
            continue;
        }
        var root = big(0);
        var rem = big(0);
        for digit in 100 {
            multiply(rem, 100);
            if digit == 0 {
                add(rem, big(n));
            }
            var x = 9;
            var trial = big(0);
            loop {
                trial = root;
                multiply(trial, 20);
                add(trial, big(x));
                multiply(trial, x);
                if big_compare(trial, rem) <= 0 {
                    break;
                }
                x--;
            }
            subtract(rem, trial);
            multiply(root, 10);
            add(root, big(x));
        }
        total += big_sum(root);
    }
    return total;
}

fn main() {
    print(solve());
}
