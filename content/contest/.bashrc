function q() {
    g++ -std=c++17 -O2 -lm -static -o "$1" "$1.cpp"
}