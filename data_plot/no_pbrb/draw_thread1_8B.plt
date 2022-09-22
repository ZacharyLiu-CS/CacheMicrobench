set size ratio 0.5
set title "Performance with 1 thread and 8B value" font ",20"
set auto x
set yrange [0:1000]
set style data histogram
set style histogram cluster gap 1
set style fill solid border -1
set boxwidth 1
# set style fill pattern 1
set palette color
set grid

set ylabel "KOPS" font ",20"
set xtics font ",20"
set key box font ",16" height 1 at graph 1,1 maxrows 2 spacing 0.7 width 0 samplen 3.4

set term pdfcairo font "Times-New-Roman,16"
set ytics nomirror
set xtics nomirror
set output "thread_1_valuesize_8B.pdf"
plot 'thread_1_valuesize_8B.dat'\
 using 2:xtic(1) ti col fillstyle pattern 2 linecolor rgb 0x9301d3 ,\
'' using 3 ti col linecolor rgb 0x009e23,\
'' using 4 ti col fillstyle pattern 2,\
'' using 6 ti col linecolor rgb 0xef9300,\
'' using 7 ti col fillstyle pattern 2 lc rgb 0xefe442,\
'' using 5 ti col linecolor rgb 0xff0000
