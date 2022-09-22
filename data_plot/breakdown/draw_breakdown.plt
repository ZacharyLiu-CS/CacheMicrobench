set size ratio 0.5
set title "Performance breakdown of each operation" font ",20"
set auto x
set yrange [0:3000]
set style data histograms
set style histogram rowstacked
set style fill solid border -1
set boxwidth 0.5 relative
# set style fill pattern 1
set palette color
set grid

set ylabel "Latency/ns" font ",20"
set xtics font ",20"
set key box font ",16" height 1 at graph 1,1 maxrows 8 spacing 0.8 width 0 samplen 3.4
set key Right

set term pdfcairo font "Times-New-Roman,16"
set ytics nomirror
set xtics nomirror
set output "basic_breakdown.pdf"

plot 'breakdown.dat' using 2:xtic(1) ti col fillstyle pattern 2 linecolor rgb 0x9301d3 ,\
'' using 3 ti col linecolor rgb 0x009e23,\
'' using 4 ti col fillstyle pattern 2,\
'' using 5 ti col linecolor rgb 0xff0000, \
'' using 6 ti col linecolor rgb 0xef9300,\
'' using 7 ti col fillstyle pattern 2 lc rgb 0xefe442