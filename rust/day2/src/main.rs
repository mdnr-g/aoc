use std::fs;

fn main() {
    let f = fs::read_to_string("puzzle").expect("no file found");
    let lines = f.lines();
    let mut sum = 0;
    'next_game_id: for line in lines {
        let (game_id_str, game) = line.split_once(": ").unwrap();
        let game_id_iter: Vec<&str> = game_id_str.split(' ').collect();
        let game_id = game_id_iter[1].parse::<u32>().unwrap();
        let rounds: Vec<&str> = game.split("; ").collect();
        for round in rounds {
            let rolls: Vec<&str> = round.split(", ").collect();
            for roll in rolls {
                let (count, color) = roll.split_once(" ").unwrap();
                let count: u32 = count.parse::<u32>().unwrap();
                let game_valid: bool = match color {
                    "blue" => count <= 14,
                    "red" => count <= 12,
                    "green" => count <= 13,
                    _ => false,
                };
                if !game_valid {
                    continue 'next_game_id;
                } 
            }
        }
        sum += game_id;
    }
    println!("sum = {}", sum);
}
