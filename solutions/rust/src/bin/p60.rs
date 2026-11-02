//! Project Euler 60: Prime Pair Sets.
use project_euler::extra_helpers::{prime64, primes};

fn p60_concat(a: u64, b: u64) -> u64 {
    let mut power = 10;
    while power <= b {
        power *= 10;
    }
    a * power + b
}

struct PrimeGraph {
    primes: Vec<u64>,
    edges: Vec<Vec<bool>>,
}
impl PrimeGraph {
    fn new(limit: usize, best: u64) -> Self {
        let primes: Vec<u64> = primes(limit)
            .into_iter()
            .filter(|&v| v != 2 && v != 5)
            .collect();
        let n = primes.len();
        let mut edges = vec![vec![false; n]; n];
        for i in 0..n {
            for j in i + 1..n {
                let (a, b) = (primes[i], primes[j]);
                if a + b >= best || (a + b) % 3 == 0 {
                    continue;
                }
                if prime64(p60_concat(a, b)) && prime64(p60_concat(b, a)) {
                    edges[i][j] = true;
                    edges[j][i] = true;
                }
            }
        }
        Self { primes, edges }
    }
    fn search(&self, depth: usize, sum: u64, candidates: &[usize], best: &mut u64) {
        let needed = 5 - depth;
        if candidates.len() < needed {
            return;
        }
        let lower = sum
            + candidates[..needed]
                .iter()
                .map(|&i| self.primes[i])
                .sum::<u64>();
        if lower >= *best {
            return;
        }
        if needed == 1 {
            *best = lower;
            return;
        }
        for i in 0..=candidates.len() - needed {
            let bound = sum
                + candidates[i..i + needed]
                    .iter()
                    .map(|&j| self.primes[j])
                    .sum::<u64>();
            if bound >= *best {
                break;
            }
            let head = candidates[i];
            let rest: Vec<usize> = candidates[i + 1..]
                .iter()
                .copied()
                .filter(|&j| self.edges[head][j])
                .collect();
            self.search(depth + 1, sum + self.primes[head], &rest, best);
        }
    }
    fn minimum(&self, best: &mut u64) {
        let candidates: Vec<usize> = (0..self.primes.len()).collect();
        self.search(0, 0, &candidates, best);
    }
}

fn solve() -> i64 {
    let mut best = u64::MAX;
    let mut limit = 1000usize;
    loop {
        PrimeGraph::new(limit, best).minimum(&mut best);
        if best != u64::MAX {
            break;
        }
        limit *= 2;
    }
    PrimeGraph::new(best as usize - 1, best).minimum(&mut best);
    best as i64
}

fn main() {
    println!("{}", solve());
}
