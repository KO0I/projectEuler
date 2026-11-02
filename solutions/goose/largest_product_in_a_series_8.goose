// Project Euler 8: Largest Product in a Series.
import euler from "../../support/goose";

fn solve() -> i64 {
    let t = file_text("tests/data/p008.txt");
    var a: i64[>..] = [];
    for c in t {
        if c >= '0' && c <= '9' {
            a.push(c - '0');
        }
    }
    assert(a.len == 1000);
    var best = 0;
    for i in 0..a.len - 12 {
        var p = 1;
        for j in 13 {
            p *= a[i + j];
        }
        best = max(best, p);
    }
    return best;
}

fn main() {
    print(solve());
}
