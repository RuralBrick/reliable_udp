#include "util.hpp"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

// Printing Functions: Call them on receiving/sending/packet timeout according
// Section 2.6 of the spec. The content is already conformant with the spec,
// no need to change. Only call them at correct times.
void printRecv(struct packet* pkt) {
    printf("RECV %d %d%s%s%s\n", pkt->seqnum, pkt->acknum, pkt->syn ? " SYN": "", pkt->fin ? " FIN": "", (pkt->ack || pkt->dupack) ? " ACK": "");
}

void printSend(struct packet* pkt, int resend) {
    if (resend)
        printf("RESEND %d %d%s%s%s\n", pkt->seqnum, pkt->acknum, pkt->syn ? " SYN": "", pkt->fin ? " FIN": "", pkt->ack ? " ACK": "");
    else
        printf("SEND %d %d%s%s%s%s\n", pkt->seqnum, pkt->acknum, pkt->syn ? " SYN": "", pkt->fin ? " FIN": "", pkt->ack ? " ACK": "", pkt->dupack ? " DUP-ACK": "");
}

void printTimeout(struct packet* pkt) {
    printf("TIMEOUT %d\n", pkt->seqnum);
}

// You can use this function with formatting, just like printf
void printError(std::string format, ...) {
    va_list ap;
    va_start(ap, format);
    // this just uses styling control codes, not really important
    fprintf(stderr, "\x1b[1m\x1b[31mERROR:\x1b[0m\x1b[1m ");
    vfprintf(stderr, format.c_str(), ap);
    fprintf(stderr, "\x1b[0m\n");
    va_end(ap);
    exit(1);
}
