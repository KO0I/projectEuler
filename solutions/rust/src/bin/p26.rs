//! Project Euler 26: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(26).expect("implemented Euler problem");
    println!("{answer}");
}
