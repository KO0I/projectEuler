//! Iterative-deepening search of star addition chains.
fn reaches(target: usize, depth: usize, limit: usize, chain: &mut [usize; 16]) -> bool {
    let top = chain[depth];
    if depth == limit {
        return top == target;
    }
    if top << (limit - depth) < target {
        return false;
    }
    let mut seen = [false; 201];
    for i in (0..=depth).rev() {
        let next = top + chain[i];
        if next <= target && !seen[next] {
            seen[next] = true;
            chain[depth + 1] = next;
            if reaches(target, depth + 1, limit, chain) {
                return true;
            }
        }
    }
    false
}
fn minimum(target: usize) -> usize {
    let mut chain = [0; 16];
    chain[0] = 1;
    (0..).find(|&d| reaches(target, 0, d, &mut chain)).unwrap()
}
fn solve() -> usize {
    (1..=200).map(minimum).sum()
}
fn main() {
    println!("{}", solve());
}
