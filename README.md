# 2024_Analysis_12-Uno
## Autor
- Ime i prezime: **Marija Ristić**
- Broj indeksa: **1008/2024**
- Kontakt: **mi241008@alas.matf.bg.ac.rs**
## Informacije o projektu 
Ovaj repozitorijum sadrži rezultate praktičnog seminarskog rada u okviru kursa *Verifikacija softvera*, na master studijama Matematičkog fakulteta u Beogradu. Izrada projekta obuhvata primenu alata za verifikaciju softvera na odabranom projektu otvorenog koda i analizu dobijenih rezultata.

### Informacije o projektu koji se analizira
- Naziv: **UNO**
- Link do izvornog koda: [GitLab Repository](https://gitlab.com/matf-bg-ac-rs/course-rs/projects-2021-2022/12-Uno)
- Grana:  ```main```
- Commit hash: ```d8426a1```
- Kratak opis: UNO je multiplayer kartaška igra koja se može igrati protiv botova ili pravih igrača povezanih preko servera, sa ukupno 2, 4 ili 8 učesnika. Cilj igre je da igrač ostane bez svih karata u ruci, dok istovremeno zarađuje poene tako što postavlja karte na talon. Na kraju igre, ukupan broj osvojenih poena određuje konačni rezultat svakog igrača.

## Korišćeni alati za analizu, instalacija i uputstvo za pokretanje
Alati korišćeni za analizu koda:
- **Clang-format** - za formatiranje koda
- **CodeChecker** - statički analizator za detektovanje potencijalnih grešaka i rizičnih konstrukcija
- **Lcov** - za merenje pokrivenosti koda testovima
- **Memcheck** - za pronalaženje grešaka u radu sa memorijom prilikom izvršavanja programa

Rezultati svakog alata mogu se reprodukovati pozicioniranjem u istoimenom direktorijumu i pokretanjem skripte. Uputstvo za instalaciju i pokretanje alata na Linux sistemima:

- **1. Clang-format**
    - Instalacija: ```sudo apt-get install clang-format```
    - Pokretanje: U direktorijumu *2024_Analysis_12-Uno/clang-format* pokrenuti ```./run_clang_format.sh``` skript fajl. U istom direktorijumu, fajl *.clang-format* se može izmeniti, pre pokretanja alata, prema ličnim preferancama za formatiranje projekta
- **2. CodeChecker**
    - Instalacija: ```pip3 install codechecker``` ili praćenjem [uputstva](https://codechecker.readthedocs.io/en/latest/#linux-build-from-source) za kompajliranje iz izvornog koda 
    - Pokretanje: Pre pokretanja alata potrebno je napraviti izmene u CMakeLists.txt na način definisan u *2024_Analysis_12-Uno/patches/qt_fix.patch* fajlu. U direktorijumu *2024_Analysis_12-Uno/codeChecker* pokrenuti ```./run_codeChecker.sh``` skript fajl.
- **3. lcov**
-    - Instalacija: ```sudo apt-get install lcov```
    - Pokretanje: Pre pokretanja alata potrebno je napraviti izmene u CMakeLists.txt na način definisan u *2024_Analysis_12-Uno/patches/lcov_cmake_flags.patch* fajlu. U direktorijumu *2024_Analysis_12-Uno/lcov* pokrenuti ```./run_lcov.sh``` skript fajl.
- **4. Memcheck**
    - Instalacija: ```sudo apt-get install valgrind```
    - Pokretanje: Pre pokretanja alata potrebno je napraviti izmene u CMakeLists.txt na način definisan u *2024_Analysis_12-Uno/patches/memcheck_cmake_flags.patch* fajlu. U direktorijumu *2024_Analysis_12-Uno/valgrind/memcheck* pokrenuti ```./run_memcheck.sh``` skript fajl. U istom direktorijumu, fajl *qt.supp* se može izmeniti pre pokretanja alata tako da se dodaju/obrišu pravila za potiskivanje grešaka


## Zaključak
Ključni rezultati analize ukazuju da je neophodno napisati dodatne testove koji bi povećali pokrivenost koda i time testirali veći broj komponenti sistema. Prilikom implementacije testova potrebno je više pažnje posvetiti curenju memorije koja nastaje tokom njihovog izvršavanja. Dodatno je potrebno izbegavati korišćenje default-nog konstruktora u testovima koji porede objekte, kako ne bi dolazilo do nedefinisog ponašanja koje prouzrokuje pad testova.

Ovim radom pokazana je značajnost alata za verifikaciju softvera, čak i kada program naizgled funkcioniše ispravno. Detaljan izveštaj i zaključci mogu se pronaći u fajlu [ProjectAnalysisReport.pdf](\\\\).
