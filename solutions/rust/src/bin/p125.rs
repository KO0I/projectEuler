//! Enumerate prefix differences of consecutive square sums and deduplicate palindromes.
use project_euler::extra_helpers::palindrome;
use std::collections::HashSet;
fn solve() -> u64 {
    let mut pre = vec![0u64; 10_001];
    for n in 1..=10_000 {
        pre[n] = pre[n - 1] + (n * n) as u64;
    }
    let mut set = HashSet::new();
    for a in 1..10_000 {
        for b in a + 1..=10_000 {
            let s = pre[b] - pre[a - 1];
            if s >= 100_000_000 {
                break;
            }
            if palindrome(s, 10) {
                set.insert(s);
            }
        }
    }
    set.into_iter().sum()
}
fn main() {
    println!("{}", solve());
}
