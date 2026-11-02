//! Project Euler 14: Longest Collatz Sequence.

fn solve() -> i64 {
    {
        let mut best_len = 0;
        let mut best_start = 0;
        for start in 1u64..1_000_000 {
            let (mut v, mut length) = (start, 1);
            while v != 1 {
                v = if v % 2 == 0 { v / 2 } else { 3 * v + 1 };
                length += 1;
            }
            if length > best_len {
                best_len = length;
                best_start = start;
            }
        }
        best_start as i64
    }
}

fn main() {
    println!("{}", solve());
}
