//! Project Euler 26: Reciprocal Cycles.

fn solve() -> i64 {
    {
        let mut best = (0usize, 0i64);
        for denominator in 2usize..1000 {
            let mut seen = vec![usize::MAX; denominator];
            let mut r = 1usize % denominator;
            let mut position = 0;
            while r != 0 && seen[r] == usize::MAX {
                seen[r] = position;
                position += 1;
                r = (r * 10) % denominator;
            }
            let length = if r == 0 { 0 } else { position - seen[r] };
            if length > best.0 {
                best = (length, denominator as i64);
            }
        }
        best.1
    }
}

fn main() {
    println!("{}", solve());
}
