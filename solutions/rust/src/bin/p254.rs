// Project Euler 254: sums of digit factorials.
// Search k=f(n), not enormous n.  Factorial-number-system remainders encode
// the lexicographically smallest lower digits; only 37 possible counts of
// digit nine can improve the representation of the smallest k.
#[derive(Clone)]
struct R {
    r: u32,
    digit_sum: u16,
    length: u8,
    count: [u8; 8],
}
fn decode(r: u32) -> R {
    let facts = [1, 1, 2, 6, 24, 120, 720, 5040, 40320];
    let mut value = r;
    let (mut length, mut sum, mut count) = (0, 0, [0; 8]);
    for d in (1..=8).rev() {
        let c = (value / facts[d]) as u8;
        value %= facts[d];
        count[d - 1] = c;
        length += c;
        sum += d as u16 * c as u16;
    }
    R {
        r,
        digit_sum: sum,
        length,
        count,
    }
}
fn digit_sum(mut n: u64) -> u32 {
    let mut s = 0;
    while n > 0 {
        s += (n % 10) as u32;
        n /= 10;
    }
    s
}
fn smallest(sum: u32) -> u64 {
    let mut nines = sum / 9;
    let first = sum % 9;
    let mut v = first as u64;
    if first == 0 {
        v = 9;
        nines -= 1;
    }
    for _ in 0..nines {
        v = v * 10 + 9;
    }
    v
}
fn better(q: u64, r: &R, bq: u64, br: &R) -> bool {
    let l = q + r.length as u64;
    let bl = bq + br.length as u64;
    if l != bl {
        return l < bl;
    }
    for d in 0..8 {
        if r.count[d] != br.count[d] {
            return r.count[d] > br.count[d];
        }
    }
    q > bq
}
fn solve() -> u64 {
    const F: u64 = 362_880;
    let mut buckets: Vec<Vec<R>> = (0..9)
        .map(|_| Vec::with_capacity((F / 9) as usize))
        .collect();
    for r in 0..F as u32 {
        buckets[(r % 9) as usize].push(decode(r));
    }
    for b in &mut buckets {
        b.sort_unstable_by(|a, b| {
            a.length.cmp(&b.length).then_with(|| {
                for d in 0..8 {
                    if a.count[d] != b.count[d] {
                        return b.count[d].cmp(&a.count[d]);
                    }
                }
                std::cmp::Ordering::Equal
            })
        });
    }
    let mut total = 0;
    for target in 1..=150 {
        let k = smallest(target);
        let qmin = k / F;
        let mut bq = qmin;
        let mut br = decode((k % F) as u32);
        let residue = (target % 9) as usize;
        for q in qmin..=qmin + 36 {
            if q >= bq + br.length as u64 {
                break;
            }
            let base = q * F;
            if let Some(r) = buckets[residue]
                .iter()
                .find(|r| digit_sum(base + r.r as u64) == target)
            {
                if better(q, r, bq, &br) {
                    bq = q;
                    br = r.clone();
                }
            }
        }
        total += br.digit_sum as u64 + 9 * bq;
    }
    total
}
fn main() {
    println!("{}", solve());
}
