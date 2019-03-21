# LinUM
Shared repostory for LinUM Lab 6
En uppdaterad version av koden har bifogats med denna inlämning.  Den
enda biten som ändrats är Makefilen.


# Uppdatering
Tidigare hade vi en `make_local`-target som specificerade en sökväg till
`*.so`-filerna som var lokal i katalogen med kod.  Vid en vanlig `make`
så användes en hårdkodad `-rpath`-sökväg för `electrotest` baserad på
`$LIB_INSTDIR` (`/usr/local/lib` per default).

Vad vi ändrat på är att vi nu använder `$ORIGIN` som bas för rpath.
Detta ersätter `make_local` i och med att vi nu kan använda en relativ
sökväga för `*.so-filerna`:

```sh
~/repos/LinUM$ ldd bin/electrotest
[...]
libcomponent.so => /home/usr/repos/LinUM/bin/../lib/libcomponent.so (0x00007fc812055000)
libpower.so => /home/usr/repos/LinUM/bin/../lib/libpower.so (0x00007fc811e53000)
libresistance.so => /home/usr/repos/LinUM/bin/../lib/libresistance.so (0x00007fc811c51000)
[...]
```

Och vid installation:

```sh
~/repos/LinUM$ DESTDIR=$HOME/a/b/c/d/e make install
~/repos/LinUM$ cd ~/a
~/a$ ldd b/c/d/e/bin/electrotest
[...]
libcomponent.so => /home/usr/a/b/c/d/e/bin/../lib/libcomponent.so (0x00007fb376444000)
libpower.so => /home/usr/a/b/c/d/e/bin/../lib/libpower.so (0x00007fb376242000)
libresistance.so => /home/usr/a/b/c/d/e/bin/../lib/libresistance.so (0x00007fb376040000)

~/a$ readelf -d b/c/d/e/bin/electrotest |grep RUNPATH
 0x000000000000001d (RUNPATH)            Library runpath: [$ORIGIN/../lib]
```

Nackdelen med detta tillvägagångssätt är att vi nu måste använda
ytterligare en byggkatalog -- nämligen `/bin`.  Detta beror på att vi
måste skapa en katalogstruktur som motsvarar katalogstrukturen vid
installation.  Det vill säga, `electrotest` måste placeras så att
biblioteken finns i syskonkatalogen `lib` -- vilket ej är fallet om
`electrotest` byggs i `./`.



Del 2.

Uppdateringen i den här kompletterande inlämningen består endast av ändringar i Makefile. Istället för en hårdkodad absolut
sökäg till de dynamiska biblioteken som ska länkas in så har vi infört variablen ORIGIN, se nedan, som innebär att det blir
en relativ sökväk till biblioteksfilerna, beroende på var ifrån binären exekveras. För att få den relativa sökvägen till 
biblioteken att fungera likadant för både ett lokalt bygge som för installation av binärerna så har en ny underkatalog skapats,
bin, där binärfilen sparas för ett lokalt bygge.

Bibliotek och huvudprogram byggdes och länkades var för sig, och för biblioteken länkades dessa till en .so fil med liknande 
kommando och växlar som i del 1 ovan, men .so filen placerades i en underkatalog, lib.
För att länka ihop samtliga bibliotek och huvudprogrammet användes följande kommando:
$(CC) -o $(BIN_BUILDDIR)/$(TARGET) electrotest.o -L./lib -lcomponent -lpower -lresistance -Wl,-rpath,\$$ORIGIN/../lib
Där olika delar och flaggor betyder:
$(CC)                        - gcc kompilerare
-o $(BIN_BUILDDIR)/$(TARGET) - Outputfil är namnet på target, vilket är electrotest och placeras i underkatalogen bin
electrotest.o                - Namnet på huvudprogrammets objektfil
-L./lib                      - Pekar ut var biblioteken finns som ska länkas in i programmet, i det här fallet i katalogen lib
-lcomponent                  - libcomponent.so som länkas med
-lpower                      - libpower.so som länkas med
-lresistance                 - libresistance.so som länkas med
-Wl,-rpath,\$$ORIGIN/../lib  - -Wl betyder att en option skickas till länkaren, i detta fall -rpath, som betyder att de dynamiska 
                               biblioteken kommer att finnas i följande biblioteket. $ORIGIN sätts automatiskt till binärens katalog
                               och vid körning så söks biblioteken i ../lib relativt till sökvägen för $ORIGIN.
                               \\$ i ORIGIN betyder att variabeln inte expanderas vid make, utan den skickas med till 
                               binärens dynamiska sektion och expanderas vid körning.

