# SideJS

Javascript runtime based on the JavaScriptCore engine, supporting node modules and nice to embed in your c++ projects.

### How to build
1. clone repository
2. mkdir build
3. cd build
4. cmake ..
5. make

### Roadmap
- [x] JavaScriptCore
- [x] Libuv
- [x] https://nodejs.org/api/console.html (log, error, log aliases)
- [ ] https://nodejs.org/api/timers.html - WIP (setTimeout, setInterval, clearTimeout, clearInterval)
- [ ] http1
- [ ] http2
- [ ] https
- [ ] polyfills
- [ ] preload-api
- [ ] tcp
- [ ] udp
- [ ] https://nodejs.org/api/assert.html
- [ ] https://nodejs.org/api/buffer.html
- [ ] https://nodejs.org/api/events.html ?
- [ ] https://nodejs.org/api/os.html
- [ ] https://nodejs.org/api/path.html
- [ ] https://nodejs.org/api/module.html
- [ ] https://nodejs.org/api/process.html
- [ ] https://nodejs.org/api/querystring.html
- [ ] https://nodejs.org/api/readline.html
- [ ] https://nodejs.org/api/stream.html
- [ ] https://nodejs.org/api/string_decoder.html
- [ ] https://nodejs.org/api/tty.html
- [ ] https://nodejs.org/api/url.html
- [ ] https://nodejs.org/api/zlib.html