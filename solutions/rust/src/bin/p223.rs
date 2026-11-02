// Project Euler 223: almost-right triangles I.
// Factor (c-b)(c+b)=a^2-1.  Divisor bounds enforce perimeter and a<=b;
// equal parity makes b,c integral.
fn isqrt(n: u64) -> u64 {
    let mut r = (n as f64).sqrt() as u64;
    while (r + 1) <= n / (r + 1) {
        r += 1;
    }
    while r > n / r {
        r -= 1;
    }
    r
}
fn factor(mut v: usize, spf: &[u32], fs: &mut Vec<(u64, u32)>) {
    while v > 1 {
        let p = if spf[v] == 0 { v } else { spf[v] as usize };
        let mut e = 0;
        while v % p == 0 {
            v /= p;
            e += 1;
        }
        if let Some(x) = fs.iter_mut().find(|x| x.0 == p as u64) {
            x.1 += e
        } else {
            fs.push((p as u64, e));
        }
    }
}
fn count(fs: &[(u64, u32)], i: usize, d: u64, min: u64, max: u64, n: u64) -> u64 {
    if i == fs.len() {
        return (d >= min && d <= max && ((n / d) ^ d) & 1 == 0) as u64;
    }
    let (mut sum, mut power) = (0, 1);
    for _ in 0..=fs[i].1 {
        if d * power > max {
            break;
        }
        sum += count(fs, i + 1, d * power, min, max, n);
        power *= fs[i].0;
    }
    sum
}
fn solve() -> u64 {
    const P: usize = 25_000_000;
    const A: usize = P / 3;
    let mut spf = vec![0u32; A + 2];
    for p in 2..=((A + 1) as f64).sqrt() as usize {
        if spf[p] == 0 {
            for m in (p * p..=A + 1).step_by(p) {
                if spf[m] == 0 {
                    spf[m] = p as u32;
                }
            }
        }
    }
    let mut total = ((P - 1) / 2) as u64;
    for a in 2..=A {
        let n = a as u64 * a as u64 - 1;
        let min = n.div_ceil((P - a) as u64);
        let max = isqrt(2 * a as u64 * a as u64 - 1) - a as u64;
        if min > max {
            continue;
        }
        let mut fs = Vec::new();
        factor(a - 1, &spf, &mut fs);
        factor(a + 1, &spf, &mut fs);
        total += count(&fs, 0, 1, min, max, n);
    }
    total
}
fn main() {
    println!("{}", solve());
}
