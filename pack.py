import os
os.popen('mkdir -p build')
os.popen('cp -r src/api/node/* build/node_modules/nodejs/')
os.popen('cp -r src/api/sidejs/* build/node_modules/sidejs/')
os.popen('cp cmake-build-debug/sidejs build/sidejs')
os.popen('cp tests/test.js build/test.js')