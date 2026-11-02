//! Project Euler 2: Even Fibonacci Numbers.

fn solve() -> i64 {
    {
        let (mut a, mut b, mut sum) = (1i64, 2i64, 0i64);
        while b <= 4_000_000 {
            if b % 2 == 0 {
                sum += b;
            }
            (a, b) = (b, a + b);
        }
        sum
    }
}

fn main() {
    println!("{}", solve());
}
