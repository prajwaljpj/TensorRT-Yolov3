import os
import sys
import struct
import subprocess
from multiprocessing import Process
# import threading
# import time
import errno
import cv2
from boundbox import Box, wrap_box


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
        videocap = cv2.VideoCapture("latest.mp4")
        if (videocap.isOpened() == False):
            print("python side ::::::::: Error opening video stream or file")
        out = cv2.VideoWriter('outpy.avi',cv2.VideoWriter_fourcc('M','J','P','G'), 20, (1280, 720))

        while(videocap.isOpened()):
            ret, frame = videocap.read()
            if ret == False:
                # print("python side ::::::::: ********videocap return gave false*******")
                break

            try:
                head = os.read(fifo, 1)
                print("python side ::::::::: value of head in try :: ", head)
                # head = sys.stdin.read(1)
            except:
                print("python side ::::::::: some error :: end??")
                sys.exit(0)

            head = int.from_bytes(head, "big")
            print("python side ::::::::: header length(number of boxes): ", head)
            # print("pythonasdfadf aasdf a asdf asdf asdfasdfasdaffasdf")
            # print(data_byte.decode("utf-8"))
            print("python side ::::::::: frame_number :::::::::::::: ", frame_number)
            frame_number += 1
            # print("python side ::::::::: data_byte :::::::::::::: ", data_byte)
            # print("python side ::::::::: data_byte length :::::::::::::: ", len(data_byte))
            if head == 0:
                print("python side ::::::::: data :::::::::::::: DATA IS EMPTY")
                continue

            for i in range(head):
                data_byte = os.read(fifo, 24)
                print("python side :::::::::::: data_byte value :::::: ", data_byte)
                print("python side :::::::::::: data_byte value :::::: ",
                      len(data_byte))
                # if len(data_byte[i*24:24+(i*24)]) != 24:
                #     print("python side ::::::::: BYTE CONFLICT OCCURED +++++ TRYING TO WAIT IN PYTHON SIZE+++++")
                #     # time.sleep(0.5)
                # try:
                data = struct.unpack("=iiiiif", data_byte)

                print("python side ::::::::: data :::::::::::::: ", data)
                data = Box(data)
                frame = wrap_box(frame, data)
            out.write(frame)
        videocap.release()
        out.release()
    #             except:
    #                 print("python side ::::::::: IGNORING BYTE CONFLICT +++++++++ CHECK THE DATA AFTER THIS+++++++")
    #                 exit(0)
    # # string = fifo.read()

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
