//! Project Euler 107: Minimal Network.

fn solve() -> i64 {
    let input: Vec<Vec<u64>> = include_str!("../../../../tests/data/p107_network.txt")
        .lines()
        .map(|line| {
            line.split(',')
                .map(|v| if v == "-" { 0 } else { v.parse().unwrap() })
                .collect()
        })
        .collect();
    let n = input.len();
    assert_eq!(n, 40);
    let mut total = 0;
    for i in 0..n {
        for j in i + 1..n {
            total += input[i][j];
            assert_eq!(input[i][j], input[j][i]);
        }
    }
    let mut in_tree = vec![false; n];
    let mut best = vec![u64::MAX; n];
    best[0] = 0;
    let mut weight = 0;
    for _ in 0..n {
        let u = (0..n)
            .filter(|&i| !in_tree[i])
            .min_by_key(|&i| best[i])
            .unwrap();
        assert!(best[u] != u64::MAX);
        weight += best[u];
        in_tree[u] = true;
        for v in 0..n {
            if !in_tree[v] && input[u][v] > 0 {
                best[v] = best[v].min(input[u][v]);
            }
        }
    }
    (total - weight) as i64
}

fn main() {
    println!("{}", solve());
}
