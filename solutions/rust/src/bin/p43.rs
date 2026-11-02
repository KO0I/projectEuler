//! Project Euler 43: Sub-string Divisibility.

fn solve() -> i64 {
    {
        fn search(at: usize, used: u16, n: u64) -> u64 {
            const DIV: [u64; 7] = [2, 3, 5, 7, 11, 13, 17];
            if at == 10 {
                return n;
            }
            let mut total = 0;
            for d in (if at == 0 { 1 } else { 0 })..10 {
                if used & (1 << d) != 0 {
                    continue;
                }
                let next = n * 10 + d;
                if at >= 3 && next % 1000 % DIV[at - 3] != 0 {
                    continue;
                }
                total += search(at + 1, used | (1 << d), next);
            }
            total
        }
        search(0, 0, 0) as i64
    }
}

fn main() {
    println!("{}", solve());
}
