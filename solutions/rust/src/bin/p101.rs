//! Project Euler 101: Optimum Polynomial.

fn solve() -> i64 {
    fn sequence(n: i128) -> i128 {
        let mut v = 1;
        for _ in 0..10 {
            v = 1 - n * v;
        }
        v
    }
    fn fit(first: &[i128], n: usize) -> i128 {
        let mut sum = 0;
        let mut choose = 1i128;
        for (j, &v) in first.iter().enumerate() {
            sum += v * choose;
            if j + 1 < first.len() {
                choose = choose * (n - 1 - j) as i128 / (j + 1) as i128;
            }
        }
        sum
    }
    let mut total = 0;
    for k in 1..=10 {
        let mut diffs: Vec<i128> = (1..=k).map(|v| sequence(v as i128)).collect();
        let mut first = Vec::new();
        for order in 0..k {
            first.push(diffs[0]);
            for i in 0..k - order - 1 {
                diffs[i] = diffs[i + 1] - diffs[i];
            }
        }
        let mut at = k + 1;
        while fit(&first, at) == sequence(at as i128) {
            at += 1;
        }
        total += fit(&first, at);
    }
    total as i64
}

fn main() {
    println!("{}", solve());
}
