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
