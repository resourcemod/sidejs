const a = 1;
const b = 2;

function calc() {
    return a + b;
}

const secondInterval = () => {
    console.log("Second interval tick");
    try {
        clearTimeout(0)
    } catch (e) {
        console.log(e);
    }
}
const firstInterval = () => {
    console.log("First interval tick");
}

try {
    console.log("First")
    setTimeout(secondInterval, 500)
    setTimeout(firstInterval, 2000)
    console.log("third")
} catch (e) {
    console.log(e)
}