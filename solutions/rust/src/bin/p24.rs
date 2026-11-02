//! Project Euler 24: Lexicographic Permutations.

fn solve() -> i64 {
    {
        let mut pool: Vec<u64> = (0..10).collect();
        let mut rank = 999999usize;
        let mut value = 0u64;
        for remaining in (1..=10).rev() {
            let factorial = (1..remaining).product::<usize>();
            let idx = rank / factorial;
            rank %= factorial;
            value = value * 10 + pool.remove(idx);
        }
        value as i64
    }
}

fn main() {
    println!("{}", solve());
}
