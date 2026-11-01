//! Project Euler 119: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(119).expect("implemented Euler problem");
    println!("{answer}");
}
