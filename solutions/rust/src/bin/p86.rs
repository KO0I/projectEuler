//! Project Euler 86: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(86).expect("implemented Euler problem");
    println!("{answer}");
}
