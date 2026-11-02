//! Enumerate square pairs x+y and x-y, then test the remaining four constraints.
use project_euler::extra_helpers::is_square;
fn solve() -> u64 {
    let mut best = u64::MAX;
    let mut ar = 2;
    while ar * ar < best {
        let a = ar * ar;
        for br in ((if ar % 2 == 1 { 1 } else { 2 })..ar).step_by(2) {
            let b = br * br;
            let (x, y) = ((a + b) / 2, (a - b) / 2);
            if x + y >= best {
                continue;
            }
            for dr in 1.. {
                let d = dr * dr;
                if d >= x {
                    break;
                }
                let c = 2 * x - d;
                if is_square(c) {
                    let z = x - d;
                    if z > 0 && y > z && is_square(y + z) && is_square(y - z) {
                        best = best.min(x + y + z)
                    }
                }
            }
        }
        ar += 1
    }
    best
}
fn main() {
    println!("{}", solve());
}
