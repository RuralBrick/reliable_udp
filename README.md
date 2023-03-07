# CS118 Project 2

Reliable data transport implemented over UDP.
* Quan Do, ### TODO: Put email and UID here ###
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



### TODO: Finish ###
* polling (realizing the template code was using polling)
* numbers (synchronizing seq and ack numbers)
    - (also affected teardown/handshake with lost packets)
* file content (sending empty data packet)
* server/client logs (spec/autograder compliance)

## Outside Resources

### TODO: Finish ###
* 
