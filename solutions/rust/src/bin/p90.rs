//! Project Euler 90: Cube Digit Pairs.

fn solve() -> i64 {
    let masks: Vec<u32> = (0..1u32 << 10).filter(|m| m.count_ones() == 6).collect();
    let mut count = 0;
    for (i, &a) in masks.iter().enumerate() {
        for &b in &masks[i..] {
            let rev = (1 << 6) | (1 << 9);
            let a = if a & rev != 0 { a | rev } else { a };
            let b = if b & rev != 0 { b | rev } else { b };
            if [1, 4, 9, 16, 25, 36, 49, 64, 81].iter().all(|&s| {
                let (x, y) = (1 << (s / 10), 1 << (s % 10));
                ((a & x != 0) && (b & y != 0)) || ((a & y != 0) && (b & x != 0))
            }) {
                count += 1;
            }
        }
    }
    count
}

fn main() {
    println!("{}", solve());
}
