// Project Euler 43: Sub-string Divisibility.
import euler from "../../support/goose";

fn solve() -> i64 {
    var a = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9];
    let div = [2, 3, 5, 7, 11, 13, 17];
    var sum = 0;
    loop {
        var ok = a[0] != 0;
        for k in 7 {
            if (100 * a[k + 1] + 10 * a[k + 2] + a[k + 3]) % div[k] != 0 {
                ok = false;
                break;
            }
        }
        if ok {
            var n = 0;
            for d in a {
                n = n * 10 + d;
            }
            sum += n;
        }
        if !next_perm(a[..]) {
            break;
        }
    }
    return sum;
}

fn main() {
    print(solve());
}
