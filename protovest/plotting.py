import time
from matplotlib import pyplot as plt
import numpy as np


class Plotter:

    def __init__(self, history=1000, cb_close=None, cb_key=None):
        self.t = np.linspace(1, history, num=history)
        self.fig = plt.figure(figsize=(10, 8))
        if cb_close is not None:
            self.fig.canvas.mpl_connect('close_event', cb_close)
        if cb_key is not None:
            self.fig.canvas.mpl_connect('key_press_event', cb_key)

        self.ax_accel = self.fig.add_subplot(3, 1, 1)
        self.ax_gyro = self.fig.add_subplot(3, 1, 2)
        self.ax_clutch = self.fig.add_subplot(3, 1, 3)

        self.accel_x_buffer = np.zeros(history)
        self.accel_y_buffer = np.zeros(history)
        self.accel_z_buffer = np.zeros(history)

        self.accel_x, = self.ax_accel.plot(self.accel_x_buffer, lw=3, label="X")
        self.accel_y, = self.ax_accel.plot(self.accel_y_buffer, lw=3, label="Y")
        self.accel_z, = self.ax_accel.plot(self.accel_z_buffer, lw=3, label="Z")
        self.ax_accel.legend()
        self.ax_accel.title.set_text("Accelerometer")
        self.ax_accel.set_xlim(self.t[0], self.t[-1])
        self.ax_accel.set_ylim([-20, 20])
        self.ax_accel.grid()

        self.gyro_x_buffer = np.zeros(history)
        self.gyro_y_buffer = np.zeros(history)
        self.gyro_z_buffer = np.zeros(history)

        self.gyro_x, = self.ax_gyro.plot(self.gyro_x_buffer, lw=3, label="X")
        self.gyro_y, = self.ax_gyro.plot(self.gyro_y_buffer, lw=3, label="Y")
        self.gyro_z, = self.ax_gyro.plot(self.gyro_z_buffer, lw=3, label="Z")
        self.ax_gyro.legend()
        self.ax_gyro.title.set_text("Gyroscope")
        self.ax_gyro.set_xlim(self.t[0], self.t[-1])
        self.ax_gyro.set_ylim([-130, 130])
        self.ax_gyro.grid()


        self.clutch_buffer = np.zeros(history)

        self.clutch, = self.ax_clutch.plot(self.clutch_buffer, "r", lw=3, label="soft")
        self.ax_clutch.legend()
        self.ax_clutch.title.set_text("Clutch control")
        self.ax_clutch.set_xlim(self.t[0], self.t[-1])
        self.ax_clutch.set_ylim([0.0, 5.0])
        self.ax_clutch.grid()

        plt.tight_layout()
        self.fig.canvas.draw()

        self.bg_accel = self.fig.canvas.copy_from_bbox(self.ax_accel.bbox)
        self.bg_gyro = self.fig.canvas.copy_from_bbox(self.ax_gyro.bbox)
        self.bg_clutch = self.fig.canvas.copy_from_bbox(self.ax_clutch.bbox)

        plt.show(block=False)

    def update(self, batch):
        batch_size = batch.shape[0]

        self.accel_x_buffer[:-batch_size] = self.accel_x_buffer[batch_size:]
        self.accel_x_buffer[-batch_size:] = batch[:, 3]
        self.accel_y_buffer[:-batch_size] = self.accel_y_buffer[batch_size:]
        self.accel_y_buffer[-batch_size:] = batch[:, 4]
        self.accel_z_buffer[:-batch_size] = self.accel_z_buffer[batch_size:]
        self.accel_z_buffer[-batch_size:] = batch[:, 5]

        self.gyro_x_buffer[:-batch_size] = self.gyro_x_buffer[batch_size:]
        self.gyro_x_buffer[-batch_size:] = batch[:, 0]
        self.gyro_y_buffer[:-batch_size] = self.gyro_y_buffer[batch_size:]
        self.gyro_y_buffer[-batch_size:] = batch[:, 1]
        self.gyro_z_buffer[:-batch_size] = self.gyro_z_buffer[batch_size:]
        self.gyro_z_buffer[-batch_size:] = batch[:, 2]

        self.clutch_buffer[:-batch_size] = self.clutch_buffer[batch_size:]
        self.clutch_buffer[-batch_size:] = batch[:, 6]

        self.accel_x.set_data(self.t, self.accel_x_buffer)
        self.accel_y.set_data(self.t, self.accel_y_buffer)
        self.accel_z.set_data(self.t, self.accel_z_buffer)

        self.gyro_x.set_data(self.t, self.gyro_x_buffer)
        self.gyro_y.set_data(self.t, self.gyro_y_buffer)
        self.gyro_z.set_data(self.t, self.gyro_z_buffer)

        self.clutch.set_data(self.t, self.clutch_buffer)

        # restore background
        self.fig.canvas.restore_region(self.bg_accel)
        self.fig.canvas.restore_region(self.bg_gyro)
        self.fig.canvas.restore_region(self.bg_clutch)

        # redraw just the points

        self.ax_accel.draw_artist(self.accel_x)
        self.ax_accel.draw_artist(self.accel_y)
        self.ax_accel.draw_artist(self.accel_z)

        self.ax_gyro.draw_artist(self.gyro_x)
        self.ax_gyro.draw_artist(self.gyro_y)
        self.ax_gyro.draw_artist(self.gyro_z)

        self.ax_clutch.draw_artist(self.clutch)

        # fill in the axes rectangle
        self.fig.canvas.blit(self.ax_accel.bbox)
        self.fig.canvas.blit(self.ax_gyro.bbox)
        self.fig.canvas.blit(self.ax_clutch.bbox)

        self.fig.canvas.flush_events()
