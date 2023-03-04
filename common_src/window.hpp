#pragma once

#include "util.hpp"

enum PacketStatus {
    empty, // there is no pkt in this slot
    waiting, // pkt was added
    finished, // pkt is finished, but earlier packets might not be finished
};

class Window {
public:
    Window(unsigned short startSeqNum);
    void addPacket(struct packet pkt);
    void finishPacket(unsigned short seqNum);
    bool isFull();
    bool packetFitsInWindow(unsigned short seqNum, int len);

private:
    struct packet packets[WND_SIZE];
    PacketStatus status[WND_SIZE] = {empty};
    unsigned short startSeqNum, endSeqNum;

    int getPacketIdx(unsigned short seqNum);
    int getEmptySlot();
    int getIdxOfEarliestPacket();
    void shiftBySeqNums(unsigned short deltaSeqNum);
    void shiftToFirstWaiting();
};
