//! Project Euler 58: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(58).expect("implemented Euler problem");
    println!("{answer}");
}
