#Minor 6

##Resources
1. [C Socket Programming](http://www.thegeekstuff.com/2011/12/c-socket-programming/)
2. [SO: Connection Refused error](http://stackoverflow.com/questions/11497716/c-sockets-connection-refused-error)
3. [VIDEO (15 min): UDP Programming in C](https://www.youtube.com/watch?v=Emuw71lozdA)
4. [VIDEO (9 min): TCP/IP Socket in C Language](https://www.youtube.com/watch?v=oP-n5tLUPVQ)

##Assignment
In this assignment you'll be using Linux sockets (two-way communication devices between processes running on a single computer). You will submit ONE solution to this problem for your entire group.

Start by going through the tutorial found here. Along with some discussion it includes code for a client server appllication using sockets. Hey, sounds like you're all done, right? Well, not quite.

So, here's the application that I want you to complete. First off we'll have one server but TWO clients. The server will maintain a "database" of 10 tickets to something, a airplane seat, a conarference, a public hanging, it doesn't matter. Each ticket will have a 5-digit (randomly generated) ticket number. After all, we don't want forged tickets at our hanging. So that's simple enuff. The server will maintain an array of 10 5-digit integers.

Each client will make requests that the server will attempt to satisfy. Here are the types of requests:

* `Buy` --- the server will (hopefully) provide a unique 5-digit ticket number to the client

* `Cancel` --- the server will not "refund" the $ (actually you don't need to deal with money in this assignment at all.) But the client WILL tell the server what ticket number is being cancelled.

Ok, that's pretty simple, but I do want you to handle error cases, like the client trying to return an invalid ticket (in which case the server does nothing), or the server running out of tickets.

So my STRONG suggestion is that you modify the server and single client software that the tutorial provides you with. Again I strongly advise adding function(s) to at least the server.c file and possibly the client as well. Once you have a server and single client model working the 2nd client can be a copy of the first, though you'll have to use a different executable name, of course.

To add some fun each client should probabilistically (using a random number generator)  BUY 90% of the time and CANCEL 10%.  If you don't know how to do this, contact me.  Its simple using "random()".   And to make things fun each client should make 7 requests of the server, which means our "show" will likely be a full house.

##Submission
**This IS important.** Follow these directions or you're likely to get a zero for this assignment. You can submit up to 20 files, though I doubt you'll need to submit anywhere near that number.  All be one of them should be the requisite .c files. The ONE different one should be a "bash" script. The bash script could be just a simple list of linux commands. For example

```bash
gcc -o server server.c
gcc -o client client.c
./server 12345
./client cherios 12345
```

would suffice for the example in the tutorial.

Name the bash script something so that the grader can easily determine which file is the bash script. We'll expect to "run" your program simply by typing

```bash
source YourBashFile
```

And we'll be expecting correct output to pop up on the screen. Do NOT use C file I/O to store the output, just use standard input and output in addition to the command line arguments used by `server.c` and `client.c`

**NOTE:**
I expect that there will be no "strangeness" in the output related to having two clients, but we MIGHT get something. And that would be Grrrrrrrrrrrrrrreat. It would set us up for the next minor assignment. So, don't worry if your program output looks to be "non-deterministic."