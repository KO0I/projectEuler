//! Seven free digits and two later choices determine the remaining 4x4 grid.
fn digit(x: i32) -> bool {
    (0..=9).contains(&x)
}
fn solve() -> u64 {
    let mut answer = 0;
    for a in 0..=9 {
        for b in 0..=9 {
            for c in 0..=9 {
                for d in 0..=9 {
                    let sum = a + b + c + d;
                    for e in 0..=9 {
                        for f in 0..=9 {
                            for g in 0..=9 {
                                let h = sum - e - f - g;
                                if !digit(h) {
                                    continue;
                                }
                                for j in 0..=9 {
                                    let i = d + g + j - a - e;
                                    if !digit(i) {
                                        continue;
                                    }
                                    for k in 0..=9 {
                                        let l = sum - i - j - k;
                                        let m = sum - a - e - i;
                                        let n = sum - b - f - j;
                                        let o = sum - c - g - k;
                                        let p = sum - d - h - l;
                                        if [l, m, n, o, p].into_iter().all(digit)
                                            && a + f + k + p == sum
                                        {
                                            answer += 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    answer
}
fn main() {
    println!("{}", solve());
}
