//! Project Euler 63: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(63).expect("implemented Euler problem");
    println!("{answer}");
}
