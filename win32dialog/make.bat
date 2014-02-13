mkdir tmp
windres -J rc -O coff -i resource.rc -o tmp\resource.res
mingw32-g++ -Wall -O2 -c win32dialog.cpp -o tmp\win32dialog.o
mingw32-g++ -o win32dialog.exe  tmp\win32dialog.o tmp\resource.res -s  -lgdi32 -luser32 -lkernel32 -lcomctl32 -mwindows
pause
