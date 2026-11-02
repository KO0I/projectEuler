//! Project Euler 98: Anagramic Squares.
use project_euler::extra_helpers::{is_square, isqrt};
use std::collections::HashMap;

fn solve() -> i64 {
    let words: Vec<&str> = include_str!("../../../../tests/data/p098_words.txt")
        .split(',')
        .map(|w| w.trim().trim_matches('"'))
        .collect();
    let mut groups: HashMap<Vec<u8>, Vec<&str>> = HashMap::new();
    for word in &words {
        let mut key = word.as_bytes().to_vec();
        key.sort_unstable();
        groups.entry(key).or_default().push(word);
    }
    let mut best = 0u64;
    for group in groups.values() {
        if group.len() < 2 {
            continue;
        }
        let length = group[0].len() as u32;
        assert!(length < 19);
        let lower = 10u64.pow(length - 1);
        let upper = 10u64.pow(length);
        let mut start = isqrt(lower);
        if start * start < lower {
            start += 1;
        }
        let finish = isqrt(upper - 1);
        for (i, &a) in group.iter().enumerate() {
            for &b in &group[i + 1..] {
                for root in start..=finish {
                    let square = root * root;
                    let digits = square.to_string();
                    let mut letters = [None::<u8>; 26];
                    let mut used = [None::<u8>; 10];
                    let mut valid = true;
                    for (c, ch) in a.bytes().zip(digits.bytes()) {
                        let index = (c - b'A') as usize;
                        let digit = ch - b'0';
                        if letters[index].is_some_and(|v| v != digit)
                            || used[digit as usize].is_some_and(|v| v != c)
                        {
                            valid = false;
                            break;
                        }
                        letters[index] = Some(digit);
                        used[digit as usize] = Some(c);
                    }
                    if !valid {
                        continue;
                    }
                    let mut mapped = 0u64;
                    for c in b.bytes() {
                        let digit = letters[(c - b'A') as usize].unwrap();
                        if mapped == 0 && digit == 0 {
                            valid = false;
                            break;
                        }
                        mapped = mapped * 10 + digit as u64;
                    }
                    if valid && is_square(mapped) {
                        best = best.max(square).max(mapped);
                    }
                }
            }
        }
    }
    best as i64
}

fn main() {
    println!("{}", solve());
}
