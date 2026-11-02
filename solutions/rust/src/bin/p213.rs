// Project Euler 213: flea circus.
// Distribute each independent flea for 50 steps.  For every target accumulate
// log(1-p); exponentiating gives its empty probability without underflow.
fn degree(r: usize, c: usize) -> f64 {
    (4 - (r == 0) as usize - (r == 29) as usize - (c == 0) as usize - (c == 29) as usize) as f64
}
fn solve() -> f64 {
    let mut logs = vec![0f64; 900];
    for start in 0..900 {
        let mut cur = vec![0f64; 900];
        cur[start] = 1.0;
        for _ in 0..50 {
            let mut next = vec![0f64; 900];
            for r in 0..30 {
                for c in 0..30 {
                    let i = r * 30 + c;
                    let q = cur[i] / degree(r, c);
                    if r > 0 {
                        next[i - 30] += q;
                    }
                    if r < 29 {
                        next[i + 30] += q;
                    }
                    if c > 0 {
                        next[i - 1] += q;
                    }
                    if c < 29 {
                        next[i + 1] += q;
                    }
                }
            }
            cur = next;
        }
        for i in 0..900 {
            logs[i] += (-cur[i]).ln_1p();
        }
    }
    logs.into_iter().map(f64::exp).sum()
}
fn main() {
    println!("{:.6}", solve());
}
