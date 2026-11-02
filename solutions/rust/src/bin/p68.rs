//! Project Euler 68: Magic 5-gon Ring.

fn solve() -> i64 {
    fn search(inner: &mut [i64; 5], depth: usize, used: u16, best: &mut u64) {
        if depth < 5 {
            for value in 1..=9 {
                if used & (1 << value) != 0 {
                    continue;
                }
                inner[depth] = value;
                search(inner, depth + 1, used | (1 << value), best);
            }
            return;
        }
        for first in 1..=10i64 {
            if used & (1 << (first as u16)) != 0 {
                continue;
            }
            let line = first + inner[0] + inner[1];
            let mut all = used;
            let mut outer = [0i64; 5];
            let mut valid = true;
            for i in 0..5 {
                let candidate = line - inner[i] - inner[(i + 1) % 5];
                if candidate < first || candidate > 10 || all & (1 << (candidate as u16)) != 0 {
                    valid = false;
                    break;
                }
                outer[i] = candidate;
                all |= 1 << (candidate as u16);
            }
            if valid {
                let mut representation = 0;
                for i in 0..5 {
                    representation = representation * (if outer[i] == 10 { 100 } else { 10 })
                        + (outer[i] as u64);
                    representation = representation * 10 + inner[i] as u64;
                    representation = representation * 10 + inner[(i + 1) % 5] as u64;
                }
                *best = (*best).max(representation);
            }
        }
    }
    let mut best = 0;
    search(&mut [0; 5], 0, 0, &mut best);
    best as i64
}

fn main() {
    println!("{}", solve());
}
