// Project Euler 55: Lychrel Numbers.
import euler from "../../support/goose";

fn solve() -> i64 {
    var count = 0;
    for n in 1..10000 {
        var b = big(n);
        var ok = false;
        for _ in 50 {
            var r = big(0);
            r.len = b.len;
            for i in b.len {
                r.d[i] = b.d[b.len - 1 - i];
            }
            add(b, r);
            var pal = true;
            for i in b.len / 2 {
                if b.d[i] != b.d[b.len - 1 - i] {
                    pal = false;
                }
            }
            if pal {
                ok = true;
                break;
            }
        }
        if !ok {
            count++;
        }
    }
    return count;
}

fn main() {
    print(solve());
}
