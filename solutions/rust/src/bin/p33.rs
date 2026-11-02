//! Project Euler 33: Digit Cancelling Fractions.
use project_euler::extra_helpers::gcd;

fn solve() -> i64 {
    {
        let mut numerator = 1u64;
        let mut denominator = 1u64;
        for a in 10u64..100 {
            for b in a + 1..100 {
                let ad = [a / 10, a % 10];
                let bd = [b / 10, b % 10];
                let mut valid = false;
                for i in 0..2 {
                    for j in 0..2 {
                        if ad[i] > 0
                            && ad[i] == bd[j]
                            && bd[1 - j] > 0
                            && a * bd[1 - j] == b * ad[1 - i]
                        {
                            valid = true;
                        }
                    }
                }
                if valid {
                    numerator *= a;
                    denominator *= b;
                }
            }
        }
        (denominator / gcd(numerator, denominator)) as i64
    }
}

fn main() {
    println!("{}", solve());
}
