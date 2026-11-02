// Project Euler 23: Non-Abundant Sums.
import euler from "../../support/goose";

fn solve() -> i64 {
    var abundant: i64[>..] = [];
    for n in 1..28124 {
        if divisor_sum(n) > n {
            abundant.push(n);
        }
    }
    var sums = [false; 28124];
    for i in abundant.len {
        for j in i..abundant.len {
            let s = abundant[i] + abundant[j];
            if s > 28123 {
                break;
            }
            sums[s] = true;
        }
    }
    var sum = 0;
    for n in 1..28124 {
        if !sums[n] {
            sum += n;
        }
    }
    return sum;
}

fn main() {
    print(solve());
}
