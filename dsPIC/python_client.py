#!/usr/bin/env python

"""
Example code to work with Vector VN1630A
Requirements:
- Vector Driver
- python-can (pip install python-can)
"""

import can
import struct
from can.bus import BusState


def loop():

    with can.interface.Bus(
        interface="vector", channel=0, bitrate=1000000, fd=False) as bus:


        cnt = 0
        send = can.Message(arbitration_id = 0x80, is_extended_id=False, data=[2,0,0,0,0,0,0,0])
        try:
            while True:
                msg = bus.recv(1)
                if msg is not None and len(msg.data) == 8:
                    cnt += 1
                    #print(msg)
                    ax, ay, az, enc = struct.unpack("<hhhh", msg.data)
                    if cnt % 100 == 0:
                        print(msg)
                        print(ax, ay, az, enc)
                        print(send)

                    bus.send(send, 0.1)

                if ax > 550:
                    send = can.Message(arbitration_id = 0x80, is_extended_id=False, data=[3,0,0,0,0,0,0,0])
                elif ax < 450:
                    send = can.Message(arbitration_id = 0x80, is_extended_id=False, data=[2,0,0,0,0,0,0,0])

        except KeyboardInterrupt:
            pass  # exit normally


if __name__ == "__main__":
    loop()
