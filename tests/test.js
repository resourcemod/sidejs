const {Buffer} = require('node:buffer')

console.log(Buffer)
try {

// Creates a zero-filled Buffer of length 10.
    const buf1 = Buffer.alloc(10);

// Creates a Buffer of length 10,
// filled with bytes which all have the value `1`.
    const buf2 = Buffer.alloc(10, 1);

// Creates an uninitialized buffer of length 10.
// This is faster than calling Buffer.alloc() but the returned
// Buffer instance might contain old data that needs to be
// overwritten using fill(), write(), or other functions that fill the Buffer's
// contents.
    const buf3 = Buffer.allocUnsafe(10);

// Creates a Buffer containing the bytes [1, 2, 3].
    const buf4 = Buffer.from([1, 2, 3]);
} catch (e) {
    console.log(e)
}