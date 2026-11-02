// Project Euler 78: Coin Partitions.
import euler from "../../support/goose";

fn solve() -> i64 {
    var p: i64[>..] = [1];
    var n = 1;
    loop {
        var sum = 0;
        var k = 1;
        while k * (3 * k - 1) / 2 <= n {
            let sign = if k % 2 == 1 {
                1
            } else {
                -1
            };
            sum += sign * p[n - k * (3 * k - 1) / 2];
            let b = k * (3 * k + 1) / 2;
            if b <= n {
                sum += sign * p[n - b];
            }
            sum %= 1000000;
            k++;
        }
        sum %= 1000000;
        p.push(sum);
        if sum == 0 {
            return n;
        }
        n++;
    }
}

fn main() {
    print(solve());
}
