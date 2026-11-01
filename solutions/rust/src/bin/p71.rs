//! Project Euler 71: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(71).expect("implemented Euler problem");
    println!("{answer}");
}
