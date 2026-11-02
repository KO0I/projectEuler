// Project Euler 189: tri-colouring a triangular grid.
// A state is the base-3 colouring of the upward triangles in the latest row.
// Given adjacent rows, each intervening downward triangle has 3 minus the
// number of distinct neighbouring colours independent choices.
fn pow3(n: usize) -> usize {
    3usize.pow(n as u32)
}
fn solve() -> u64 {
    const SIDE: usize = 8;
    const STATES: usize = 6561;
    let mut decoded = vec![[0u8; SIDE]; STATES];
    for state in 0..STATES {
        let mut x = state;
        for j in 0..SIDE {
            decoded[state][j] = (x % 3) as u8;
            x /= 3;
        }
    }
    let mut previous = vec![0u64; STATES];
    previous[0] = 1;
    previous[1] = 1;
    previous[2] = 1;
    for row in 2..=SIDE {
        let mut current = vec![0u64; STATES];
        for before in 0..pow3(row - 1) {
            if previous[before] == 0 {
                continue;
            }
            for after in 0..pow3(row) {
                let mut multiplicity = 1u64;
                for j in 0..row - 1 {
                    let (a, b, c) = (decoded[before][j], decoded[after][j], decoded[after][j + 1]);
                    let distinct = 1 + (b != a) as u64 + (c != a && c != b) as u64;
                    multiplicity *= 3 - distinct;
                    if multiplicity == 0 {
                        break;
                    }
                }
                current[after] += previous[before] * multiplicity;
            }
        }
        previous = current;
    }
    previous[..pow3(SIDE)].iter().sum()
}
fn main() {
    println!("{}", solve());
}
