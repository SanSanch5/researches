gnuplot -e "specialdata='Сортировка случайно заполенного массива с мощностью алфавита 1000'; infilename='sortsdata/ins3q_heap'; outfilename='plots/ins3q_heap.pdf'" graphic.gnu
gnuplot -e "specialdata='Сортировка отсортированного массива'; infilename='sortsdata/with_sorted_array'; outfilename='plots/with_sorted_array.pdf'" graphic.gnu
gnuplot -e "specialdata='Сортировка массива с минимальным количеством повторяющихся данных'; infilename='sortsdata/minimum_equals'; outfilename='plots/minimum_equals.pdf'" graphic.gnu
gnuplot -e "specialdata='Сортировка массива с мощностью алфавита 50'; infilename='sortsdata/much_equals'; outfilename='plots/much_equals.pdf'" graphic.gnu

xdg-open plots/ins3q_heap.pdf
xdg-open plots/minimum_equals.pdf
xdg-open plots/much_equals.pdf
xdg-open plots/with_sorted_array.pdf

convert -density 300 -trim plots/ins3q_heap.pdf -quality 100 -sharpen 0x1.0 plots/ins3q_heap.png
convert -density 300 -trim plots/minimum_equals.pdf -quality 100 -sharpen 0x1.0 plots/minimum_equals.png
convert -density 300 -trim plots/much_equals.pdf -quality 100 -sharpen 0x1.0 plots/much_equals.png
convert -density 300 -trim plots/with_sorted_array.pdf -quality 100 -sharpen 0x1.0 plots/with_sorted_array.png

