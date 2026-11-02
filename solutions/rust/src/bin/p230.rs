// Project Euler 230: Fibonacci words.
// Store saturated term lengths only; descend a requested position through
// concatenations until it reaches one of the two 100-digit seeds.
const A:&[u8]=b"1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679";
const B:&[u8]=b"8214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196";
fn digit(mut pos: u64) -> u64 {
    let mut lengths = [0u64; 100];
    lengths[0] = 100;
    lengths[1] = 100;
    let mut term = 1;
    while lengths[term] < pos {
        term += 1;
        lengths[term] = lengths[term - 2].saturating_add(lengths[term - 1]);
    }
    while term >= 2 {
        if pos <= lengths[term - 2] {
            term -= 2
        } else {
            pos -= lengths[term - 2];
            term -= 1
        }
    }
    (if term == 0 { A } else { B })[pos as usize - 1] as u64 - (b'0' as u64)
}
fn solve() -> u64 {
    let (mut sum, mut p7, mut p10) = (0, 1, 1);
    for n in 0..=17 {
        sum += digit((127 + 19 * n) * p7) * p10;
        p7 *= 7;
        p10 *= 10;
    }
    sum
}
fn main() {
    println!("{}", solve());
}
