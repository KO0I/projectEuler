// Project Euler 65: Convergents of e.
import euler from "../../support/goose";

fn solve() -> i64 {
    var p0 = big(0);
    var p1 = big(1);
    for k in 100 {
        let a = if k == 0 {
            2
        } else if k % 3 == 2 {
            2 * ((k + 1) / 3)
        } else {
            1
        };
        var p = p1;
        multiply(p, a);
        add(p, p0);
        p0 = p1;
        p1 = p;
    }
    return big_sum(p1);
}

fn main() {
    print(solve());
}
