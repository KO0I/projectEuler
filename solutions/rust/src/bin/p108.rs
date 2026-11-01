//! Project Euler 108: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(108).expect("implemented Euler problem");
    println!("{answer}");
}
