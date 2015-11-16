gnuplot -e "specialdata='Сортировка случайно заполенного массива с мощностью алфавита, равной половине размера массива '; infilename='sortsdata/my_sort_all_sorts_general'; outfilename='plots/my_sort_all_sorts_general.pdf'" graphic.gnu
gnuplot -e "specialdata='Сортировка отсортированного массива'; infilename='sortsdata/my_sort_sorted'; outfilename='plots/my_sort_sorted.pdf'" graphic.gnu
gnuplot -e "specialdata='Сортировка массива с минимальным количеством повторяющихся данных'; infilename='sortsdata/my_sort_minimum_equals'; outfilename='plots/my_sort_minimum_equals.pdf'" graphic.gnu
gnuplot -e "specialdata='Сортировка массива с мощностью алфавита, равной 1% от размера алфавита'; infilename='sortsdata/my_sort_much_equals'; outfilename='plots/my_sort_much_equals.pdf'" graphic.gnu


echo Converting all_sorts_general to PNG ... 
convert -density 300 -trim plots/my_sort_all_sorts_general.pdf -quality 100 -sharpen 0x1.0 plots/my_sort_all_sorts_general.png
echo Converted
echo Opening ...
# xdg-open plots/my_sort_all_sorts_general.pdf
echo Converting minimum_equals to PNG ... 
convert -density 300 -trim plots/my_sort_minimum_equals.pdf -quality 100 -sharpen 0x1.0 plots/my_sort_minimum_equals.png
echo Converted
echo Opening ...
# xdg-open plots/my_sort_minimum_equals.pdf
echo Converting much_equals to PNG ... 
convert -density 300 -trim plots/my_sort_much_equals.pdf -quality 100 -sharpen 0x1.0 plots/my_sort_much_equals.png
echo Converted
echo Opening ...
# xdg-open plots/my_sort_much_equals.pdf
echo Converting sorted to PNG ... 
convert -density 300 -trim plots/my_sort_sorted.pdf -quality 100 -sharpen 0x1.0 plots/my_sort_sorted.png
echo Converted
echo Opening ...
# xdg-open plots/my_sort_sorted.pdf

