set terminal png size 1024,768
set output "graphs/case_2.png"
set title "Lock Analysis Case 2 (R - 90%, Insert/Delete - 5%) 150359E,"
set xlabel "Threads"
set xrange [0:9]
set ylabel "Average time taken (ms)"
set style line 1 lc rgb '#00ff00' lt 1 lw 2 pt 7 pi -1 ps 3
set style line 2 lc rgb '#ff0000' lt 1 lw 2 pt 7 pi -1 ps 1.5
set style line 3 lc rgb '#0000ff' lt 1 lw 2 pt 7 pi -1 ps 1.5
set pointintervalbox 3
plot "out_2_seq.csv" with linespoints ls 1 title "Sequential", \
	"out_2_mutex.csv" with linespoints ls 2 title "Mutex", \
	"out_2_rwl.csv" with linespoints ls 3 title "Read Write Lock", \

