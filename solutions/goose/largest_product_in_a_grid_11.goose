// Project Euler 11: Largest Product in a Grid.
import euler from "../../support/goose";

fn solve() -> i64 {
    let a = numbers("tests/data/p011.txt");
    assert(a.len == 400);
    var best = 0;
    let dx = [1, 0, 1, -1];
    let dy = [0, 1, 1, 1];
    for y in 20 {
        for x in 20 {
            for k in 4 {
                let ex = x + 3 * dx[k];
                let ey = y + 3 * dy[k];
                if ex >= 0 && ex < 20 && ey < 20 {
                    var p = 1;
                    for j in 4 {
                        p *= a[(y + j * dy[k]) * 20 + x + j * dx[k]];
                    }
                    best = max(best, p);
                }
            }
        }
    }
    return best;
}

fn main() {
    print(solve());
}
