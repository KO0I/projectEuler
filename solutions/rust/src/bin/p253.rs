// Project Euler 253: tidying up.
// Reverse the permutation: deleting from a canonical sorted multiset of
// segment lengths has the same maximum segment count.  Memoized weighted
// transitions combine equal segments and mirror-image cuts exactly.
use std::collections::HashMap;
fn expected(state: &[u8], best: u8, memo: &mut HashMap<(Vec<u8>, u8), f64>) -> f64 {
    if state.is_empty() {
        return best as f64;
    }
    let key = (state.to_vec(), best);
    if let Some(&v) = memo.get(&key) {
        return v;
    }
    let remaining: usize = state.iter().map(|&x| x as usize).sum();
    let mut weighted = 0.0;
    let mut first = 0;
    while first < state.len() {
        let mut after = first + 1;
        while after < state.len() && state[after] == state[first] {
            after += 1;
        }
        let copies = (after - first) as f64;
        let length = state[first];
        let mut base = state.to_vec();
        base.remove(first);
        if length == 1 {
            weighted += copies * expected(&base, best, memo);
        } else {
            let mut child = base.clone();
            child.push(length - 1);
            child.sort_unstable();
            weighted += 2.0 * copies * expected(&child, best.max(child.len() as u8), memo);
            let rest = length - 1;
            for left in 1..=rest / 2 {
                let right = rest - left;
                let mut child = base.clone();
                child.extend([left, right]);
                child.sort_unstable();
                let weight = if left == right { 1.0 } else { 2.0 };
                weighted += copies * weight * expected(&child, best.max(child.len() as u8), memo);
            }
        }
        first = after;
    }
    let result = weighted / remaining as f64;
    memo.insert(key, result);
    result
}
fn solve() -> f64 {
    expected(&[40], 1, &mut HashMap::new())
}
fn main() {
    println!("{:.6}", solve());
}
