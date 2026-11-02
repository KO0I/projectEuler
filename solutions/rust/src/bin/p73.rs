//! Project Euler 73: Counting Fractions in a Range.

fn solve() -> i64 {
    {
        fn count(a: u32, b: u32, c: u32, d: u32) -> u64 {
            if b + d > 12000 {
                return 0;
            }
            1 + count(a, b, a + c, b + d) + count(a + c, b + d, c, d)
        }
        count(1, 3, 1, 2) as i64
    }
}

fn main() {
    println!("{}", solve());
}
