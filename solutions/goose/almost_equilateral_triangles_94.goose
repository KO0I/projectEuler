// Project Euler 94: Almost Equilateral Triangles.
import euler from "../../support/goose";

fn solve() -> i64 {
    var x = 2;
    var y = 0;
    var total = 0;
    loop {
        let xx = 2 * x + 3 * y;
        y = x + 2 * y;
        x = xx;
        var perimeter = 0;
        if (x + 1) % 3 == 0 {
            let a = (x + 1) / 3;
            if a > 1 {
                perimeter = 3 * a + 1;
            }
        } else if (x - 1) % 3 == 0 {
            let a = (x - 1) / 3;
            if a > 1 {
                perimeter = 3 * a - 1;
            }
        }
        if perimeter > 1000000000 {
            break;
        }
        total += perimeter;
    }
    return total;
}

fn main() {
    print(solve());
}
