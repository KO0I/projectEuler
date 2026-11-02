//! Project Euler 11: Largest Product in a Grid.

fn solve() -> i64 {
    {
        let v: Vec<i64> = include_str!("../../../../tests/data/p011.txt")
            .split_whitespace()
            .map(|v| v.parse().unwrap())
            .collect();
        assert_eq!(v.len(), 400);
        let dirs = [(1, 0), (0, 1), (1, 1), (-1, 1)];
        let mut best = 0;
        for y in 0..20i32 {
            for x in 0..20i32 {
                for (dx, dy) in dirs {
                    let xx = x + 3 * dx;
                    let yy = y + 3 * dy;
                    if xx < 0 || xx >= 20 || yy < 0 || yy >= 20 {
                        continue;
                    }
                    let mut product = 1;
                    for step in 0..4i32 {
                        let j = ((y + step * dy) * 20 + x + step * dx) as usize;
                        product *= v[j];
                    }
                    if product > best {
                        best = product;
                    }
                }
            }
        }
        best
    }
}

fn main() {
    println!("{}", solve());
}
