use std::fs;

struct MaxValues<'a> {
    _color: &'a str,
    count: u32,
}

fn main() {

    let f = fs::read_to_string("puzzle").expect("no file found");
    let lines = f.lines();
    let mut sum = 0;
    for line in lines {
        let mut max: [MaxValues; 3] = [
            MaxValues {_color: "red", count: 0},
            MaxValues {_color: "green", count: 0},
            MaxValues {_color: "blue", count: 0},
        ];
        let (_game_id_str, game) = line.split_once(": ").unwrap();
        let rounds: Vec<&str> = game.split("; ").collect();
        for round in rounds {
            let rolls: Vec<&str> = round.split(", ").collect();
            for roll in rolls {
                let (count, color) = roll.split_once(" ").unwrap();
                let count: u32 = count.parse::<u32>().unwrap();
                match color {
                    "red" => {max[0].count = std::cmp::max(count, max[0].count);}
                    "green" => {max[1].count = std::cmp::max(count, max[1].count);}
                    "blue" => {max[2].count = std::cmp::max(count, max[2].count);}
                    _ => todo!()
                }
            }
        }
        let power = max[0].count * max[1].count * max[2].count;
        sum += power;
    }
    println!("sum = {}", sum);
}
