// Project Euler 85: Counting Rectangles.
import euler from "../../support/goose";

fn solve() -> i64 {
    var best = 2000000;
    var area = 0;
    for a in 1..2001 {
        for b in a..2001 {
            let count = a * (a + 1) * b * (b + 1) / 4;
            let diff = abs(count - 2000000);
            if diff < best {
                best = diff;
                area = a * b;
            }
            if count > 2000000 {
                break;
            }
        }
    }
    return area;
}

fn main() {
    print(solve());
}
