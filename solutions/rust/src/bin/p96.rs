//! Project Euler 96: Su Doku.

fn sudoku_solve(
    grid: &mut [u8; 81],
    row: &mut [u16; 9],
    col: &mut [u16; 9],
    box_mask: &mut [u16; 9],
    answers: &mut Vec<[u8; 81]>,
) {
    if answers.len() >= 2 {
        return;
    }
    let mut chosen = 81;
    let mut options = 0;
    let mut minimum = 10;
    for i in 0..81 {
        if grid[i] != 0 {
            continue;
        }
        let (r, c) = (i / 9, i % 9);
        let b = r / 3 * 3 + c / 3;
        let mask = 0x3fe & !(row[r] | col[c] | box_mask[b]);
        let number = mask.count_ones();
        if number == 0 {
            return;
        }
        if number < minimum {
            minimum = number;
            chosen = i;
            options = mask;
        }
    }
    if chosen == 81 {
        answers.push(*grid);
        return;
    }
    let (r, c) = (chosen / 9, chosen % 9);
    let b = r / 3 * 3 + c / 3;
    for digit in 1..=9 {
        let bit = 1u16 << digit;
        if options & bit == 0 {
            continue;
        }
        grid[chosen] = digit;
        row[r] |= bit;
        col[c] |= bit;
        box_mask[b] |= bit;
        sudoku_solve(grid, row, col, box_mask, answers);
        row[r] &= !bit;
        col[c] &= !bit;
        box_mask[b] &= !bit;
        grid[chosen] = 0;
    }
}

fn solve() -> i64 {
    let mut lines = include_str!("../../../../tests/data/p096_sudoku.txt").lines();
    let mut sum = 0;
    for index in 1..=50 {
        assert_eq!(lines.next().unwrap(), format!("Grid {index:02}"));
        let mut grid = [0u8; 81];
        let mut row = [0u16; 9];
        let mut col = [0u16; 9];
        let mut bx = [0u16; 9];
        for r in 0..9 {
            let text = lines.next().unwrap().as_bytes();
            assert_eq!(text.len(), 9);
            for c in 0..9 {
                let d = text[c] - b'0';
                grid[r * 9 + c] = d;
                if d > 0 {
                    let bit = 1u16 << d;
                    let b = r / 3 * 3 + c / 3;
                    assert_eq!((row[r] | col[c] | bx[b]) & bit, 0);
                    row[r] |= bit;
                    col[c] |= bit;
                    bx[b] |= bit;
                }
            }
        }
        let mut answers = Vec::new();
        sudoku_solve(&mut grid, &mut row, &mut col, &mut bx, &mut answers);
        assert_eq!(answers.len(), 1);
        sum += 100 * answers[0][0] as i64 + 10 * answers[0][1] as i64 + answers[0][2] as i64;
    }
    assert!(lines.next().is_none());
    sum
}

fn main() {
    println!("{}", solve());
}
