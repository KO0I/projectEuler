//! Project Euler 17: Number Letter Counts.

fn letters(mut n: usize) -> u32 {
    let small = [0, 3, 3, 5, 4, 4, 3, 5, 5, 4, 3, 6, 6, 8, 8, 7, 7, 9, 8, 8];
    let tens = [0, 0, 6, 6, 5, 5, 5, 7, 6, 6];
    if n == 1000 {
        return 11;
    }
    let mut count = 0;
    if n >= 100 {
        count += small[n / 100] + 7;
        n %= 100;
        if n != 0 {
            count += 3;
        }
    }
    if n >= 20 {
        count += tens[n / 10];
        n %= 10;
    }
    if n > 0 {
        count += small[n];
    }
    count
}

fn solve() -> i64 {
    (1usize..=1000).map(letters).map(i64::from).sum()
}

fn main() {
    println!("{}", solve());
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn numeral_edges() {
        assert_eq!(letters(342), 23);
        assert_eq!(letters(115), 20);
        assert_eq!(letters(1000), 11);
    }
}
