#pragma once

#include "util.hpp"
#include <stdio.h>

enum PacketStatus {
    empty, // there is no pkt in this slot
    waiting, // pkt was sent, but not yet acked (used by client)
    acked, // pkt is acked, but earlier packets might not be acked
};

class Window {
public:
    Window(unsigned short startSeqNum);
    void addPacket(struct packet pkt);
    void ackPacket(unsigned short seqNum, FILE *fp);
    bool isFull();
    bool isEmpty();
    bool canFitPacket(unsigned short seqNum, int len);
    bool containsPacket(unsigned short seqNum);

private:
    struct packet packets[WND_SIZE];
    PacketStatus status[WND_SIZE] = {empty};
    // TODO: Timers
    unsigned short startSeqNum, endSeqNum;

    int getPacketIdx(unsigned short seqNum);
    int getEmptySlot();
    int getIdxOfEarliestPacket();
    void shiftBySeqNums(unsigned short deltaSeqNum);
    void shiftPastAckedPackets(FILE *fp);
};
