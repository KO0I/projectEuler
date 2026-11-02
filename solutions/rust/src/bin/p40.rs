//! Project Euler 40: Champernowne's Constant.

fn solve() -> i64 {
    {
        let positions = [1, 10, 100, 1000, 10000, 100000, 1_000_000];
        let mut at = 0;
        let mut digit = 0;
        let mut product = 1i64;
        for n in 1.. {
            for c in n.to_string().bytes() {
                digit += 1;
                if digit == positions[at] {
                    product *= i64::from(c - b'0');
                    at += 1;
                    if at == positions.len() {
                        break;
                    }
                }
            }
            if at == positions.len() {
                break;
            }
        }
        product
    }
}

fn main() {
    println!("{}", solve());
}
