// Project Euler 15: Lattice Paths.
import euler from "../../support/goose";

fn solve() -> i64 {
    var a = [1; 21];
    for _ in 20 {
        for j in 1..21 {
            a[j] += a[j - 1];
        }
    }
    return a[20];
}

fn main() {
    print(solve());
}
