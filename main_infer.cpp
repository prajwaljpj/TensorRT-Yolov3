#include "inference.hpp"

#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "opencv2/opencv.hpp"



using namespace std;

#if 0

void push_boxed_data(Inference infr, cv::Mat img, int fd)
{
  std::list<vector<Bbox>> op1 = infr.infer(img);
  
  auto bbox1 = *op1.begin(); 

  for(const auto& item : bbox1)  
    {
      cout << "class=" << item.classId << " prob=" << item.score*100 << endl;  
      cout << "left=" << item.left << " right=" << item.right << " top=" << item.top << " bot=" << item.bot << endl;  

      unsigned char* box = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(&item));
      write(fd, box, sizeof(item));
    }  
  cout << "write finished" << endl;

  char delim = 'd';
  cout << "Size of delim::" << sizeof(delim) << endl;
  // write(fd, &delim, sizeof(delim));

}

#endif

int main(int argc, char** argv)
{ 
    cout << "hello" << endl;
    string enginename = "/home/rbccps2080/projects/prajwal/test/TensorRT-Yolov3/yolov3_fp32.engine";
    cout << enginename << endl;
    Inference iff;
    iff.loadTRTModel(enginename);
    int fd;
    const char* myfifo = "/tmp/fifopipe";
    /* create the FIFO (named pipe) */
    mkfifo(myfifo, 0666);
    cout << "Waiting for connection to open\n";
    /* write message to the FIFO */
    fd = open(myfifo, O_WRONLY);
    cout << "Connected\n";
    cv::VideoCapture cap("latest.mp4");

    while (1) {
      // Get list of videos realtime
      // cv::VideoCapture cap("latest.mp4");
      int frame_num = 0;
      // Check if camera opened successfully
      if(!cap.isOpened()){
	cout << "Error opening video stream or file" << endl;
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

	for(const auto& item : op1)  
	  {
	    // cout << "class=" << item.classId << " prob=" << item.score*100 << endl;  
	    // cout << "left=" << item.left << " right=" << item.right << " top=" << item.top << " bot=" << item.bot << endl;  

	    unsigned char* box = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(&item));
	    write(fd, box, sizeof(item));
	  }  
	cout << "write finished" << endl;

      }
      
    }
    close(fd);
    cout << "closed finished" << endl;
    /* remove the FIFO */
    unlink(myfifo);

    cout << "unlink finished" << endl;

    return 0;


#ifdef not_process

    vector<cv::Mat> images; 

    // string filepath = argv[1];
    // cv::Mat img_0 = cv::imread(filepath);
    cv::Mat img_0 = cv::imread("/home/rbccps2080/projects/prajwal/test/TensorRT-Yolov3/toydata/frame_84time_3.65217391304.jpg"); 
    cv::Mat img_1 = cv::imread("/home/rbccps2080/projects/prajwal/test/TensorRT-Yolov3/toydata/frame_296time_12.8695652174.jpg"); 
    images.push_back(img_0); 
    images.push_back(img_1); 
    cout << images.size() << endl; 
      cout << images[0].size() << endl;

      int fd;
      const char* myfifo = "/tmp/fifopipe";
      /* create the FIFO (named pipe) */
      mkfifo(myfifo, 0666);
      cout << "Waiting for connection to open\n";
      /* write message to the FIFO */
      fd = open(myfifo, O_WRONLY);
      cout << "Connected\n";

      for(auto i=0; i<images.size(); i++)
      {
	// push_boxed_data(iff, images[i], fd);
	// cout << "Size of delim::" << sizeof(delim) << endl;
	vector<cv::Mat> image_vect; 
	image_vect.push_back(images[i]);

	std::list<vector<Bbox>> op1 = iff.infer(image_vect);
  
	auto bbox1 = *op1.begin(); 
	int delim = bbox1.size();
	char delim_char = (unsigned char) delim;
	cout << "delim :: number :: " << delim << endl;
	cout << "delim :: sizeof :: " << sizeof(delim_char) << endl;
	write(fd, &delim_char, sizeof(delim_char));

	for(const auto& item : bbox1)  
	  {
	    cout << "class=" << item.classId << " prob=" << item.score*100 << endl;  
	    cout << "left=" << item.left << " right=" << item.right << " top=" << item.top << " bot=" << item.bot << endl;  

	    unsigned char* box = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(&item));
	    write(fd, box, sizeof(item));
	  }  
	cout << "write finished" << endl;

	// char delim = 'd';
	// cout << "Size of delim::" << sizeof(delim) << endl;
	// // write(fd, &delim, sizeof(delim));
      }

    close(fd);
    cout << "closed finished" << endl;
    /* remove the FIFO */
    unlink(myfifo);

    cout << "unlink finished" << endl;
    // cv::imwrite("img_0.jpg",img_0);  
    // cv::imwrite("img_1.jpg",img_1);  
    return 0;
#endif
}


//#if 0
//int main()
//{
//    cout << "hello" << endl;
//     vector<cv::Mat> images; 
//     cv::Mat img_0 = cv::imread("/home/rbccps2080/projects/prajwal/test/TensorRT-Yolov3/toydata/frame_84time_3.65217391304.jpg"); 
//     cv::Mat img_1 = cv::imread("/home/rbccps2080/projects/prajwal/test/TensorRT-Yolov3/toydata/frame_296time_12.8695652174.jpg"); 
//     images.push_back(img_0); 
//     images.push_back(img_1); 
//     cout << images.size() << endl; 
//     cout << images[0].size() << endl;
//     cout << images[1].size() << endl;
//    
//     string enginename = "yolov3_fp32.engine";
//     Inference iff;
//     iff.loadTRTModel(enginename);
//     std::list<vector<Bbox>> op1 = iff.infer(img_0);
//     std::list<vector<Bbox>> op2 = iff.infer(img_1);
//
//     auto bbox1 = *op1.begin(); 
//     for(const auto& item : bbox1)  
//     {  
//         cv::rectangle(img_0,cv::Point(item.left,item.top),cv::Point(item.right,item.bot),cv::Scalar(0,0,255),3,8,0);  
//         cout << "class=" << item.classId << " prob=" << item.score*100 << endl;  
//         cout << "left=" << item.left << " right=" << item.right << " top=" << item.top << " bot=" << item.bot << endl;  
//     }  
//     auto bbox2 = *op2.begin();  
//     for(const auto& item : bbox2)  
//     {  
//         cv::rectangle(img_1,cv::Point(item.left,item.top),cv::Point(item.right,item.bot),cv::Scalar(0,0,255),3,8,0);  
//         cout << "class=" << item.classId << " prob=" << item.score*100 << endl;  
//         cout << "left=" << item.left << " right=" << item.right << " top=" << item.top << " bot=" << item.bot << endl;  
//     }  
//     cv::imwrite("img_0.jpg",img_0);  
//     cv::imwrite("img_1.jpg",img_1);  
//     return 0;  
//}
//#endif
