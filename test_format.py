#!/usr/bin/env python3

"""
For the purpose of CS118 Winter 2023 Project 2 output result check
Usage: ./test_format.py output_file or python3 test_format.py output_file
"""

import sys
import re

all_lines = None
output_pattern = re.compile(
    r'(?P<event>\w+) (?P<seq>\d+)\s?(?P<ack>\d+)?\s?(?P<syn_flag>SYN)?\s?(?P<fin_flag>FIN)?\s?(?P<ack_flag>ACK)?\s?(?P<dup_ack_flag>DUP-ACK)?')

my_isn = None
server_isn = None
my_fin = None
server_fin = None


def get_filename() -> str:
    if len(sys.argv) < 2:
        print(
            "Usage:", sys.argv[0], "output_file_name \n", file=sys.stderr)
        exit(-1)
    return sys.argv[1]


def check_format(all_lines) -> bool:
    handshake_case1 = False
    handshake_case2 = False
    fin_case1 = False
    fin_case2 = False
    global my_isn, server_isn, my_fin, server_fin
    for line in all_lines:
        if len(line) == 0:
            continue
        result = output_pattern.match(line)
        result = result.groupdict()

        # SEND SYN, get ISN
        # Test case: Client initiates three-way handshake by sending a SYN packet with correct values in its header
        if result['event'] == 'SEND' and result['syn_flag'] is not None:
            my_isn = int(result['seq'])
            print('Client ISN: ', my_isn)
            handshake_case1 = True

        # RECV SYN ACK, get ISN
        # Test case: Server responses with SYN-ACK packet
        if result['event'] == 'RECV' and result['syn_flag'] is not None and result['ack_flag'] is not None:
            server_isn = int(result['seq'])
            print('Server ISN: ', server_isn)
            handshake_case2 = True

        # SEND FIN
        # Test case: Client sends a FIN packet after transmitting a file
        if result['event'] == 'SEND' and result['fin_flag'] is not None:
            my_fin = int(result['seq'])
            fin_case1 = True

        # RECV FIN
        # Test case: Server replies with a FIN packet after reciving the FIN
        if fin_case1 and result['event'] == 'RECV' and result['fin_flag'] is not None:
            server_fin = int(result['seq'])
            fin_case2 = True

    if handshake_case1 and handshake_case2 and fin_case1 and fin_case2:
        return True
    return False


if __name__ == "__main__":
    with open(get_filename()) as f:
        all_lines = f.readlines()

    check_result = check_format(all_lines)
    print("Check result: ", check_result)