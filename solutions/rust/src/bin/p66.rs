//! Project Euler 66: Diophantine Equation.
use num_bigint::BigUint;
use num_traits::{One, Zero};
use project_euler::extra_helpers::isqrt;

fn solve() -> i64 {
    {
        let mut best = BigUint::zero();
        let mut answer = 0;
        for d in 2..=1000u64 {
            let a0 = isqrt(d);
            if a0 * a0 == d {
                continue;
            }
            let (mut m, mut den, mut a) = (0u64, 1u64, a0);
            let (mut p0, mut p1, mut q0, mut q1) = (
                BigUint::zero(),
                BigUint::one(),
                BigUint::one(),
                BigUint::zero(),
            );
            loop {
                let p = &p1 * a + &p0;
                let q = &q1 * a + &q0;
                if &p * &p == BigUint::one() + BigUint::from(d) * &q * &q {
                    if p > best {
                        best = p;
                        answer = d;
                    }
                    break;
                }
                p0 = p1;
                p1 = p;
                q0 = q1;
                q1 = q;
                m = den * a - m;
                den = (d - m * m) / den;
                a = (a0 + m) / den;
            }
        }
        answer as i64
    }
}

fn main() {
    println!("{}", solve());
}
