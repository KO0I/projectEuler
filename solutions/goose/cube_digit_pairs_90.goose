// Project Euler 90: Cube Digit Pairs.
import euler from "../../support/goose";

fn solve() -> i64 {
    var count = 0;
    for a in 1..1024 {
        if popcount(a as u64) != 6 {
            continue;
        }
        for b in a..1024 {
            if popcount(b as u64) != 6 {
                continue;
            }
            var aa = a;
            var bb = b;
            if aa & ((1 << 6) | (1 << 9)) != 0 {
                aa |= (1 << 6) | (1 << 9);
            }
            if bb & ((1 << 6) | (1 << 9)) != 0 {
                bb |= (1 << 6) | (1 << 9);
            }
            var ok = true;
            for n in [1, 4, 9, 16, 25, 36, 49, 64, 81] {
                let x = 1 << (n / 10);
                let y = 1 << (n % 10);
                if !((aa & x != 0 && bb & y != 0) || (aa & y != 0 && bb & x != 0)) {
                    ok = false;
                }
            }
            if ok {
                count++;
            }
        }
    }
    return count;
}

fn main() {
    print(solve());
}
