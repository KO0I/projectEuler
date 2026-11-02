//! Only the two corner classes of each hexagonal ring can have PD(n)=3.
use project_euler::extra_helpers::prime64;
fn solve() -> u64 {
    let (mut found, mut answer) = (1, 1);
    for n in 1u64.. {
        if prime64(6 * n - 1) && prime64(6 * n + 1) && prime64(12 * n + 5) {
            answer = 3 * n * (n - 1) + 2;
            found += 1;
            if found == 2000 {
                break;
            }
        }
        if n > 1 && prime64(6 * n - 1) && prime64(6 * n + 5) && prime64(12 * n - 7) {
            answer = 3 * n * (n + 1) + 1;
            found += 1;
            if found == 2000 {
                break;
            }
        }
    }
    answer
}
fn main() {
    println!("{}", solve());
}
