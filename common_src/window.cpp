#include "util.hpp"
#include "window.hpp"

/* Public methods */

Window::Window(unsigned short startSeqNum)
    : startSeqNum(startSeqNum),
      endSeqNum(startSeqNum + WND_SIZE * PAYLOAD_SIZE)
{}

void Window::addPacket(struct packet pkt) {
    if (!packetFitsInWindow(pkt.seqnum, pkt.length)) {
        printError("tried to add pkt %u, but it doesn't fit in the window", pkt.seqnum);
    }
    if (getPacketIdx(pkt.seqnum) >= 0) {
        printError("tried to add pkt %u, but it's already in the window", pkt.seqnum);
    }
    if (isFull()) {
        printError("tried to add pkt %u, but the window is full", pkt.seqnum);
    }

    int idx = getEmptySlot();
    packets[idx] = pkt;
    status[idx] = waiting;
}

// Marks the given packet for removal
void Window::finishPacket(unsigned short ackNum) {
    for (int i = 0; i < WND_SIZE; i++) {
        if (status[i] == waiting && (packets[i].seqnum + packets[i].length) % MAX_SEQN == ackNum) {
            status[i] = finished;
            shiftToFirstWaiting();
            return;
        }
    }
    printError("tried to remove pkt ackNum=%u, but it isn't in buffered", ackNum);
}

bool Window::isFull() {
    return getEmptySlot() < 0;
}

bool Window::isEmpty() {
    for (PacketStatus s : status) {
        if (s != empty) return false;
    }
    return true;
}

bool Window::packetFitsInWindow(unsigned short seqNum, int len) {
    unsigned short beg = seqNum;
    unsigned short end = (beg + len) % MAX_SEQN;

    if (startSeqNum > endSeqNum) {  // if the window overflows past MAX_SEQN
        return (beg >= startSeqNum || beg < endSeqNum)
            && (end > startSeqNum || end <= endSeqNum)
            && !(beg < endSeqNum && end > startSeqNum);
    }

    return beg >= startSeqNum
        && end <= endSeqNum
        && beg < end;
}

/* Private methods */

// if there is a packet in the window with the given seqNum, return its index
// returns -1 if the packet isn't in the window
int Window::getPacketIdx(unsigned short seqNum) {
    for (int i = 0; i < WND_SIZE; i++) {
        if (status[i] != empty && packets[i].seqnum == seqNum) {
            return i;
        }
    }
    return -1;
}

// returns idx of the first empty slot
// returns -1 if there aren't any empty spots (i.e. buffer is full)
int Window::getEmptySlot() {
    for (int i = 0; i < WND_SIZE; i++) {
        if (status[i] == empty) return i;
    }
    return -1;
}

// returns -1 if the buffer is empty
int Window::getIdxOfEarliestPacket() {
    int earliestIdx = -1;
    for (int i = 0; i < WND_SIZE; i++) {
        if (status[i] == empty) continue;
        if (earliestIdx == -1) {
            earliestIdx = i;
            continue;
        }

        int diffCurrent = (int)(packets[earliestIdx].seqnum) - (int)startSeqNum;
        int diffNew = (int)(packets[i].seqnum) - (int)startSeqNum;

        if ((diffNew >= 0 && diffCurrent < 0) ||
            (diffNew >= 0 && diffNew < diffCurrent) ||
            (diffCurrent < 0 && diffNew < diffCurrent)) {
            earliestIdx = i;
            continue;
        }
    }
    return earliestIdx;
}

void Window::shiftBySeqNums(unsigned short deltaSeqNum) {
    startSeqNum = (startSeqNum + deltaSeqNum) % MAX_SEQN;
    endSeqNum = (endSeqNum + deltaSeqNum) % MAX_SEQN;
}

void Window::shiftToFirstWaiting() {
    int nextIdx = getIdxOfEarliestPacket();
    while (nextIdx != -1) {
        if (status[nextIdx] == waiting) return;

        // if the earliest packet is finished, we can remove it from the buffer
        shiftBySeqNums(packets[nextIdx].length);
        status[nextIdx] = empty;
        nextIdx = getIdxOfEarliestPacket();
    }
}
