//! Project Euler 114: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(114).expect("implemented Euler problem");
    println!("{answer}");
}
