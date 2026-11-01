//! Project Euler 45: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(45).expect("implemented Euler problem");
    println!("{answer}");
}
