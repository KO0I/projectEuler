//! Project Euler 95: Amicable Chains.
use project_euler::extra_helpers::proper_divisor_sums;

fn solve() -> i64 {
    const LIMIT: usize = 1_000_000;
    let sums = proper_divisor_sums(LIMIT);
    let mut visited = vec![0usize; LIMIT + 1];
    let mut depths = vec![0usize; LIMIT + 1];
    let mut best = (0, usize::MAX);
    for start in 2..=LIMIT {
        if visited[start] != 0 {
            continue;
        }
        let mut n = start;
        let mut step = 0;
        while n > 0 && n <= LIMIT && visited[n] == 0 {
            visited[n] = start;
            depths[n] = step;
            step += 1;
            n = sums[n];
        }
        if n == 0 || n > LIMIT || visited[n] != start {
            continue;
        }
        let length = step - depths[n];
        let mut minimum = n;
        let mut next = sums[n];
        while next != n {
            minimum = minimum.min(next);
            next = sums[next];
        }
        if length > best.0 || length == best.0 && minimum < best.1 {
            best = (length, minimum);
        }
    }
    best.1 as i64
}

fn main() {
    println!("{}", solve());
}
