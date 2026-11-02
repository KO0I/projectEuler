//! Three-row profile DP anchored at the first empty cell.
use std::collections::HashMap;
const W: u32 = 9;
fn go(rows: u32, a: u32, b: u32, c: u32, m: &mut HashMap<(u32, u32, u32, u32), u64>) -> u64 {
    if rows == 0 {
        return 1;
    }
    let full = (1 << W) - 1;
    if a == full {
        return go(rows - 1, b, c, 0, m);
    }
    if let Some(&v) = m.get(&(rows, a, b, c)) {
        return v;
    }
    let col = a.trailing_ones();
    let x = 1 << col;
    let mut r = 0;
    if rows >= 2 && col + 1 < W {
        let y = 1 << (col + 1);
        if a & (x | y) == 0 && b & x == 0 {
            r += go(rows, a | x | y, b | x, c, m)
        }
        if a & (x | y) == 0 && b & y == 0 {
            r += go(rows, a | x | y, b | y, c, m)
        }
        if a & x == 0 && b & (x | y) == 0 {
            r += go(rows, a | x, b | x | y, c, m)
        }
    }
    if rows >= 2 && col > 0 {
        let l = 1 << (col - 1);
        if a & x == 0 && b & (l | x) == 0 {
            r += go(rows, a | x, b | l | x, c, m)
        }
    }
    if rows >= 3 && a & x == 0 && b & x == 0 && c & x == 0 {
        r += go(rows, a | x, b | x, c | x, m)
    }
    if col + 2 < W {
        let h = x | (1 << (col + 1)) | (1 << (col + 2));
        if a & h == 0 {
            r += go(rows, a | h, b, c, m)
        }
    }
    m.insert((rows, a, b, c), r);
    r
}
fn solve() -> u64 {
    go(12, 0, 0, 0, &mut HashMap::new())
}
fn main() {
    println!("{}", solve());
}
