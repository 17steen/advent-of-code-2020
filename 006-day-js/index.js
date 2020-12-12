import { readFileSync } from "fs";

console.time("day 6 time");

let part1 = readFileSync("input")
    .toString()
    .split("\n\n")
    .map(v => [...new Set([...v.split("\n").join("")])].length)
    .reduce((a, b) => a + b)

let part2 = readFileSync("input")
    .toString()
    .split("\n\n")
    .map(v => v
        .split("\n")
        .reduce((a, b) =>
            [...b].filter(letter => a.includes(letter))).length)
    .reduce((a, b) => a + b)


console.log(part1, part2);


console.timeEnd("day 6 time");




