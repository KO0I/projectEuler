// Project Euler 187: semiprimes below 10^8.
// Sieve possible larger prime factors, then use a monotonically decreasing
// upper pointer to count pairs p<=q and p*q<LIMIT.
fn solve() -> u64 {
    const LIMIT: usize = 100_000_000;
    const SIEVE: usize = LIMIT / 2;
    let mut composite = vec![false; SIEVE];
    let mut primes = Vec::with_capacity(3_100_000);
    for n in 2..SIEVE {
        if !composite[n] {
            primes.push(n);
            if n <= SIEVE / n {
                for m in (n * n..SIEVE).step_by(n) {
                    composite[m] = true;
                }
            }
        }
    }
    let mut answer = 0u64;
    let mut high = primes.len();
    for low in 0..primes.len() {
        while high > low && primes[low] as u64 * primes[high - 1] as u64 >= LIMIT as u64 {
            high -= 1;
        }
        if high <= low {
            break;
        }
        answer += (high - low) as u64;
    }
    answer
}
fn main() {
    println!("{}", solve());
}
