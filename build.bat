docker run --rm -v %cd%:/code -u emscripten -w /code/build trzeci/emscripten emcc ../src/main.cpp -std=c++11 -s WASM=1 -s USE_WEBGL2=1 -s FULL_ES3=1 -O3 -o index.js
copy_static.bat