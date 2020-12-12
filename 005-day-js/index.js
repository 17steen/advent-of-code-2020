import { readFileSync } from "fs";

console.time("day 5 time");

const toBinary = {
    "F": "0",
    "B": "1",
    "R": "1",
    "L": "0"
};

let thing = readFileSync("./input")
    .toString()
    .split("\n")
    .map(v =>
        parseInt([...v].map(letter => toBinary[letter]).join(""), 2)
    );

let part_1 = Math.max(...thing);
let part_2 = thing.sort((a, b) => a - b).find((val, idx, arr) => val == arr[idx + 1] - 2);


console.timeEnd("day 5 time");

console.log(part_1, part_2);



