//! Project Euler 31: Coin Sums.

fn solve() -> i64 {
    {
        let mut ways = [0u64; 201];
        ways[0] = 1;
        for coin in [1, 2, 5, 10, 20, 50, 100, 200] {
            for amount in coin..=200 {
                ways[amount] += ways[amount - coin];
            }
        }
        ways[200] as i64
    }
}

fn main() {
    println!("{}", solve());
}
