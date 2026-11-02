// Project Euler 185: Number Mind.
//
// Backtracking stores the matches committed for each clue.  We choose the
// unfilled column with the smallest legal digit domain (MRV), reject clues
// whose quota is exceeded or can no longer be reached, and try digits that
// satisfy the most outstanding clues first.
const GUESSES: [&[u8; 16]; 22] = [
    b"5616185650518293",
    b"3847439647293047",
    b"5855462940810587",
    b"9742855507068353",
    b"4296849643607543",
    b"3174248439465858",
    b"4513559094146117",
    b"7890971548908067",
    b"8157356344118483",
    b"2615250744386899",
    b"8690095851526254",
    b"6375711915077050",
    b"6913859173121360",
    b"6442889055042768",
    b"2321386104303845",
    b"2326509471271448",
    b"5251583379644322",
    b"1748270476758276",
    b"4895722652190306",
    b"3041631117224635",
    b"1841236454324589",
    b"2659862637316867",
];
const TARGET: [u8; 22] = [
    2, 1, 3, 3, 3, 1, 2, 3, 1, 2, 3, 1, 1, 2, 0, 2, 2, 3, 1, 3, 3, 2,
];
fn legal(pos: usize, d: u8, matches: &[u8; 22]) -> bool {
    if d == GUESSES[14][pos] - b'0' {
        return false;
    }
    (0..22).all(|c| d != GUESSES[c][pos] - b'0' || matches[c] != TARGET[c])
}
fn capacity_ok(filled: &[bool; 16], matches: &[u8; 22]) -> bool {
    (0..22).all(|c| {
        let possible = (0..16)
            .filter(|&p| !filled[p] && legal(p, GUESSES[c][p] - b'0', matches))
            .count();
        matches[c] as usize + possible >= TARGET[c] as usize
    })
}
fn search(
    depth: usize,
    answer: &mut [u8; 16],
    filled: &mut [bool; 16],
    matches: &mut [u8; 22],
) -> bool {
    if depth == 16 {
        return matches == &TARGET;
    }
    let mut best_pos = 0;
    let mut best: Vec<(u8, usize)> = Vec::new();
    let mut best_len = 11;
    for pos in 0..16 {
        if filled[pos] {
            continue;
        }
        let mut candidates = Vec::new();
        for d in 0..=9 {
            if legal(pos, d, matches) {
                let score = (0..22)
                    .filter(|&c| matches[c] < TARGET[c] && d == GUESSES[c][pos] - b'0')
                    .count();
                candidates.push((d, score));
            }
        }
        if candidates.is_empty() {
            return false;
        }
        if candidates.len() < best_len {
            best_len = candidates.len();
            best_pos = pos;
            best = candidates;
        }
    }
    best.sort_unstable_by_key(|&(_, score)| std::cmp::Reverse(score));
    filled[best_pos] = true;
    for (d, _) in best {
        answer[best_pos] = d;
        for c in 0..22 {
            matches[c] += (d == GUESSES[c][best_pos] - b'0') as u8;
        }
        if capacity_ok(filled, matches) && search(depth + 1, answer, filled, matches) {
            return true;
        }
        for c in 0..22 {
            matches[c] -= (d == GUESSES[c][best_pos] - b'0') as u8;
        }
    }
    filled[best_pos] = false;
    false
}
fn solve() -> String {
    let mut answer = [0u8; 16];
    assert!(search(0, &mut answer, &mut [false; 16], &mut [0; 22]));
    answer.iter().map(|d| (b'0' + d) as char).collect()
}
fn main() {
    println!("{}", solve());
}
