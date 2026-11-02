//! Project Euler 49: Prime Permutations.
use project_euler::extra_helpers::{decimal_signature, sieve};

fn solve() -> i64 {
    {
        let p = sieve(9999);
        let mut answer = 0;
        'outer: for a in 1000..10000usize {
            if !p[a] {
                continue;
            }
            for b in a + 1..10000 {
                let c = 2 * b - a;
                if c >= 10000 {
                    break;
                }
                if a == 1487 && b == 4817 {
                    continue;
                }
                if p[b]
                    && p[c]
                    && decimal_signature(a as u64) == decimal_signature(b as u64)
                    && decimal_signature(a as u64) == decimal_signature(c as u64)
                {
                    answer = (a as i64) * 100_000_000 + (b as i64) * 10000 + c as i64;
                    break 'outer;
                }
            }
        }
        answer
    }
}

fn main() {
    println!("{}", solve());
}
