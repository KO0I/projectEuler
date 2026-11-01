//! Project Euler 87: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(87).expect("implemented Euler problem");
    println!("{answer}");
}
