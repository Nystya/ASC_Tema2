Vajaiala-Tomici Catalin-Mihai
332CB
Tema2-ASC

Toate cele 4 variante folosesc acelasi algoritm:
    - (zerotr(AtB + BtA))^2
    In primul rand m-am bazat pe faptul ca AtB = (BtA)t pentru a evita
    o inmultire. De asemenea la ridicarea la puterea a 2-a am profitat
    de faptul ca matricea este superior triunghiulara si am facut
    inmultirea doar pe jumatate de matrice, restul fiind 0.


Pentru varianta care foloseste BLAS, am folosit functiile cblas_dgemm
pentru inmultirea a doua matrici si cblas_dtrmm care inmulteste o
matrice superior triunghiulara cu alta matrice.

Pentru varianta optimizata manual am folosit urmatoarele tehnici:
    - Blocked Matrix Multiplication cu dimensiunea blocului 40.
    - Ordinea i j k ib kb jb deoarece ordinea interioara ib kb jb
    face mai putine accese la memorie.
    - Am folosit pointeri in loc sa calculez la fiecare acces dintr-o
    matrice indexul valorii.
    - Folosirea keyword-ului "register" pentru a tine cele mai folosite
    variabile in registii.
    - Loop unrolling pentru bucla jb, eliminand-o astfel.

Au fost optimizate in special inmultirea si ridicarea la putere, fiind
cele mai costisitoare operatii.
