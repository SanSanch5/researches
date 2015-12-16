set terminal pdfcairo dashed enhanced font "Helvetica,7"
set output outfilename
set encoding utf8

set mxtics 4
set mytics 3
set format x "{%.1t}*10^{%L}" 
set format x "%g"
set grid xtics ytics 

set xlabel  "Количество элементов массива"
set ylabel  "Время работы сортировок, мc"

set style line 2 lt -1 lw 1 ps 0.5 pi -60 pt 5
set style line 3 lt -1 lw 1 ps 0.5 pi -50 pt 4
set style line 7 lt -1 lw 1 ps 0.5 pi -26 pt 6 

set style line 5 lt -1 lw 2 ps 0.3 pi -29 pt 2
set style line 4 lt -1 lw 2 ps 0.3 pi -25 pt 1 
set style line 6 lt -1 lw 2 ps 0.3 pi -27 pt 3 
set style line 8 lt -1 lw 2 ps 0.3 pi -26 pt 7 
set style line 9 lt -1 lw 2 ps 0.3 pi -25 pt 8 
set style line 10 lt -1 lw 2 ps 0.3 pi -5 pt 9 
set style line 11 lt -1 lw 2 ps 0.3 pi -8 pt 10 
plot infilename using 1:($2*1000) title "1" with linespoint ls 7 lc rgb "magenta",\
 	 infilename using 1:($3*1000) title "2" with linespoint ls 2 lc rgb "blue",\
	 infilename using 1:($4*1000) title "3" with linespoint ls 3 lc rgb "orange"
