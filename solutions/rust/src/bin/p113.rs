//! Project Euler 113: Non-bouncy Numbers.

fn solve() -> i64 {
    {
        let mut up = [0u64; 10];
        let mut down = [0u64; 10];
        for d in 1..=9 {
            up[d] = 1;
            down[d] = 1;
        }
        let mut sum = 0u64;
        for _ in 1..=100 {
            sum += up.iter().sum::<u64>() + down.iter().sum::<u64>() - 9;
            let mut next_up = [0u64; 10];
            let mut next_down = [0u64; 10];
            for old in 0..=9 {
                for d in old..=9 {
                    next_up[d] += up[old];
                }
                for d in 0..=old {
                    next_down[d] += down[old];
                }
            }
            up = next_up;
            down = next_down;
        }
        sum as i64
    }
}

fn main() {
    println!("{}", solve());
}
