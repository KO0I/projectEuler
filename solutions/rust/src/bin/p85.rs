//! Project Euler 85: Counting Rectangles.

fn solve() -> i64 {
    {
        let mut closest = u64::MAX;
        let mut best = 0;
        for w in 1..=2000u64 {
            for h in 1..=w {
                let number = w * (w + 1) * h * (h + 1) / 4;
                let diff = number.abs_diff(2_000_000);
                if diff < closest {
                    closest = diff;
                    best = w * h;
                }
                if number > 2_100_000 {
                    break;
                }
            }
        }
        best as i64
    }
}

fn main() {
    println!("{}", solve());
}
