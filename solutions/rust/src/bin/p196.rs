// Project Euler 196: prime triplets in two very large triangular rows.
// A segmented sieve covers only rows n-2..n+2.  Membership requires either
// two prime neighbours, or a neighbouring prime that itself has two.
fn row_start(r: i64) -> u64 {
    r as u64 * (r - 1) as u64 / 2 + 1
}
fn row_end(r: i64) -> u64 {
    r as u64 * (r + 1) as u64 / 2
}
struct Strip {
    low: u64,
    high: u64,
    prime: Vec<bool>,
}
impl Strip {
    fn at(&self, r: i64, c: i64) -> bool {
        if r < 1 || c < 0 || c >= r {
            return false;
        }
        let v = row_start(r) + c as u64;
        v >= self.low && v <= self.high && self.prime[(v - self.low) as usize]
    }
    fn neighbours(&self, r: i64, c: i64) -> u8 {
        let mut n = self.at(r, c - 1) as u8 + self.at(r, c + 1) as u8;
        for d in -1..=1 {
            n += self.at(r - 1, c + d) as u8 + self.at(r + 1, c + d) as u8;
        }
        n
    }
    fn member(&self, r: i64, c: i64) -> bool {
        if self.neighbours(r, c) >= 2 {
            return true;
        }
        [
            (0, -1),
            (0, 1),
            (-1, -1),
            (-1, 0),
            (-1, 1),
            (1, -1),
            (1, 0),
            (1, 1),
        ]
        .iter()
        .any(|&(dr, dc)| self.at(r + dr, c + dc) && self.neighbours(r + dr, c + dc) >= 2)
    }
}
fn row_sum(row: i64, base: &[usize]) -> u64 {
    let low = row_start(row - 2);
    let high = row_end(row + 2);
    let mut prime = vec![true; (high - low + 1) as usize];
    for &p0 in base {
        let p = p0 as u64;
        if p * p > high {
            break;
        }
        let mut first = low.div_ceil(p) * p;
        first = first.max(p * p);
        for v in (first..=high).step_by(p as usize) {
            prime[(v - low) as usize] = false;
        }
    }
    let s = Strip { low, high, prime };
    let first = row_start(row);
    (0..row)
        .filter(|&c| s.at(row, c) && s.member(row, c))
        .map(|c| first + c as u64)
        .sum()
}
fn solve() -> u64 {
    let max = row_end(7_208_785 + 2);
    let limit = (max as f64).sqrt().ceil() as usize;
    let mut comp = vec![false; limit + 1];
    let mut base = Vec::new();
    for n in 2..=limit {
        if !comp[n] {
            base.push(n);
            if n <= limit / n {
                for m in (n * n..=limit).step_by(n) {
                    comp[m] = true;
                }
            }
        }
    }
    row_sum(5_678_027, &base) + row_sum(7_208_785, &base)
}
fn main() {
    println!("{}", solve());
}
