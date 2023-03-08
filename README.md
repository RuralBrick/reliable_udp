# CS118 Project 2

Reliable data transport implemented over UDP.
* Quan Do, biquando@g.ucla.edu, 105490012
* Theodore Lau, teddy1405@g.ucla.edu, 405462550

## High-Level Design

We modified the `while(1)` loop in `server_src/server.cpp` to additionally send
appropriately numbered ACKs and dup-ACKs and log them in its `window`. We
modified `client_src/client.cpp` to
1. keep filling its `window` with data packets until the `window` is either full
   or we reach the end of the file,
2. send the data packets it creates,
3. tell its `window` which packets have been ACKed,
4. and tell its `window` to resend any packets which have timed out.

All other common functionality was factored out into `common_src/`, including
some repeated template code and our `Window` class.

Our `Window` class implements the bulk of our packet management, including
- tracking packet statuses (waiting, ACKed, inaccessible),
- tracking packet timeouts,
- shifting the window,
- and manipulating the circular buffer.

The same `Window` class is used in both `server_src/server.cpp` and
`client_src/client.cpp` to realize Selective Repeat.

## Problems Encountered

Our first "major" problem was not realizing the template code polled the sockets
instead of using blocking, but after noticing it, we were quickly able to adjust
our logic appropriately. Our second major problem was synchronizing the sequence
and ACK numbers between the server and client, especially for the teardown stage
and for handshakes in which packets were lost. We realized we could solve the
problem by simply updating one host's variables based on the latest packets the
other host has sent.

Our third problem involved the contents of our data packets, specifically
concerning the very last data packet. We use `feof()` in `client_src/client.cpp`
to check whether we have already read the entire file we are sending, but it
only returns non-zero after a failed read. Therefore, we would create and send a
packet of payload size 0 after already sending the entire file, which is
incorrect behavior. We fixed it by simply ignoring any size 0 reads.

Our last major problem involved making sure our printouts simply complied with
what the spec and autograder expected, which involved tweaking each of our print
calls slightly until our output matched what was expected.

## Outside Resources

We used no outside resources other than what was presented in class and
discussion.
