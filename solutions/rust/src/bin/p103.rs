//! Project Euler 103: Special Subset Sums: Optimum.

fn solve() -> i64 {
    fn cardinality_ok(values: &[u64; 7], chosen: usize) -> bool {
        for k in 1..=3 {
            if chosen < k + 1 {
                continue;
            }
            let small: usize = (0..=k).map(|i| values[i] as usize).sum();
            let large: usize = (7 - k..7)
                .map(|i| {
                    if i < chosen {
                        values[i] as usize
                    } else {
                        values[chosen - 1] as usize + i - chosen + 1
                    }
                })
                .sum();
            if small <= large {
                return false;
            }
        }
        true
    }
    fn dfs(
        chosen: usize,
        total: u64,
        current: &mut [u64; 7],
        best: &mut [u64; 7],
        best_sum: &mut u64,
        sums: &[u64],
        seen: &[bool; 512],
    ) {
        if chosen == 7 {
            *best_sum = total;
            *best = *current;
            return;
        }
        let remain = (7 - chosen) as u64;
        let first = if chosen == 0 {
            1
        } else {
            current[chosen - 1] + 1
        };
        for value in first.. {
            let minimum = total + remain * value + remain * (remain - 1) / 2;
            if minimum >= *best_sum {
                break;
            }
            current[chosen] = value;
            if !cardinality_ok(current, chosen + 1) {
                continue;
            }
            if sums.iter().any(|&s| seen[(s + value) as usize]) {
                continue;
            }
            let mut next_sums = sums.to_vec();
            let mut next_seen = *seen;
            for &sum in sums {
                next_sums.push(sum + value);
                next_seen[(sum + value) as usize] = true;
            }
            dfs(
                chosen + 1,
                total + value,
                current,
                best,
                best_sum,
                &next_sums,
                &next_seen,
            );
        }
    }
    // The size-six optimal set is given in the problem statement, and the
    // middle-element construction provides a verified feasible upper bound.
    const PRIOR: [u64; 6] = [11, 18, 19, 20, 22, 25];
    let mut best = [0u64; 7];
    best[0] = PRIOR[3];
    for i in 0..6 {
        best[i + 1] = PRIOR[3] + PRIOR[i];
    }
    let mut best_sum = best.iter().sum();
    let mut current = [0u64; 7];
    let mut seen = [false; 512];
    seen[0] = true;
    dfs(0, 0, &mut current, &mut best, &mut best_sum, &[0], &seen);
    best.iter().fold(0i64, |v, &part| {
        let magnitude = 10i64.pow(part.ilog10() + 1);
        v * magnitude + part as i64
    })
}

fn main() {
    println!("{}", solve());
}
