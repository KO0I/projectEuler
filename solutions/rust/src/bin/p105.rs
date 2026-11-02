//! Project Euler 105: Special Subset Sums: Testing.
use std::collections::HashMap;

fn solve() -> i64 {
    include_str!("../../../../tests/data/p105_sets.txt")
        .lines()
        .map(|line| {
            let mut values: Vec<u64> = line.split(',').map(|v| v.parse().unwrap()).collect();
            values.sort_unstable();
            if !special_sum_set(&values) {
                0
            } else {
                values.iter().sum::<u64>() as i64
            }
        })
        .sum()
}

fn special_sum_set(values: &[u64]) -> bool {
    // Strictly increasing sorted positive values: compare all distinct
    // cardinalities by the boundary sums, then equal-cardinality subset sums.
    for k in 1..=(values.len() / 2) {
        if values[..k + 1].iter().sum::<u64>() <= values[values.len() - k..].iter().sum::<u64>() {
            return false;
        }
    }
    let mut sums: HashMap<(usize, u64), Vec<u64>> = HashMap::new();
    for mask in 1..(1u64 << values.len()) {
        let mut sum = 0;
        let mut count = 0;
        for (i, &v) in values.iter().enumerate() {
            if mask & (1 << i) != 0 {
                count += 1;
                sum += v;
            }
        }
        let entry = sums.entry((count, sum)).or_default();
        if entry.iter().any(|prior| mask & prior == 0) {
            return false;
        }
        entry.push(mask);
    }
    true
}

fn main() {
    println!("{}", solve());
}
