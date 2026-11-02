// Project Euler 18: Maximum Path Sum I.
import euler from "../../support/goose";

fn solve() -> i64 {
    var a = numbers("tests/data/p018_triangle.txt");
    assert(a.len == 120);
    var row = 14;
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
