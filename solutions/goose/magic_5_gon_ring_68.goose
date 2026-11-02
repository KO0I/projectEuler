// Project Euler 68: Magic 5-gon Ring.
import euler from "../../support/goose";

fn solve() -> i64 {
    var a = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    var best = 0;
    loop {
        var valid = true;
        for i in 1..5 {
            if a[i] < a[0] {
                valid = false;
            }
        }
        for i in 5..10 {
            if a[i] == 10 {
                valid = false;
            }
        }
        if valid {
            let sum = a[0] + a[5] + a[6];
            for i in 1..5 {
                if a[i] + a[5 + i] + a[5 + (i + 1) % 5] != sum {
                    valid = false;
                }
            }
            if valid {
                var n = 0;
                for i in 5 {
                    for v in [a[i], a[5 + i], a[5 + (i + 1) % 5]] {
                        n = n * ipow(10, digits(v)) + v;
                    }
                }
                best = max(best, n);
            }
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
