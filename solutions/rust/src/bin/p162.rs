//! Inclusion-exclusion forces hexadecimal digits 0,1,A to appear.
fn solve() -> u64 {
    let mut answer = 0;
    for len in 3..=16 {
        let mut count = 0i64;
        for mask in 0u32..8 {
            let removed = mask.count_ones();
            let a = 16 - removed as u64;
            let ways = if mask & 1 != 0 {
                a.pow(len)
            } else {
                (a - 1) * a.pow(len - 1)
            };
            count += if removed % 2 == 1 {
                -(ways as i64)
            } else {
                ways as i64
            }
        }
        answer += count as u64
    }
    answer
}
fn main() {
    println!("{:X}", solve());
}
