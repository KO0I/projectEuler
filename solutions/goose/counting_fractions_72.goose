// Project Euler 72: Counting Fractions.
import euler from "../../support/goose";

fn solve() -> i64 {
    var phi = [0; 1000001];
    for n in 1000001 {
        phi[n] = n;
    }
    for p in 2..1000001 {
        if phi[p] == p {
            var n = p;
            while n <= 1000000 {
                phi[n] -= phi[n] / p;
                n += p;
            }
        }
    }
    var sum = 0;
    for n in 2..1000001 {
        sum += phi[n];
    }
    return sum;
}

fn main() {
    print(solve());
}
