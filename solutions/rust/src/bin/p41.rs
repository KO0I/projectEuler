//! Project Euler 41: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(41).expect("implemented Euler problem");
    println!("{answer}");
}
