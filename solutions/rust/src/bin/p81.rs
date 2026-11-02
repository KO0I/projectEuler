//! Project Euler 81: Path Sum: Two Ways.
use project_euler::extra_helpers::matrix;

fn solve() -> i64 {
    let input = matrix(include_str!("../../../../tests/data/p081_matrix.txt"));
    let mut cost = vec![0u64; input[0].len()];
    for (r, line) in input.iter().enumerate() {
        for (c, &value) in line.iter().enumerate() {
            if r == 0 && c > 0 {
                cost[c] = cost[c - 1];
            } else if r > 0 && c > 0 {
                cost[c] = cost[c].min(cost[c - 1]);
            }
            cost[c] += value;
        }
    }
    cost[cost.len() - 1] as i64
}

fn main() {
    println!("{}", solve());
}
