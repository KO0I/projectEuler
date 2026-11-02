//! Project Euler 3: Largest Prime Factor.

fn solve() -> i64 {
    {
        let mut n = 600_851_475_143u64;
        let mut d = 2u64;
        let mut last = 1u64;
        while d <= n / d {
            if n % d == 0 {
                last = d;
                n /= d;
            } else {
                d += if d == 2 { 1 } else { 2 };
            }
        }
        (if n > 1 { n } else { last }) as i64
    }
}

fn main() {
    println!("{}", solve());
}
