g++ -O2 guass_eli_vec.cpp -o vec
g++ -O2 guass_eli.cpp  -o normal
g++ -O2 SSE.cpp  -o  SSE
echo "normal"
./normal

echo "vec"
./vec

echo "SSE"
./SSE