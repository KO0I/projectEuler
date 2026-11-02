// Project Euler 67: Maximum Path Sum II.
import euler from "../../support/goose";

fn solve() -> i64 {
    var a = numbers("tests/data/p067_triangle.txt");
    assert(a.len == 5050);
    var row = 99;
    while row > 0 {
        let start = row * (row + 1) / 2;
        let prev = (row - 1) * row / 2;
        for j in row {
            a[prev + j] += max(a[start + j], a[start + j + 1]);
        }
        row--;
    }
    return a[0];
}

fn main() {
    print(solve());
}
