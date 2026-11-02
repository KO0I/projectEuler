//! Count upright rectangles by formula and tilted rectangles on doubled coordinates.
fn upright(w: u64, h: u64) -> u64 {
    w * (w + 1) * h * (h + 1) / 4
}
fn diagonal(width: usize, height: usize) -> u64 {
    let (mut a, mut b) = (width, height);
    if a < b {
        (a, b) = (b, a)
    }
    let mut count = 0;
    for i in 0..a {
        for j in 0..b {
            for parity in 0..2 {
                let (sx, sy) = (2 * i + 1 + parity, 2 * j + 2 - parity);
                let mut stop = false;
                let mut maxh = usize::MAX;
                for rw in 0.. {
                    if stop || rw >= sy {
                        break;
                    }
                    let (x, y) = (sx + rw, sy - rw);
                    for rh in 0..maxh {
                        if x + rh >= 2 * a || y + rh >= 2 * b {
                            maxh = maxh.min(rh);
                            stop = rh == 0;
                            break;
                        }
                        count += 1
                    }
                }
            }
        }
    }
    count
}
fn solve() -> u64 {
    let mut total = 0;
    for w in 1..=47 {
        for h in 1..=43 {
            total += upright(w, h) + diagonal(w as usize, h as usize)
        }
    }
    total
}
fn main() {
    println!("{}", solve());
}
