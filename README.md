# haskell_data_types

Experience Haskellish philosophy in C++20 (concepts!) 

## Quick start

```
...

```


## A note to myself

```console
(cd examples/ && CC=cc CXX=c++ LD_LIBRARY_PATH=/usr/local/stow/gcc-10.2.0-dist/usr/local/lib64/:$(LD_LIBRARY_PATH) LIB=-L/usr/local/stow/gcc-10.2.0-dist/usr/local/lib64 INC=-Wl,-y,_ZdlPvm /usr/bin/make VERBOSE=1 && ./either)

/usr/local/stow/gcc-10.2.0-dist/usr/local/bin/gcov tests.cpp ../haskell_data_types.hpp -m

(cd tests/ && GCOV=/usr/local/stow/gcc-10.2.0-dist/usr/local/bin/gcov make -B run)
```

