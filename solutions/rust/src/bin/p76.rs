//! Project Euler 76: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(76).expect("implemented Euler problem");
    println!("{answer}");
}
