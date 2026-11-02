// Project Euler 66: Diophantine Equation.
import euler from "../../support/goose";

fn solve() -> i64 {
    var best = big(0);
    var answer = 0;
    for d0 in 2..1001 {
        let a0 = sqrt(d0 as f64) as! i64;
        if a0 * a0 == d0 {
            continue;
        }
        var m = 0;
        var d = 1;
        var a = a0;
        var period = 0;
        loop {
            m = d * a - m;
            d = (d0 - m * m) / d;
            a = (a0 + m) / d;
            period++;
            if a == 2 * a0 {
                break;
            }
        }
        let terms = if period % 2 == 0 {
            period
        } else {
            2 * period
        };
        var p0 = big(0);
        var p1 = big(1);
        m = 0;
        d = 1;
        a = a0;
        for _ in terms {
            var p = p1;
            multiply(p, a);
            add(p, p0);
            p0 = p1;
            p1 = p;
            m = d * a - m;
            d = (d0 - m * m) / d;
            a = (a0 + m) / d;
        }
        if big_compare(p1, best) > 0 {
            best = p1;
            answer = d0;
        }
    }
    return answer;
}

fn main() {
    print(solve());
}
