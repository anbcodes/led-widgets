let s = ''

let a = (+new Date(2022, 0, 14, 8, 15, 0)) / 1000;

console.error(new Date(a * 1000).toString());

// let a = (+new Date(+new Date() + 1000 * 60 * 0.5) / 1000)
s += `\\x${Math.floor(a % 256).toString(16).padStart(2, '0')}`
s += `\\x${Math.floor((a / 256) % 256).toString(16).padStart(2, '0')}`
s += `\\x${Math.floor((a / 256 / 256) % 256).toString(16).padStart(2, '0')}`
s += `\\x${Math.floor((a / 256 / 256 / 256) % 256).toString(16).padStart(2, '0')}`
s += `\\x${Math.floor((a / 256 / 256 / 256 / 256) % 256).toString(16).padStart(2, '0')}`
s += `\\x${Math.floor((a / 256 / 256 / 256 / 256 / 256) % 256).toString(16).padStart(2, '0')}`
s += `\\x${Math.floor((a / 256 / 256 / 256 / 256 / 256 / 256) % 256).toString(16).padStart(2, '0')}`
s += `\\x${Math.floor((a / 256 / 256 / 256 / 256 / 256 / 256 / 256) % 256).toString(16).padStart(2, '0')}`
console.log(s)
