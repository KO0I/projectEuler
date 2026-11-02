// Project Euler 214: totient chains.
// Euler's sieve builds phi; increasing order then makes length(phi(n)) known.
// phi(n)=n-1 simultaneously identifies primes without another large table.
fn solve() -> u64 {
    const N: usize = 40_000_000;
    let mut phi: Vec<u32> = (0..N as u32).collect();
    for p in 2..N {
        if phi[p] == p as u32 {
            for m in (p..N).step_by(p) {
                phi[m] -= phi[m] / p as u32;
            }
        }
    }
    let mut len = vec![0u8; N];
    len[1] = 1;
    let mut sum = 0;
    for n in 2..N {
        len[n] = len[phi[n] as usize] + 1;
        if phi[n] == n as u32 - 1 && len[n] == 25 {
            sum += n as u64;
        }
    }
    sum
}
fn main() {
    println!("{}", solve());
}
