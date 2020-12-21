set title "Résultats de la simulation" font ",20"
set xlabel 'Nombre de tours'
set ylabel 'Nombre de citoyens
set style line 1 lc rgb 'gray' lt 1
set grid ytics ls 1
set grid xtics ls 1
set ytics 5
set xtics 10
set mytics 5
set mxtics 5
set key outside right center Left reverse
set xrange [0:100]
set yrange [0:37]

plot "./etc/evolution.txt" using 1:2 title 'Personnes saines' smooth csplines lw 2, \
"./etc/evolution.txt" using 1:3 title 'Personnes malades' smooth csplines lw 2, \
"./etc/evolution.txt" using 1:4 title 'Personnes décédées' smooth csplines lw 2, \
"./etc/evolution.txt" using 1:5 title 'Cadavres brûlés' smooth csplines lw 2