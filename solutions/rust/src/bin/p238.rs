// Project Euler 238: infinite string tour.
// The modular squaring sequence is purely periodic.  Prefix digit-sum residues
// determine whether each target sum is attainable from a proposed start.
fn append(mut v: u32, prefix: &mut Vec<u32>, sum: &mut u32) {
    let mut divisor = 1;
    while divisor <= v / 10 {
        divisor *= 10;
    }
    loop {
        let digit = v / divisor;
        v %= divisor;
        prefix.push(*sum);
        *sum += digit;
        if divisor == 1 {
            break;
        }
        divisor /= 10;
    }
}
fn solve() -> u64 {
    const MOD: u64 = 20_300_713;
    const INITIAL: u32 = 14_025_256;
    const TARGET: u64 = 2_000_000_000_000_000;
    let (mut prefix, mut sum, mut state) = (Vec::new(), 0u32, INITIAL);
    loop {
        append(state, &mut prefix, &mut sum);
        state = (state as u64 * state as u64 % MOD) as u32;
        if state == INITIAL {
            break;
        }
    }
    let mut present = vec![false; sum as usize];
    for &p in &prefix {
        present[p as usize] = true;
    }
    let (mut period_sum, mut remainder_sum) = (0u64, 0u64);
    let rem = (TARGET % sum as u64) as u32;
    for k in 1..=sum {
        let target = if k == sum { 0 } else { k };
        let start = prefix
            .iter()
            .position(|&p| present[((p + target) % sum) as usize])
            .unwrap() as u64
            + 1;
        period_sum += start;
        if k <= rem {
            remainder_sum += start;
        }
    }
    TARGET / sum as u64 * period_sum + remainder_sum
}
fn main() {
    println!("{}", solve());
}
