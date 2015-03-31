#!/bash/bin
rm 12345
gcc -o server server.c server_func.c
./server 12345