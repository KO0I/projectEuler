// Project Euler 76: Counting Summations.
import euler from "../../support/goose";

fn solve() -> i64 {
    var ways = [0; 101];
    ways[0] = 1;
    for part in 1..100 {
        for n in part..101 {
            ways[n] += ways[n - part];
        }
    }
    return ways[100];
}

fn main() {
    print(solve());
}
