#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""CPAC prototype vest controller."""

import struct
import time
import threading
import itertools
import struct

import numpy as np
import torch

from can import Message
from can.bus import BusState
from can.interface import Bus

from plotting import Plotter

DECIMATION = 32
BATCH_SIZE = 256
N_FEATURES = 6  # gyro_x, gyro_y, gyro_z, accel_x, accel_y, accel_z


class DummyVest:
    def __init__(self):
        self.sender = threading.Thread(target=self.send_loop)
        self.sender.daemon = True
        self.sender.start()

    def send_loop(self):
        msg = Message(arbitration_id=0x20, is_extended_id=False, data=[0] * 8)
        with Bus(
            interface="vector",
            app_name="CPAC",
            channel=1,
            bitrate=1000000,
            fd=False,
        ) as bus:

            for counter in itertools.count():
                msg.arbitration_id = 0x20
                msg.data = struct.pack("<Q", counter)
                bus.send(msg)
                msg.arbitration_id = 0x21
                msg.data = struct.pack("<Q", counter)
                bus.send(msg)
                # time.sleep(0.001)


def collect_batch(bus):
    """Collect a full batch of data from the CAN bus."""
    batch = np.empty((BATCH_SIZE, N_FEATURES))
    for i_sample in range(BATCH_SIZE):
        for _ in range(2):
            msg = bus.recv()
            if msg.arbitration_id == 0x20:
                (
                    batch[i_sample][3],
                    batch[i_sample][4],
                    batch[i_sample][5],
                    _,
                ) = struct.unpack("<hhhh", msg.data)
            if msg.arbitration_id == 0x21:
                (
                    batch[i_sample][0],
                    batch[i_sample][1],
                    batch[i_sample][2],
                    _,
                ) = struct.unpack("<hhhh", msg.data)
    return batch


def load_model():
    """Load and return trained PyTorch model."""
    model = torch.jit.load("model_scripted.pt")
    model.eval()
    return model


def main():
    #dummy = DummyVest()

    model = load_model()

    csv_name = time.strftime("trial_%Y%m%d-%H%M%S.csv")

    with Bus(
        interface="vector",
        app_name="CPAC",
        channel=0,
        bitrate=1000000,
        fd=False,
    ) as bus, open(csv_name, "w") as csv_file:

        csv_file.write("gyro_x,gyro_y,gyro_z,accel_x,accel_y,accel_z,mode\n")

        def cb_close(event):
            exit()

        def cb_key(event):
            if event.key == "up":
                msg = Message(
                    arbitration_id=0x80,
                    is_extended_id=False,
                    data=[2, 0, 0, 0, 0, 0, 0, 0],
                )
                bus.send(msg, 0.1)
            elif event.key == "down":
                msg = Message(
                    arbitration_id=0x80,
                    is_extended_id=False,
                    data=[3, 0, 0, 0, 0, 0, 0, 0],
                )
                bus.send(msg, 0.1)

        plotter = Plotter(cb_close=cb_close, cb_key=cb_key)

        while True:
            batch = collect_batch(bus)
            # print(batch[-1][0])
            y = model(torch.from_numpy(batch[np.newaxis, ...]).float()).view(-1).detach()
            soft_preds = torch.sigmoid(y).detach().numpy()

            all_data = np.append(batch, soft_preds[..., np.newaxis], axis=1)
            np.savetxt(csv_file, all_data, delimiter=",", fmt='%.4e')

            plot_data = all_data.reshape(DECIMATION, -1, N_FEATURES + 1).mean(axis=0)
            #print(plot_data)
            plotter.update(plot_data)


            if plot_data[-1][-1] > 0.95:
                msg = Message(
                    arbitration_id=0x80,
                    is_extended_id=False,
                    data=[3, 0, 0, 0, 0, 0, 0, 0],
                )
                bus.send(msg, 0.1)
            else:
                msg = Message(
                    arbitration_id=0x80,
                    is_extended_id=False,
                    data=[2, 0, 0, 0, 0, 0, 0, 0],
                )
                bus.send(msg, 0.1)



if __name__ == "__main__":
    main()
