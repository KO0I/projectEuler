// Project Euler 64: Odd Period Square Roots.
import euler from "../../support/goose";

fn solve() -> i64 {
    var count = 0;
    for n in 2..10001 {
        let a0 = sqrt(n as f64) as! i64;
        if a0 * a0 == n {
            continue;
        }
        var m = 0;
        var d = 1;
        var a = a0;
        var period = 0;
        loop {
            m = d * a - m;
            d = (n - m * m) / d;
            a = (a0 + m) / d;
            period++;
            if a == 2 * a0 {
                break;
            }
        }
        if period % 2 == 1 {
            count++;
        }
    }
    return count;
}

fn main() {
    print(solve());
}
