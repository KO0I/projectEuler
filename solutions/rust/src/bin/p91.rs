//! Project Euler 91: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(91).expect("implemented Euler problem");
    println!("{answer}");
}
