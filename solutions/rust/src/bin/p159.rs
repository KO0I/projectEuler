//! Multiplicative DP maximizes digital-root sums over every factor split.
fn solve() -> u64 {
    const L: usize = 1_000_000;
    let mut m = vec![0u8; L];
    for n in 1..L {
        m[n] = (1 + (n - 1) % 9) as u8
    }
    for a in 2..L {
        for b in 2.. {
            let n = a * b;
            if n >= L {
                break;
            }
            m[n] = m[n].max(m[a] + m[b])
        }
    }
    m[2..].iter().map(|&x| x as u64).sum()
}
fn main() {
    println!("{}", solve());
}
