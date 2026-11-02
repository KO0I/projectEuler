//! Project Euler 32: Pandigital Products.
use project_euler::extra_helpers::pandigital_1_to_9;

fn solve() -> i64 {
    {
        let mut found = vec![false; 10000];
        let mut sum = 0;
        for a in 1..100u64 {
            for b in a + 1..=9999 / a {
                let product = a * b;
                let text = format!("{a}{b}{product}");
                if text.len() == 9
                    && pandigital_1_to_9(text.parse().unwrap())
                    && !found[product as usize]
                {
                    found[product as usize] = true;
                    sum += product;
                }
            }
        }
        sum as i64
    }
}

fn main() {
    println!("{}", solve());
}
