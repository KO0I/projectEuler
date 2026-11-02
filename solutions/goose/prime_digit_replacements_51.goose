// Project Euler 51: Prime Digit Replacements.
import euler from "../../support/goose";

fn solve() -> i64 {
    let primes = sieve(999999);
    for n in 11..1000000 {
        if !primes[n] {
            continue;
        }
        let len = digits(n);
        for mask in 1..(1 << len) {
            var original = -1;
            var valid = true;
            var place = 1;
            var amount = 0;
            for i in len {
                if mask & (1 << i) != 0 {
                    let d = n / place % 10;
                    if original >= 0 && d != original {
                        valid = false;
                    }
                    original = d;
                    amount += place;
                }
                place *= 10;
            }
            if !valid {
                continue;
            }
            var count = 0;
            var smallest = 0;
            for d in 10 {
                let v = n + (d - original) * amount;
                if v >= ipow(10, len - 1) && v < 1000000 && primes[v] {
                    if count == 0 {
                        smallest = v;
                    }
                    count++;
                }
            }
            if count == 8 {
                return smallest;
            }
        }
    }
    abort("no family");
}

fn main() {
    print(solve());
}
