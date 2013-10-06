#!/usr/bin/env python
import pyudev
import subprocess

def get_serial():
    con = pyudev.Context()
    for dev in con.list_devices( subsystem = "usb", ID_MODEL="Ruggeduino"):
        return dev["ID_SERIAL_SHORT"]

serial_file = open("serials.csv", "a")

while True:
    part_number = raw_input("Enter part number: ")
    serial = get_serial()
    subprocess.call(["make", "flash"])
    serial_file.write("{0},{1}\n".format(part_number.upper(), serial))
