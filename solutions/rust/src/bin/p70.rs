//! Project Euler 70: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(70).expect("implemented Euler problem");
    println!("{answer}");
}
