//! Project Euler 88: Product-sum Numbers.
use std::collections::HashSet;

fn solve() -> i64 {
    fn enumerate(product: usize, sum: usize, count: usize, first: usize, min: &mut [usize]) {
        let k = product - sum + count;
        if k >= min.len() {
            return;
        }
        if k >= 2 {
            min[k] = min[k].min(product);
        }
        for factor in first..=2 * (min.len() - 1) / product {
            enumerate(product * factor, sum + factor, count + 1, factor, min);
        }
    }
    let limit = 12000usize;
    let mut min = vec![2 * limit; limit + 1];
    enumerate(1, 0, 0, 2, &mut min);
    min[2..]
        .iter()
        .copied()
        .collect::<HashSet<_>>()
        .into_iter()
        .sum::<usize>() as i64
}

fn main() {
    println!("{}", solve());
}
