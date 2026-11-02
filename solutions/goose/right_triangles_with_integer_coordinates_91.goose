// Project Euler 91: Right Triangles with Integer Coordinates.
import euler from "../../support/goose";

fn solve() -> i64 {
    var count = 0;
    for x1 in 51 {
        for y1 in 51 {
            if x1 == 0 && y1 == 0 {
                continue;
            }
            for x2 in 51 {
                for y2 in 51 {
                    if (x2 == 0 && y2 == 0) || x1 * 51 + y1 >= x2 * 51 + y2 {
                        continue;
                    }
                    let origin = x1 * x2 + y1 * y2;
                    let atp = x1 * (x1 - x2) + y1 * (y1 - y2);
                    let atq = x2 * (x2 - x1) + y2 * (y2 - y1);
                    if origin == 0 || atp == 0 || atq == 0 {
                        count++;
                    }
                }
            }
        }
    }
    return count;
}

fn main() {
    print(solve());
}
