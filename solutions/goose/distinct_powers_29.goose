// Project Euler 29: Distinct Powers.
import euler from "../../support/goose";

fn solve() -> i64 {
    var seen: i64[>..] = [];
    for a in 2..101 {
        var base = a;
        var exponent = 1;
        for e in 2..7 {
            for r in 2..11 {
                if ipow(r, e) == a {
                    base = r;
                    exponent = e;
                }
            }
        }
        for b in 2..101 {
            let key = base * 1000 + exponent * b;
            if !contains(seen[..], key) {
                seen.push(key);
            }
        }
    }
    return seen.len;
}

fn main() {
    print(solve());
}
