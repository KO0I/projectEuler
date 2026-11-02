//! Memoized expectation over counts of A2,A3,A4,A5 sheets.
use std::collections::HashMap;
fn expected(s: [u8; 4], memo: &mut HashMap<[u8; 4], f64>) -> f64 {
    let total: u8 = s.iter().sum();
    if total == 0 {
        return 0.0;
    }
    if let Some(&v) = memo.get(&s) {
        return v;
    }
    let mut result = if total == 1 && s[3] == 0 { 1.0 } else { 0.0 };
    for i in 0..4 {
        if s[i] == 0 {
            continue;
        }
        let mut n = s;
        n[i] -= 1;
        for x in &mut n[i + 1..] {
            *x += 1
        }
        result += s[i] as f64 / total as f64 * expected(n, memo)
    }
    memo.insert(s, result);
    result
}
fn solve() -> f64 {
    expected([1, 1, 1, 1], &mut HashMap::new())
}
fn main() {
    println!("{:.6}", solve());
}
