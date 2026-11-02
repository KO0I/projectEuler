// Project Euler 70: Totient Permutation.
import euler from "../../support/goose";

fn solve() -> i64 {
    var phi: i64[>..] = [];
    for n in 10000000 {
        phi.push(n);
    }
    for p in 2..10000000 {
        if phi[p] == p {
            var n = p;
            while n < 10000000 {
                phi[n] -= phi[n] / p;
                n += p;
            }
        }
    }
    var best = 0;
    for n in 2..10000000 {
        if signature(n) == signature(phi[n]) && (best == 0 || n * phi[best] < best * phi[n]) {
            best = n;
        }
    }
    return best;
}

fn main() {
    print(solve());
}
