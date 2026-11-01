//! Project Euler 74: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(74).expect("implemented Euler problem");
    println!("{answer}");
}
