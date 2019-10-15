import os
import sys
import struct
import subprocess
import cv2 as cv
from multiprocessing import Process
import threading
import time
import errno


def setBbox():
    cmd = "/home/rbccps2080/projects/prajwal/test/TensorRT-Yolov3/build/runYolov3"
    subprocess.Popen(cmd)


def getBbox():

    path = "/tmp/fifopipe"

    try:
        os.mkfifo(path)
    except OSError as oe:
        if oe.errno != errno.EEXIST:
            raise

    fifo = os.open(path, os.O_RDONLY)
    frame_number = 1

    while(True):
        try:
                head = os.read(fifo, 1)
        except:
                print("some error :: end??")
                
        head = int.from_bytes(head, "big")
        print("header length(number of boxes): ", head)
        data_byte = os.read(fifo, head*24)
        print("python side ::::::::: frame_number :::::::::::::: ", frame_number)
        frame_number+=1
        # print(data_byte)
        # print(len(data_byte))
        for i in range(head):
                data = struct.unpack("=iiiiif", data_byte[i*24:24+(i*24)])
                print(data)
    # string = fifo.read()
    


    # box_count = int(len(string) / 24)
    # print("box count: ", box_count)

    # box = []

    # for i in range(box_count):
    #     tup = struct.unpack('=iiiiif', string[i*24:24+(i*24)])
    #     box.append(tup)

    # print(box)
    # fifo.close()
    # # os.remove(path)

# img_path = "/home/rbccps2080/projects/prajwal/test/TensorRT-Yolov3/toydata/frame_84time_3.65217391304.jpg"
# image = cv.imread(img_path)


if __name__ == "__main__":

    p1 = Process(target=setBbox)
    p2 = Process(target=getBbox)

    p1.start()
    # time.sleep(2)
    p2.start()

    p1.join()
    p2.join()
    # getBbox()
