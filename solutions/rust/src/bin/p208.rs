// Project Euler 208: robot walks.
// A closed 70-arc path uses each fifth-root-of-unity direction 14 times.
// Track four direction counts plus heading; the step determines the fifth.
const SIDE: usize = 15;
const STATES: usize = SIDE * SIDE * SIDE * SIDE * 5;
fn ix(a: usize, b: usize, c: usize, d: usize, h: usize) -> usize {
    ((((a * SIDE + b) * SIDE + c) * SIDE + d) * 5) + h
}
fn solve() -> u64 {
    let mut cur = vec![0u64; STATES];
    cur[ix(0, 0, 0, 0, 0)] = 1;
    for step in 0..70 {
        let mut next = vec![0u64; STATES];
        for a in 0..=14 {
            for b in 0..=14 {
                for c in 0..=14 {
                    for d in 0..=14 {
                        let used = a + b + c + d;
                        if used > step || step - used > 14 {
                            continue;
                        }
                        for h in 0..5 {
                            let ways = cur[ix(a, b, c, d, h)];
                            if ways == 0 {
                                continue;
                            }
                            for turn in [4, 1] {
                                let nh = (h + turn) % 5;
                                let mut cs = [a, b, c, d];
                                if nh < 4 {
                                    if cs[nh] == 14 {
                                        continue;
                                    }
                                    cs[nh] += 1;
                                } else if step - used == 14 {
                                    continue;
                                }
                                next[ix(cs[0], cs[1], cs[2], cs[3], nh)] += ways;
                            }
                        }
                    }
                }
            }
        }
        cur = next;
    }
    cur[ix(14, 14, 14, 14, 0)]
}
fn main() {
    println!("{}", solve());
}
