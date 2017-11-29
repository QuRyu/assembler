cd test
clang++ -std=c++11 ../parser.cpp ../parser.hpp parser_test.cpp
./a.out

clang++ -std=c++11 ../parser.cpp ../parser.hpp ../main.cpp 
./a.out ./program1
cmp --silent program1.mif fibonacci.mif || echo "files are different" 

