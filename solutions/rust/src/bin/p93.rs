//! Project Euler 93: Arithmetic Expressions.
use project_euler::extra_helpers::gcd;

#[derive(Clone, Copy)]
struct Ratio {
    n: i64,
    d: i64,
}
impl Ratio {
    fn new(mut n: i64, mut d: i64) -> Self {
        assert!(d != 0);
        if d < 0 {
            n = -n;
            d = -d;
        }
        let divisor = gcd(n.unsigned_abs(), d as u64) as i64;
        Self {
            n: n / divisor,
            d: d / divisor,
        }
    }
    fn add(a: Self, b: Self) -> Self {
        Self::new(a.n * b.d + b.n * a.d, a.d * b.d)
    }
    fn sub(a: Self, b: Self) -> Self {
        Self::new(a.n * b.d - b.n * a.d, a.d * b.d)
    }
    fn mul(a: Self, b: Self) -> Self {
        Self::new(a.n * b.n, a.d * b.d)
    }
    fn div(a: Self, b: Self) -> Option<Self> {
        if b.n == 0 {
            None
        } else {
            Some(Self::new(a.n * b.d, a.d * b.n))
        }
    }
}

fn p93_score(d: [i64; 4]) -> usize {
    fn evaluate(nums: &[Ratio], reached: &mut [bool; 512]) {
        if nums.len() == 1 {
            let v = nums[0];
            if v.d == 1 && v.n > 0 && v.n < reached.len() as i64 {
                reached[v.n as usize] = true;
            }
            return;
        }
        for i in 0..nums.len() {
            for j in i + 1..nums.len() {
                let a = nums[i];
                let b = nums[j];
                let mut rest = Vec::with_capacity(nums.len() - 1);
                for (k, &v) in nums.iter().enumerate() {
                    if k != i && k != j {
                        rest.push(v);
                    }
                }
                let mut choices = vec![
                    Ratio::add(a, b),
                    Ratio::sub(a, b),
                    Ratio::sub(b, a),
                    Ratio::mul(a, b),
                ];
                if let Some(c) = Ratio::div(a, b) {
                    choices.push(c);
                }
                if let Some(c) = Ratio::div(b, a) {
                    choices.push(c);
                }
                for value in choices {
                    rest.push(value);
                    evaluate(&rest, reached);
                    rest.pop();
                }
            }
        }
    }
    let data = d.map(|v| Ratio::new(v, 1));
    let mut reached = [false; 512];
    evaluate(&data, &mut reached);
    (1..reached.len()).find(|&i| !reached[i]).unwrap_or(512) - 1
}

fn solve() -> i64 {
    let mut best = (0usize, 0i64);
    for a in 0..=6 {
        for b in a + 1..=7 {
            for c in b + 1..=8 {
                for d in c + 1..=9 {
                    let score = p93_score([a, b, c, d]);
                    if score > best.0 {
                        best = (score, 1000 * a + 100 * b + 10 * c + d);
                    }
                }
            }
        }
    }
    best.1
}

fn main() {
    println!("{}", solve());
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn rationals() {
        assert_eq!(Ratio::div(Ratio::new(3, 2), Ratio::new(9, 4)).unwrap().n, 2);
    }
}
