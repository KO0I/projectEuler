//! Project Euler 9: Special Pythagorean Triplet.

fn solve() -> i64 {
    {
        let mut answer = 0;
        for a in 1..334i64 {
            for b in a + 1..500 {
                let c = 1000 - a - b;
                if c > b && a * a + b * b == c * c {
                    answer = a * b * c;
                    break;
                }
            }
            if answer != 0 {
                break;
            }
        }
        answer
    }
}

fn main() {
    println!("{}", solve());
}
