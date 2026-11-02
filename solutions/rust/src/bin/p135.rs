//! Factor parameterization n=u*v counts the admissible arithmetic progressions.
fn solve() -> usize {
    const L: usize = 1_000_000;
    let mut reps = vec![0u8; L];
    for u in 1..L {
        let mut v = (u + 2) / 3;
        while u * v < L {
            if u < 3 * v && (u + v) % 4 == 0 {
                reps[u * v] = (reps[u * v] + 1).min(11)
            }
            v += 1
        }
    }
    reps.into_iter().filter(|&x| x == 10).count()
}
fn main() {
    println!("{}", solve());
}
