//! Project Euler 82: Path Sum: Three Ways.
use project_euler::extra_helpers::matrix;

fn solve() -> i64 {
    let input = matrix(include_str!("../../../../tests/data/p082_matrix.txt"));
    let side = input.len();
    let mut cost: Vec<u64> = (0..side).map(|row| input[row][0]).collect();
    for c in 1..side {
        for r in 0..side {
            cost[r] += input[r][c];
        }
        for r in 1..side {
            cost[r] = cost[r].min(cost[r - 1] + input[r][c]);
        }
        for r in (0..side - 1).rev() {
            cost[r] = cost[r].min(cost[r + 1] + input[r][c]);
        }
    }
    *cost.iter().min().unwrap() as i64
}

fn main() {
    println!("{}", solve());
}
