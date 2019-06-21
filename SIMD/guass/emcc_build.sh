em++ -O2 guass_eli_vec.cpp -o vec.html -s ASSERTIONS=1 -s TOTAL_MEMORY=1024MB
emcc -O2 guass_eli.cpp -o normal.html -s ASSERTIONS=1 -s TOTAL_MEMORY=1024MB
emrun vec.html
