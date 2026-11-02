// Project Euler 14: Longest Collatz Sequence.
import euler from "../../support/goose";

fn solve() -> i64 {
    var cache = [0; 1000000];
    cache[1] = 1;
    var best = 1;
    for start in 2..1000000 {
        var n = start;
        var length = 0;
        while n >= 1000000 || cache[n] == 0 {
            n = if n % 2 == 0 {
                n / 2
            } else {
                3 * n + 1
            };
            length++;
        }
        cache[start] = length + cache[n];
        if cache[start] > cache[best] {
            best = start;
        }
    }
    return best;
}

fn main() {
    print(solve());
}
