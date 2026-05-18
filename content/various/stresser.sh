# In gen, use main(int argc, char* argv[]) 
# with srand(atoi(argv[1]))
for((i=1;;++i)); do
	echo $i;
	./gen $i > r
	./a < r > w1
	./abf < r > w2
	diff -w w1 w2 || break
done