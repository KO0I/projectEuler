// Project Euler 39: Integer Right Triangles.
import euler from "../../support/goose";

fn solve() -> i64 {
    var count = [0; 1001];
    for a in 1..334 {
        for b in a..501 {
            let c = sqrt((a * a + b * b) as f64) as! i64;
            if c * c == a * a + b * b && a + b + c <= 1000 {
                count[a + b + c]++;
            }
        }
    }
    var best = 0;
    for p in 1..1001 {
        if count[p] > count[best] {
            best = p;
        }
    }
    return best;
}

fn main() {
    print(solve());
}
