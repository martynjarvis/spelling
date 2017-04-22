use std::collections::HashMap;
use std::fs::File;
use std::io;
use std::io::prelude::*;


fn train() -> HashMap<String,i32> {
    let mut word_freq = HashMap::new();

    // open the training file
    let file = File::open("big.txt")
            .expect("Error opening file");
    let buf_reader = io::BufReader::new(file);

    for result in buf_reader.lines() {
        let line = result.unwrap();
        let words = line.split_whitespace();
        for word in words {
            let count = word_freq.entry(word.to_string()).or_insert(0);
            *count += 1;
        }
    }
    word_freq
}


fn edits(word: &str) -> Vec<String> {
    let ascii = "abcdefghijklmnopqrstuvwxyz";
    let mut ret = Vec::new();
    let l = word.len();
    // delete or replace
    for i in 0..l {
        let a = &word[0..i];
        let b = &word[i+1..l];
        ret.push(format!("{}{}", a, b));
        for c in ascii.chars() {
             ret.push(format!("{}{}{}", a, c, b));
        }
    }
    // insert
    for i in 0..l+1{
        let a = &word[0..i];
        let b = &word[i..l];
        for c in ascii.chars() {
             ret.push(format!("{}{}{}", a, c, b));
        }
    }
    // transpose
    for i in 1..l {
        let a = &word[0..i-1];
        let b = &word[i-1..i];
        let c = &word[i..i+1];
        let d = &word[i+1..l];
        ret.push(format!("{}{}{}{}", a, c, b, d));
    }
    // replace
    ret
}


fn main() {
    // Train corrector
    println!("Loading corpus");
    let word_freq = train();

    // Find most common
    let mut most_common = String::new();
    let mut highest_count = 0;
    for (word, &count) in word_freq.iter() {
        if count > highest_count {
            most_common = word.to_string();
            highest_count = count;
        }
    }
    println!("Most common word: {}: \"{}\"", most_common, highest_count);

    loop {
        // Take input from stdin
        println!("Input word: ");
        let mut input = String::new();
        io::stdin().read_line(&mut input)
            .expect("Error reading line");

        // Trim whitespace
        input = input.trim().to_string();

        // Is it correct?
        if word_freq.contains_key(&input){
            println!("{} is correct", input);
            continue;
        }

        let first_edits = edits(&input);

        let mut best_edit = String::new();
        let mut highest_count = 0;

        for e in &first_edits {
            let correction = word_freq.get(e);
            if correction.is_some(){
                let count = *correction.unwrap();
                if count > highest_count {
                    best_edit = e.to_string();
                    highest_count = count;
                }
            }
        }
        println!("correction: {}", best_edit);
    }
}
