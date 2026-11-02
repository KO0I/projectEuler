// Project Euler 232: The Race.
// Descending score DP solves the zero-score self-loop algebraically for each
// of player two's eight useful wager choices.
fn solve() -> f64 {
    let (mut p1, mut p2) = (vec![vec![0f64; 228]; 100], vec![vec![0f64; 228]; 101]);
    for a in 0..100 {
        for b in 100..228 {
            p1[a][b] = 1.0;
            p2[a][b] = 1.0;
        }
    }
    for a in (0..100).rev() {
        for b in (0..100).rev() {
            let head = p2[a + 1][b];
            let (mut best, mut gain, mut probability) = (0.0, 1, 0.5);
            for _ in 1..=8 {
                let failure = 1.0 - probability;
                let value =
                    (probability * p1[a][b + gain] + 0.5 * failure * head) / (1.0 - 0.5 * failure);
                if value > best {
                    best = value;
                }
                gain *= 2;
                probability *= 0.5;
            }
            p2[a][b] = best;
            p1[a][b] = 0.5 * (head + best);
        }
    }
    p1[0][0]
}
fn main() {
    println!("{:.8}", solve());
}
