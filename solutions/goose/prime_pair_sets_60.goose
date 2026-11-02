// Project Euler 60: Prime Pair Sets.
import euler from "../../support/goose";

fn pair(a: i64, b: i64) -> bool {
    return prime(a * ipow(10, digits(b)) + b) && prime(b * ipow(10, digits(a)) + a);
}

fn bounded_search(limit: i64) -> i64 {
    var ps: i64[>..] = [];
    for n in 3..limit {
        if n != 5 && prime(n) {
            ps.push(n);
        }
    }
    var best = 1000000000;
    for a in ps.len {
        if ps[a] * 5 >= best {
            break;
        }
        for b in a + 1..ps.len {
            if ps[a] + ps[b] * 4 >= best {
                break;
            }
            if !pair(ps[a], ps[b]) {
                continue;
            }
            for c in b + 1..ps.len {
                if ps[a] + ps[b] + ps[c] * 3 >= best {
                    break;
                }
                if !pair(ps[a], ps[c]) || !pair(ps[b], ps[c]) {
                    continue;
                }
                for d in c + 1..ps.len {
                    if ps[a] + ps[b] + ps[c] + ps[d] * 2 >= best {
                        break;
                    }
                    if !pair(ps[a], ps[d]) || !pair(ps[b], ps[d]) || !pair(ps[c], ps[d]) {
                        continue;
                    }
                    for e in d + 1..ps.len {
                        let s = ps[a] + ps[b] + ps[c] + ps[d] + ps[e];
                        if s >= best {
                            break;
                        }
                        if pair(ps[a], ps[e]) && pair(ps[b], ps[e]) && pair(ps[c], ps[e]) && pair(ps[d], ps[e]) {
                            best = s;
                        }
                    }
                }
            }
        }
    }
    return best;
}

fn solve() -> i64 {
    // Grow until a witness exists, then cover every prime below its sum.
    // A better positive sum cannot contain a prime above that bound.
    var limit = 1000;
    loop {
        let candidate = bounded_search(limit);
        if candidate < 1000000000 {
            if candidate < limit { return candidate; }
            return bounded_search(candidate + 1);
        }
        limit *= 10;
    }
}

fn main() {
    print(solve());
}
