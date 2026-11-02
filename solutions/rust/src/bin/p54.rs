//! Project Euler 54: Poker Hands.

fn poker_value(cards: &[&str]) -> u64 {
    let mut counts = [0usize; 15];
    let mut flush = true;
    let suit = cards[0].as_bytes()[1];
    for card in cards {
        let b = card.as_bytes();
        let value = b"23456789TJQKA"
            .iter()
            .position(|&c| c == b[0])
            .expect("rank")
            + 2;
        counts[value] += 1;
        flush &= b[1] == suit;
    }
    let descending: Vec<usize> = (2..=14).rev().filter(|&i| counts[i] > 0).collect();
    let mut pairs: Vec<usize> = descending
        .iter()
        .copied()
        .filter(|&i| counts[i] == 2)
        .collect();
    pairs.sort_unstable_by(|a, b| b.cmp(a));
    let triple = descending
        .iter()
        .copied()
        .find(|&i| counts[i] == 3)
        .unwrap_or(0);
    let quad = descending
        .iter()
        .copied()
        .find(|&i| counts[i] == 4)
        .unwrap_or(0);
    let straight = if descending.len() == 5 && descending[0] - descending[4] == 4 {
        descending[0]
    } else if descending == [14, 5, 4, 3, 2] {
        5
    } else {
        0
    };
    let mut key = Vec::new();
    let category;
    if straight > 0 && flush {
        category = 8;
        key.push(straight);
    } else if quad > 0 {
        category = 7;
        key.push(quad);
        key.push(*descending.iter().find(|&&v| v != quad).unwrap());
    } else if triple > 0 && pairs.len() == 1 {
        category = 6;
        key.push(triple);
        key.push(pairs[0]);
    } else if flush {
        category = 5;
        key = descending;
    } else if straight > 0 {
        category = 4;
        key.push(straight);
    } else if triple > 0 {
        category = 3;
        key.push(triple);
        key.extend(descending.iter().filter(|&&v| v != triple).copied());
    } else if pairs.len() == 2 {
        category = 2;
        key.extend(pairs);
        key.extend(descending.iter().filter(|&&v| counts[v] == 1).copied());
    } else if pairs.len() == 1 {
        category = 1;
        key.push(pairs[0]);
        key.extend(descending.iter().filter(|&&v| counts[v] == 1).copied());
    } else {
        category = 0;
        key = descending;
    }
    let mut value = category;
    for n in key.into_iter().chain(std::iter::repeat(0)).take(5) {
        value = value * 15 + n;
    }
    value as u64
}

fn solve() -> i64 {
    {
        let mut sum = 0;
        for line in include_str!("../../../../tests/data/p054_poker.txt").lines() {
            let cards: Vec<&str> = line.split_whitespace().collect();
            assert_eq!(cards.len(), 10);
            if poker_value(&cards[..5]) > poker_value(&cards[5..]) {
                sum += 1;
            }
        }
        sum
    }
}

fn main() {
    println!("{}", solve());
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn poker_hands() {
        assert!(
            poker_value(&["5H", "5C", "6S", "7S", "KD"])
                < poker_value(&["2C", "3S", "8S", "8D", "TD"])
        );
        assert!(
            poker_value(&["2D", "9C", "AS", "AH", "AC"])
                < poker_value(&["3D", "6D", "7D", "TD", "QD"])
        );
        assert!(
            poker_value(&["2C", "3C", "4C", "5C", "AC"])
                > poker_value(&["9D", "9S", "9H", "9C", "3S"])
        );
    }
}
