//! Project Euler 109: Darts.

fn solve() -> i64 {
    {
        let mut scores = Vec::new();
        for n in 1..=20 {
            scores.push(n);
            scores.push(2 * n);
            scores.push(3 * n);
        }
        scores.extend([25, 50]);
        let mut doubles: Vec<i64> = (1..=20).map(|n| 2 * n).collect();
        doubles.push(50);
        let mut count = 0;
        for &d in &doubles {
            if d < 100 {
                count += 1;
            }
            for &a in &scores {
                if a + d < 100 {
                    count += 1;
                }
            }
            for (i, &a) in scores.iter().enumerate() {
                for &b in &scores[i..] {
                    if a + b + d < 100 {
                        count += 1;
                    }
                }
            }
        }
        count
    }
}

fn main() {
    println!("{}", solve());
}
