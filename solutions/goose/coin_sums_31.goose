// Project Euler 31: Coin Sums.
import euler from "../../support/goose";

fn solve() -> i64 {
    var ways = [0; 201];
    ways[0] = 1;
    for c in [1, 2, 5, 10, 20, 50, 100, 200] {
        for n in c..201 {
            ways[n] += ways[n - c];
        }
    }
    return ways[200];
}

fn main() {
    print(solve());
}
