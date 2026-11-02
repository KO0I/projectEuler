// Project Euler 77: Prime Summations.
import euler from "../../support/goose";

fn solve() -> i64 {
    var limit = 10;
    loop {
        var ways = [0; 1001];
        assert(limit <= 1000);
        ways[0] = 1;
        for p in 2..limit + 1 {
            if prime(p) {
                for n in p..limit + 1 {
                    ways[n] += ways[n - p];
                }
            }
        }
        for n in 2..limit + 1 {
            if ways[n] > 5000 {
                return n;
            }
        }
        limit *= 2;
    }
}

fn main() {
    print(solve());
}
