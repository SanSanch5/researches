set terminal pdfcairo dashed enhanced font "Helvetica,7"
set output outfilename
set encoding utf8
set key r tm

set mxtics 4
set mytics 3
set format x "{%.1t}*10^{%L}" 
set format x "%g"
set grid xtics ytics 

set title specialdata
set xlabel  "Количество элементов массива"
set ylabel  "Время работы сортировок, мc"

set style line 5 lt -1 lw 2 ps 0.3 pi -29 pt 2
set style line 2 lt -1 lw 3 ps 0.3 pi -26 pt 5
set style line 3 lt -1 lw 2 ps 0.3 pi -25 pt 4
set style line 4 lt -1 lw 2 ps 0.3 pi -25 pt 1 
set style line 6 lt -1 lw 2 ps 0.3 pi -27 pt 3 
set style line 7 lt -1 lw 2 ps 0.3 pi -26 pt 6 
set style line 8 lt -1 lw 2 ps 0.3 pi -26 pt 7 
set style line 9 lt -1 lw 2 ps 0.3 pi -25 pt 8 
set style line 10 lt -1 lw 2 ps 0.3 pi -34 pt 9 
plot infilename using 1:($2*1000) title "Сортировка слиянием (вставки при количестве элементов <= 32) T_n={/Symbol W}(lg(n^n)), E(T_n)={/Symbol Q}(lg(n!))" with linespoint ls 7 lc rgb "magenta",\
     infilename using 1:($3*1000) title "Пирамидальная сортировка T_n={/Symbol Q}(lg(n^n))" with linespoint ls 6,\
 	 infilename using 1:($4*1000) title "Быстрая сортировка с хвостовой рекурсией и выбором случайной базы (вставки при количестве элементов <= 19) E(T_n)={/Symbol Q}(lg(n^n))" with linespoint ls 2 lc rgb "blue",\
	 infilename using 1:($5*1000) title "Быстрая сортировка с выбором в качестве базы медианы из трёх случайных элементов (вставки при количестве элементов <= 18) E(T_n)={/Symbol Q}(lg(n^n))" with linespoint ls 3 lc rgb "orange",\
     infilename using 1:($6*1000) title "Быстрая сортировка с особой обработкой элементов, равных базе (вставки при количестве элементов <= 138) E(T_n)={/Symbol Q}(lg(n^n))" with linespoint ls 4 lc rgb "dark-red",\
     infilename using 1:($7*1000) title "Сортировка, встроенная в библиотеку QtAlgorithms" with linespoint ls 8 lc rgb "dark-green",\
     infilename using 1:($8*1000) title "Timsort с режимом галопа" with linespoint ls 10 lc rgb "dark-blue",\
     infilename using 1:($9*1000) title "Timsort с со слиянием k массивов на куче" with linespoint ls 9 lc rgb "green"
