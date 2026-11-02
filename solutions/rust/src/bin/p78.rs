//! Project Euler 78: Coin Partitions.

fn solve() -> i64 {
    {
        let modulo = 1_000_000i64;
        let mut partitions = vec![1i64];
        let mut answer = 0;
        for n in 1..100_000usize {
            let mut p = 0i64;
            let mut k = 1i64;
            loop {
                let g1 = k * (3 * k - 1) / 2;
                let g2 = k * (3 * k + 1) / 2;
                if g1 as usize > n {
                    break;
                }
                let sign = if k % 2 == 1 { 1 } else { -1 };
                p += sign * partitions[n - g1 as usize];
                if g2 as usize <= n {
                    p += sign * partitions[n - g2 as usize];
                }
                k += 1;
            }
            p = p.rem_euclid(modulo);
            partitions.push(p);
            if p == 0 {
                answer = n as i64;
                break;
            }
        }
        answer
    }
}

fn main() {
    println!("{}", solve());
}
