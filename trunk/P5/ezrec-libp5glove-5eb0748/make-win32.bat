gcc -O3 -I.\ -I.\src .\src\usb_hid_win32.c .\src\p5glove.c .\demo\p5dump.c -lsetupapi -L.\win32 -lhid -o p5dump.exe

gcc -O3 -I.\ -I.\src .\src\usb_hid_win32.c .\src\p5glove.c .\demo\p5common.c .\demo\p5hand.c -lgl -lglu -lglut -lsetupapi -L.\win32 -lhid -o p5hand.exe
