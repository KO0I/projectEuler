// Project Euler 75: Singular Integer Right Triangles.
import euler from "../../support/goose";

fn solve() -> i64 {
    var counts = [0; 1500001];
    for m in 2..867 {
        for n in 1..m {
            if (m - n) % 2 == 1 && gcd(m, n) == 1 {
                let p = 2 * m * (m + n);
                var k = p;
                while k <= 1500000 {
                    counts[k]++;
                    k += p;
                }
            }
        }
    }
    var count = 0;
    for n in counts {
        if n == 1 {
            count++;
        }
    }
    return count;
}

fn main() {
    print(solve());
}
