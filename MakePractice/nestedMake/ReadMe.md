# Result

## Result-1
```
$ make dist
rm -f tooltip-1.0.tar.gz
mkdir -p tooltip-1.0/src
cp Makefile tooltip-1.0
cp src/Makefile src/main.c tooltip-1.0/src
tar chof - tooltip-1.0 | gzip -9 -c > tooltip-1.0.tar.gz
rm -rf tooltip-1.0

$ tree
.
├── build
├── Makefile
├── Makefile.1st
├── src
│   ├── main.c
│   ├── Makefile
│   ├── Makefile.1st
│   └── tooltip
└── tooltip-1.0.tar.gz

2 directories, 7 files
```

# Modifying prefix from command line
```
make prefix=../build2/ install
OR
make prefix=$(pwd)/build2 install

So here, build2 was already available and, with this installed binary into build2 folder.
Check below tree command result:
$ tree
.
├── build
├── build2
│   └── build
│       └── tooltip
├── Makefile
├── Makefile.1st
├── Makefile.2nd
├── ReadMe.md
└── src
    ├── main.c
    ├── Makefile
    ├── Makefile.1st
    ├── Makefile.2nd
    └── tooltip

4 directories, 10 files

```