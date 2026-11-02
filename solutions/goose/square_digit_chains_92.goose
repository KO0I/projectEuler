// Project Euler 92: Square Digit Chains.
import euler from "../../support/goose";

fn square_digits(n0: i64) -> i64 {
    var n = n0;
    var s = 0;
    while n > 0 {
        let d = n % 10;
        s += d * d;
        n /= 10;
    }
    return s;
}

fn solve() -> i64 {
    var terminal = [0; 568];
    terminal[1] = 1;
    terminal[89] = 89;
    for n in 1..568 {
        var k = n;
        while terminal[k] == 0 {
            k = square_digits(k);
        }
        terminal[n] = terminal[k];
    }
    var count = 0;
    for n in 1..10000000 {
        if terminal[square_digits(n)] == 89 {
            count++;
        }
    }
    return count;
}

fn main() {
    print(solve());
}
