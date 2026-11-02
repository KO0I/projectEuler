//! Project Euler 59: XOR Decryption.
use project_euler::extra_helpers::parse_numbers;

fn p59_score(cipher: &[u64], key: [u8; 3]) -> Option<(i64, u64)> {
    const WEIGHTS: [i64; 26] = [
        8, 2, 3, 4, 13, 2, 2, 6, 7, 1, 1, 4, 2, 7, 8, 2, 1, 6, 6, 9, 3, 1, 2, 1, 2, 1,
    ];
    let mut score = 0;
    let mut text = Vec::with_capacity(cipher.len());
    let mut sum = 0;
    for (i, &c) in cipher.iter().enumerate() {
        let ch = (c as u8) ^ key[i % 3];
        if !(32..=126).contains(&ch) {
            return None;
        }
        sum += ch as u64;
        text.push(ch);
        score += if ch == b' ' {
            15
        } else if ch.is_ascii_alphabetic() {
            WEIGHTS[(ch.to_ascii_lowercase() - b'a') as usize]
        } else if b".,;:'\"!?()-[]".contains(&ch) {
            -1
        } else if ch.is_ascii_digit() {
            -3
        } else {
            -12
        };
    }
    let mut matches = 0;
    for word in [
        b" the ".as_slice(),
        b" and ",
        b" of ",
        b" to ",
        b" in ",
        b" that ",
        b" is ",
        b" a ",
        b" for ",
        b" with ",
    ] {
        for window in text.windows(word.len()) {
            if window == word {
                score += 100;
                matches += 1;
            }
        }
    }
    (matches >= 3).then_some((score, sum))
}

fn solve() -> i64 {
    {
        let cipher = parse_numbers(include_str!("../../../../tests/data/p059_cipher.txt"));
        let mut best = None;
        let mut tied = false;
        for a in b'a'..=b'z' {
            for b in b'a'..=b'z' {
                for c in b'a'..=b'z' {
                    if let Some((score, sum)) = p59_score(&cipher, [a, b, c]) {
                        if best.is_none_or(|(prev, _)| score > prev) {
                            best = Some((score, sum));
                            tied = false;
                        } else if best.is_some_and(|(prev, _)| score == prev) {
                            tied = true;
                        }
                    }
                }
            }
        }
        assert!(!tied, "Tied top English ciphertext key");
        best.expect("unique English plaintext").1 as i64
    }
}

fn main() {
    println!("{}", solve());
}
