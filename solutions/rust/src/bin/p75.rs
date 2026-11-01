//! Project Euler 75: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(75).expect("implemented Euler problem");
    println!("{answer}");
}
