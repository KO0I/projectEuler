//! Project Euler 100: Arranged Probability.

fn solve() -> i64 {
    {
        let (mut blue, mut total) = (15i64, 21i64);
        while total <= 1_000_000_000_000 {
            let next_blue = 3 * blue + 2 * total - 2;
            let next_total = 4 * blue + 3 * total - 3;
            blue = next_blue;
            total = next_total;
        }
        blue
    }
}

fn main() {
    println!("{}", solve());
}
