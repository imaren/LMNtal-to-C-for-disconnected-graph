max=100
max2=1000

for((i=10; i<= $max; i+=10)); do
    ./makegraph $i > tmp.txt
    tail -n 1 tmp.txt > testcase/C$i
    head -n 1 tmp.txt > testcase/L$i
done

for((i=200; i<= $max2; i+=100)); do
    ./makegraph $i > tmp.txt
    tail -n 1 tmp.txt > testcase/C$i
    head -n 1 tmp.txt > testcase/L$i
done