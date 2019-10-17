#include "inference.hpp"

#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "opencv2/opencv.hpp"



using namespace std;

int main(int argc, char** argv)
{ 
    cout << "C++ side ::::::::: hello" << endl;
    string enginename = "/home/rbccps2080/projects/prajwal/test/TensorRT-Yolov3/yolov3_fp32.engine";
    cout << enginename << endl;
    Inference iff;
    iff.loadTRTModel(enginename);
    int fd;
    const char* myfifo = "/tmp/fifopipe";
    /* create the FIFO (named pipe) */
    mkfifo(myfifo, 0666);
    cout << "C++ side ::::::::: Waiting for connection to open\n";
    /* write message to the FIFO */
    fd = open(myfifo, O_WRONLY);
    cout << "C++ side ::::::::: Connected\n";
    cv::VideoCapture cap("latest.mp4");

    while (1) {
      // Get list of videos realtime
      // cv::VideoCapture cap("latest.mp4");
      int frame_num = 0;
      // Check if camera opened successfully
      if(!cap.isOpened()){
	cout << "C++ side ::::::::: Error opening video stream or file" << endl;
	return -1;
      }
      while(1){
 
	cv::Mat frame;
	// Capture frame-by-frame
	cap >> frame;
  
	// If the frame is empty, break immediately
	if (frame.empty())
	  break; 

	cout << "C++ side ::::::::: frame_num ::::::::: " << frame_num << endl;
	frame_num++;

	vector<Bbox> op1 = iff.infer_single_image(frame);
	frame.release();

	// int delim = op1.size();
	char delim_char = (unsigned char) op1.size();
	// cout << "delim :: number :: " << op1.size() << endl;
	// cout << "delim :: sizeof :: " << sizeof(delim_char) << endl;
	write(fd, &delim_char, sizeof(delim_char));
	unsigned char total_box[24*op1.size()];

	for(const auto& item : op1)  
	  {
	    cout << "C++ side ::::::::: class=" << item.classId << " prob=" << item.score*100 << endl;  
	    cout << "C++ side ::::::::: left=" << item.left << " right=" << item.right << " top=" << item.top << " bot=" << item.bot << endl;  

	    // unsigned char* box = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(&item));
	    unsigned char* box = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(&item));
	    // total_box += box;
	    // strcat(total_box, box);
	  }  
	write(fd, total_box, sizeof(24*op1.size()));
	cout << "C++ side ::::::::: write finished" << endl;

      }
      
    }
    close(fd);
    cout << "C++ side ::::::::: closed finished" << endl;
    /* remove the FIFO */
    unlink(myfifo);

    cout << "C++ side ::::::::: unlink finished" << endl;

    return 0;

}
