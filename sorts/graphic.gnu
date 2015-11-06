set terminal pdfcairo dashed enhanced font "Helvetica,8"
set output outfilename
set encoding utf8
set key r tm

set mxtics 4
set mytics 3
set format x "{%.1t}*10^{%L}" 
set grid xtics ytics mxtics mytics 

set title specialdata
set xlabel  "Количество элементов массива"
set ylabel  "Время работы сортировок, мc"

set style line 5 lt -1 lw 2 ps 0.3 pi -9 pt 2
set style line 2 lt -1 lw 3 ps 0.3 pi -13 pt 5
set style line 3 lt -1 lw 2 ps 0.3 pi -5 pt 4
set style line 4 lt -1 lw 2 ps 0.3 pi -10 pt 1 
set style line 6 lt -1 lw 2 ps 0.3 pi -7 pt 3 
set style line 7 lt -1 lw 2 ps 0.3 pi -6 pt 6 
plot infilename using 1:($2*1000) title "Сортировка вставками с бинарным поиском позиции и стандратным сдвигом блока памяти T_n={/Symbol W}(n)=O(lg(n!))" w linespoints ls 5 lc rgb "dark-green",\
	 infilename using 1:($3*1000) title "Сортировка слиянием с применением вставок на нижних уровнях T_n={/Symbol W}(lg(n^n)), E(T_n)={/Symbol Q}(lg(n!))" w linespoints ls 7 lc rgb "magenta",\
     infilename using 1:($4*1000) title "Пирамидальная сортировка T_n={/Symbol Q}(lg(n^n))" w linespoints ls 6,\
 	 infilename using 1:($5*1000) title "Быстрая сортировка с хвостовой рекурсией и выбором случайной базы E(T_n)={/Symbol Q}(lg(n^n))" w linespoints ls 2 lc rgb "blue",\
	 infilename using 1:($6*1000) title "Быстрая сортировка с выбором в качестве базы медианы из трёх случайных элементов E(T_n)={/Symbol Q}(lg(n^n))" w linespoints ls 3 lc rgb "orange",\
     infilename using 1:($7*1000) title "Быстрая сортировка с особой обработкой элементов, равных базе E(T_n)={/Symbol Q}(lg(n^n))" w linespoints ls 4 lc rgb "dark-red"
