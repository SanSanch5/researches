gnuplot -e "specialdata='Сортировка случайно заполенного массива с мощностью алфавита, равной половине размера массива '; infilename='sortsdata/all_sorts_general'; outfilename='plots/all_sorts_general.pdf'" graphic.gnu
gnuplot -e "specialdata='Сортировка отсортированного массива'; infilename='sortsdata/with_sorted_array'; outfilename='plots/with_sorted_array.pdf'" graphic.gnu
gnuplot -e "specialdata='Сортировка массива с минимальным количеством повторяющихся данных'; infilename='sortsdata/minimum_equals'; outfilename='plots/minimum_equals.pdf'" graphic.gnu
gnuplot -e "specialdata='Сортировка массива с мощностью алфавита, равной 1% от размера алфавита'; infilename='sortsdata/much_equals'; outfilename='plots/much_equals.pdf'" graphic.gnu


echo Converting all_sorts_general to PNG ... 
convert -density 300 -trim plots/all_sorts_general.pdf -quality 100 -sharpen 0x1.0 plots/all_sorts_general.png
echo Converted
echo Opening ...
# xdg-open plots/all_sorts_general.pdf
echo Converting minimum_equals to PNG ... 
convert -density 300 -trim plots/minimum_equals.pdf -quality 100 -sharpen 0x1.0 plots/minimum_equals.png
echo Converted
echo Opening ...
# xdg-open plots/minimum_equals.pdf
echo Converting much_equals to PNG ... 
convert -density 300 -trim plots/much_equals.pdf -quality 100 -sharpen 0x1.0 plots/much_equals.png
echo Converted
echo Opening ...
# xdg-open plots/much_equals.pdf
echo Converting with_sorted_array to PNG ... 
convert -density 300 -trim plots/with_sorted_array.pdf -quality 100 -sharpen 0x1.0 plots/with_sorted_array.png
echo Converted
echo Opening ...
# xdg-open plots/with_sorted_array.pdf

