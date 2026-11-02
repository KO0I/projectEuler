// Project Euler 246: tangents to an ellipse.
// Exact i128 inequalities test exterior points and tangent angle.  For each x,
// two binary searches find the contiguous qualifying y interval.
fn qualifies(x: i64, y: i64) -> bool {
    let (xx, yy) = (x as i128 * x as i128, y as i128 * y as i128);
    let (a2, b2) = (56_250_000i128, 31_250_000i128);
    if b2 * xx + a2 * yy <= a2 * b2 {
        return false;
    }
    let radius = xx + yy;
    if radius <= a2 + b2 {
        return true;
    }
    radius * radius - 300_000_000i128 * xx - 400_000_000i128 * yy + 14_687_500_000_000_000i128 < 0
}
fn solve() -> u64 {
    let mut count = 0;
    for x in -20_000i64..=20_000 {
        let v = 200_000_000 - x * x;
        let mut middle = if v > 0 { (v as f64).sqrt() as i64 } else { 0 };
        if !qualifies(x, middle) && !qualifies(x, middle + 1) {
            continue;
        }
        if !qualifies(x, middle) {
            middle += 1;
        }
        let (mut low, mut high) = (0, middle);
        while low < high {
            let m = (low + high) / 2;
            if qualifies(x, m) {
                high = m
            } else {
                low = m + 1
            }
        }
        let first = low;
        (low, high) = (middle, 25_000);
        while low < high {
            let m = (low + high + 1) / 2;
            if qualifies(x, m) {
                low = m
            } else {
                high = m - 1
            }
        }
        count += if first == 0 {
            2 * low + 1
        } else {
            2 * (low - first + 1)
        };
    }
    count as u64
}
fn main() {
    println!("{}", solve());
}
