import os
import sys
import struct 
import subprocess
import cv2 as cv
from multiprocessing import Process
import threading
import time


def setBbox():
	cmd = "/home/rbccps2080/projects/prajwal/test/TensorRT-Yolov3/build/runYolov3"
	subprocess.Popen(cmd)


def getBbox():

	path= "/tmp/fifopipe"

	fifo=open(path,'rb')
	string=fifo.read()

	print("len of string::", len(string))
	print("len of string::", len(string))
	print("len of string::", len(string))

	box_count = int(len(string) / 24)
	print("box count: ", box_count)
	print("box count: ", box_count)

	box = []

	for i in range(box_count):
		tup = struct.unpack('=iiiiif', string[i*24:24+(i*24)]) 
		box.append(tup)

	print(box)
	fifo.close()
        #os.remove(path)

# img_path = "/home/rbccps2080/projects/prajwal/test/TensorRT-Yolov3/toydata/frame_84time_3.65217391304.jpg"
# image = cv.imread(img_path)

if __name__ == "__main__":

	# p1 = Process(target=setBbox)
	p2 = Process(target=getBbox)# 

	# p1.start()
	# time.sleep(2)
	p2.start()

	# p1.join()
	p2.join()
        #getBbox()
