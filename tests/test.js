const a = 1;
const b = 2;

function calc() {
    return a + b;
}

try {
    console.log("First")
    setInterval(() => {
        console.log("interval_tick")
    }, 1000)
    setTimeout(() => {
        setInterval(() => {
            console.log("interval_tick 2")
            setTimeout(() => {
                console.log("timeout 2")
            }, 2000)
        }, 1000)
        console.log("timeout")
    }, 2000)
    console.log("third")
} catch (e) {
    console.log(e)
}