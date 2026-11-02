// Project Euler 19: Counting Sundays.
import euler from "../../support/goose";

fn solve() -> i64 {
    let days = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
    var w = 1;
    var count = 0;
    for y in 1900..2001 {
        for m in 12 {
            if y >= 1901 && w == 0 {
                count++;
            }
            var d = days[m];
            if m == 1 && y % 4 == 0 && (y % 100 != 0 || y % 400 == 0) {
                d++;
            }
            w = (w + d) % 7;
        }
    }
    return count;
}

fn main() {
    print(solve());
}
