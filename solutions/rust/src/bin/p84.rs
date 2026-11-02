//! Project Euler 84: Monopoly Odds.

const STAY: i8 = -1;

const NEXT_R: i8 = -2;

const NEXT_U: i8 = -3;

const BACK: i8 = -4;

const SQUARES: usize = 40;

const STATES: usize = SQUARES * 3 * 16 * 16;

const CHEST: [i8; 16] = [
    STAY, 0, STAY, STAY, STAY, STAY, 10, STAY, STAY, STAY, STAY, STAY, STAY, STAY, STAY, STAY,
];

const CHANCE: [i8; 16] = [
    STAY, NEXT_R, 24, STAY, 0, NEXT_U, STAY, 11, BACK, 10, STAY, 5, NEXT_R, STAY, 39, STAY,
];

fn p84_index(square: usize, doubles: usize, cc: usize, ch: usize) -> usize {
    ((square * 3 + doubles) * 16 + cc) * 16 + ch
}

fn p84_transition(
    mut state: usize,
    roll: (usize, bool),
    cc_deck: &[i8; 16],
    ch_deck: &[i8; 16],
) -> usize {
    let mut ch = state % 16;
    state /= 16;
    let mut cc = state % 16;
    state /= 16;
    let mut doubles = state % 3;
    let mut square = state / 3;
    if roll.1 && doubles == 2 {
        return p84_index(10, 0, cc, ch);
    }
    doubles = if roll.1 { doubles + 1 } else { 0 };
    square = (square + roll.0) % 40;
    let mut jailed = square == 30;
    if [7, 22, 36].contains(&square) {
        let card = ch_deck[ch];
        ch = (ch + 1) % 16;
        if card >= 0 {
            square = card as usize;
            jailed = card == 10;
        } else if card == NEXT_R {
            square = match square {
                7 => 15,
                22 => 25,
                _ => 5,
            };
        } else if card == NEXT_U {
            square = if square == 22 { 28 } else { 12 };
        } else if card == BACK {
            square -= 3;
        }
    }
    if [2, 17, 33].contains(&square) {
        let card = cc_deck[cc];
        cc = (cc + 1) % 16;
        if card >= 0 {
            square = card as usize;
            jailed = card == 10;
        }
    }
    if jailed {
        square = 10;
        doubles = 0;
    }
    p84_index(square, doubles, cc, ch)
}

fn solve() -> i64 {
    let sides = 4usize;
    let mut rolls: Vec<(usize, bool, usize)> = Vec::new();
    for a in 1..=sides {
        for b in 1..=sides {
            let sum = a + b;
            let is_double = a == b;
            if let Some(item) = rolls.iter_mut().find(|r| r.0 == sum && r.1 == is_double) {
                item.2 += 1;
            } else {
                rolls.push((sum, is_double, 1));
            }
        }
    }
    let count = rolls.len();
    let mut dest = vec![0u16; STATES * count];
    for state in 0..STATES {
        for (i, r) in rolls.iter().enumerate() {
            dest[state * count + i] = p84_transition(state, (r.0, r.1), &CHEST, &CHANCE) as u16;
        }
    }
    let mut probability = vec![1.0 / STATES as f64; STATES];
    let mut next = vec![0.0f64; STATES];
    let mut converged = false;
    for _ in 0..30000 {
        next.fill(0.0);
        for state in 0..STATES {
            let mass = probability[state] / (sides * sides) as f64;
            for (roll, &(_, _, multiplicity)) in rolls.iter().enumerate() {
                next[dest[state * count + roll] as usize] += mass * multiplicity as f64;
            }
        }
        let total: f64 = next.iter().sum();
        let mut difference = 0f64;
        for i in 0..STATES {
            next[i] /= total;
            difference += (next[i] - probability[i]).abs();
        }
        std::mem::swap(&mut probability, &mut next);
        if difference < 1e-13 {
            converged = true;
            break;
        }
    }
    assert!(
        converged,
        "Monopoly full-state power iteration did not converge"
    );
    let mut marginal = [0.0f64; 40];
    for (state, &mass) in probability.iter().enumerate() {
        marginal[state / (3 * 16 * 16)] += mass;
    }
    let mut ranking: Vec<usize> = (0..40).collect();
    ranking.sort_by(|&a, &b| marginal[b].total_cmp(&marginal[a]).then(a.cmp(&b)));
    (10000 * ranking[0] + 100 * ranking[1] + ranking[2]) as i64
}

fn main() {
    println!("{}", solve());
}
