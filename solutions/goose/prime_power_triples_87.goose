// Project Euler 87: Prime Power Triples.
import euler from "../../support/goose";

fn solve() -> i64 {
    var primes: i64[>..] = [];
    for p in 2..7072 {
        if prime(p) {
            primes.push(p);
        }
    }
    var seen = [false; 50000000];
    for a in primes {
        if a * a >= 50000000 {
            break;
        }
        for b in primes {
            let s = a * a + b * b * b;
            if s >= 50000000 {
                break;
            }
            for c in primes {
                let v = s + c * c * c * c;
                if v >= 50000000 {
                    break;
                }
                seen[v] = true;
            }
        }
    }
    var count = 0;
    for v in seen {
        if v {
            count++;
        }
    }
    return count;
}

fn main() {
    print(solve());
}
