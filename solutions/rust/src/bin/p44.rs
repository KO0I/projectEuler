//! Project Euler 44: Pentagon Numbers.
use project_euler::extra_helpers::pentagonal;

fn solve() -> i64 {
    {
        let mut best = None;
        for j in 2u64..5000 {
            let pj = j * (3 * j - 1) / 2;
            for k in (1..j).rev() {
                let pk = k * (3 * k - 1) / 2;
                let diff = pj - pk;
                if best.is_some_and(|current| diff >= current) {
                    break;
                }
                if pentagonal(diff) && pentagonal(pj + pk) {
                    best = Some(diff);
                }
            }
            if let Some(current) = best {
                if 3 * j - 2 > current {
                    break;
                }
            }
        }
        best.unwrap_or(0) as i64
    }
}

fn main() {
    println!("{}", solve());
}
