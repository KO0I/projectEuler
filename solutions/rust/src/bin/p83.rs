//! Project Euler 83: Path Sum: Four Ways.
use project_euler::extra_helpers::matrix;

fn solve() -> i64 {
    use std::cmp::Reverse;
    use std::collections::BinaryHeap;
    let input = matrix(include_str!("../../../../tests/data/p083_matrix.txt"));
    let side = input.len();
    let mut cost = vec![u64::MAX; side * side];
    cost[0] = input[0][0];
    let mut queue = BinaryHeap::new();
    queue.push((Reverse(cost[0]), 0usize));
    while let Some((Reverse(d), u)) = queue.pop() {
        if d != cost[u] {
            continue;
        }
        if u == side * side - 1 {
            return d as i64;
        }
        let (r, c) = (u / side, u % side);
        let neighbours = [
            (r.checked_sub(1), Some(c)),
            (Some(r + 1).filter(|&v| v < side), Some(c)),
            (Some(r), c.checked_sub(1)),
            (Some(r), Some(c + 1).filter(|&v| v < side)),
        ];
        for (nr, nc) in neighbours {
            if let (Some(nr), Some(nc)) = (nr, nc) {
                let v = nr * side + nc;
                let nd = d + input[nr][nc];
                if nd < cost[v] {
                    cost[v] = nd;
                    queue.push((Reverse(nd), v));
                }
            }
        }
    }
    panic!("unreachable matrix destination")
}

fn main() {
    println!("{}", solve());
}
