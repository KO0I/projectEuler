// Project Euler 45: Triangular, Pentagonal, and Hexagonal.
import euler from "../../support/goose";

fn solve() -> i64 {
    var k = 144;
    loop {
        let h = k * (2 * k - 1);
        let r = sqrt((24 * h + 1) as f64) as! i64;
        if r * r == 24 * h + 1 && (r + 1) % 6 == 0 {
            return h;
        }
        k++;
    }
}

fn main() {
    print(solve());
}
