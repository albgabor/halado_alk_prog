## Mátrixok összeszorzása több szálon

A többszálas szorzás a matrix.hh végén van.

Úgy oldottam meg, hogy a baloldali mátrix soraival történő szorzást osztotta fel a szálak között.
Így az eredmény mátrix különböző sorait töltötték föl a különböző szálak.

### Adatfájlok ("results_*.txt"):

"simple": egyszerű operator* (egyszálas)

"1","2","4": ennyi új szálon futtattam a szorzást

"many": annyi új szálat indítottam, amekkorák a (négyzetes) mátrixok voltak.

#### Megjegyzés:

Az itthoni gépemben 2 fizikai mag van (de mutithreading-el 4 szálon tud futtatni).

### Ábrák:

"total.pdf": Minden ábrázolva van rajta.

"1st_3rd.pdf": A futtatási tartomány első harmada van ábrázolva.

"2nd...": A futtatási tartomány második harmada.

"3rd...": A futtatási tartomány harmadik harmada.
