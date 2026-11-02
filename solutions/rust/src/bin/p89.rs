//! Project Euler 89: Roman Numerals.

fn solve() -> i64 {
    {
        let text = include_str!("../../../../tests/data/p089_roman.txt");
        fn value(ch: u8) -> usize {
            match ch {
                b'I' => 1,
                b'V' => 5,
                b'X' => 10,
                b'L' => 50,
                b'C' => 100,
                b'D' => 500,
                b'M' => 1000,
                _ => 0,
            }
        }
        fn canonical(mut number: usize) -> String {
            let mut result = String::new();
            for (num, sym) in [
                (1000, "M"),
                (900, "CM"),
                (500, "D"),
                (400, "CD"),
                (100, "C"),
                (90, "XC"),
                (50, "L"),
                (40, "XL"),
                (10, "X"),
                (9, "IX"),
                (5, "V"),
                (4, "IV"),
                (1, "I"),
            ] {
                while number >= num {
                    result.push_str(sym);
                    number -= num;
                }
            }
            result
        }
        text.lines()
            .filter(|l| !l.is_empty())
            .map(|line| {
                let b = line.trim().as_bytes();
                let mut total = 0;
                for i in 0..b.len() {
                    let v = value(b[i]);
                    if i + 1 < b.len() && v < value(b[i + 1]) {
                        total -= v as i64;
                    } else {
                        total += v as i64;
                    }
                }
                b.len() as i64 - canonical(total as usize).len() as i64
            })
            .sum::<i64>()
    }
}

fn main() {
    println!("{}", solve());
}
