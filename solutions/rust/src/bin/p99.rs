//! Project Euler 99: Largest Exponential.
use num_bigint::BigUint;
use num_traits::{One, Zero};

fn logarithm_interval(num: u64, den: u64, scale: &BigUint) -> (BigUint, BigUint) {
    assert!(num < den && 3 * num <= den);
    let (numerator, denominator) = (BigUint::from(num), BigUint::from(den));
    let nsq = &numerator * &numerator;
    let dsq = &denominator * &denominator;
    let mut power = numerator.clone();
    let mut power_den = denominator.clone();
    let mut lower = BigUint::zero();
    const TERMS: usize = 24;
    for j in 0..TERMS {
        lower += ((&power * scale) << 1usize) / (&power_den * BigUint::from((2 * j + 1) as u64));
        power *= &nsq;
        power_den *= &dsq;
    }
    let divisor = &power_den * BigUint::from((2 * TERMS + 1) as u64) * (&dsq - &nsq);
    let remainder = (((&power * scale * &dsq) << 1usize) + &divisor - BigUint::one()) / &divisor;
    let upper = &lower + BigUint::from(TERMS as u64) + remainder;
    (lower, upper)
}

fn solve() -> i64 {
    let lines: Vec<(u64, u32)> = include_str!("../../../../tests/data/p099_base_exp.txt")
        .lines()
        .map(|line| {
            let (base, exponent) = line.split_once(',').expect("base,exponent");
            (base.parse().unwrap(), exponent.parse().unwrap())
        })
        .collect();
    assert_eq!(lines.len(), 1000);
    let scale = BigUint::one() << 192usize;
    let (log2_low, log2_high) = logarithm_interval(1, 3, &scale);
    let mut best = (0usize, BigUint::zero(), BigUint::zero());
    for (index, &(base, exponent)) in lines.iter().enumerate() {
        let power2 = 1u64 << (63 - base.leading_zeros());
        let (mut low, mut high) = logarithm_interval(base - power2, base + power2, &scale);
        let k = power2.trailing_zeros() as u64;
        low += &log2_low * k;
        high += &log2_high * k;
        low *= exponent;
        high *= exponent;
        let larger = if best.0 == 0 {
            true
        } else if low > best.2 {
            true
        } else if high < best.1 {
            false
        } else {
            let (prev_base, prev_exp) = lines[best.0 - 1];
            BigUint::from(base).pow(exponent) > BigUint::from(prev_base).pow(prev_exp)
        };
        if larger {
            best = (index + 1, low, high);
        }
    }
    best.0 as i64
}

fn main() {
    println!("{}", solve());
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn log_enclosure() {
        let scale = BigUint::one() << 192usize;
        let (low, high) = logarithm_interval(1, 3, &scale);
        assert!(low < high);
    }
}
