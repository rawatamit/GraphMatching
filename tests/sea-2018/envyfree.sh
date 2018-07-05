#DIRPATH=/home/rawatamit/Desktop/sea/popular/HRLQ
GMBIN=/home/rawatamit/Desktop/sea/GraphMatching/build/graphmatching
GFILE=/home/rawatamit/Desktop/sea/GraphMatching/tests/sea-2018/figure1.txt
EMATCHING=/home/rawatamit/Desktop/sea/GraphMatching/tests/sea-2018/E_figure1.txt
YMATCHING=/home/rawatamit/Desktop/sea/GraphMatching/tests/sea-2018/Y_figure1.txt
MMATCHING=/home/rawatamit/Desktop/sea/GraphMatching/tests/sea-2018/M_figure1.txt

# $GMBIN -p -i "$GFILE" -o "$EMATCHING"

$GMBIN -e -i "$GFILE" -o "$EMATCHING"
$GMBIN -y -i "$GFILE" -o "$YMATCHING"

if [ -f "$EMATCHING" ] && [ -f "$YMATCHING" ]; then
  cat "$EMATCHING" "$YMATCHING" > "$MMATCHING"
fi
