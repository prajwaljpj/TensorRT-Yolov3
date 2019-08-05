#include "inference.hpp"

using namespace std;

int main()
{
    /* cout << "hello" << endl; */
    vector<cv::Mat> images;
    cv::Mat img_0 = cv::imread("/home/rbccps2080/projects/prajwal/test/TensorRT-Yolov3/toydata/frame_84time_3.65217391304.jpg");
    cv::Mat img_1 = cv::imread("/home/rbccps2080/projects/prajwal/test/TensorRT-Yolov3/toydata/frame_296time_12.8695652174.jpg");
    images.push_back(img_0);
    images.push_back(img_1);
    cout << images.size() << endl;
    cout << images[0].size() << endl;
    cout << images[1].size() << endl;
    
    string enginename = "yolov3_fp32.engine";
    Inference iff;
    iff.loadTRTModel(enginename);
    std::list<vector<Bbox>> op1 = iff.infer(img_0);
    std::list<vector<Bbox>> op2 = iff.infer(img_1);

    auto bbox1 = *op1.begin();
    for(const auto& item : bbox1)
    {
        cv::rectangle(img_0,cv::Point(item.left,item.top),cv::Point(item.right,item.bot),cv::Scalar(0,0,255),3,8,0);
        cout << "class=" << item.classId << " prob=" << item.score*100 << endl;
        cout << "left=" << item.left << " right=" << item.right << " top=" << item.top << " bot=" << item.bot << endl;
    }
    auto bbox2 = *op2.begin();
    for(const auto& item : bbox2)
    {
        cv::rectangle(img_1,cv::Point(item.left,item.top),cv::Point(item.right,item.bot),cv::Scalar(0,0,255),3,8,0);
        cout << "class=" << item.classId << " prob=" << item.score*100 << endl;
        cout << "left=" << item.left << " right=" << item.right << " top=" << item.top << " bot=" << item.bot << endl;
    }
    cv::imwrite("img_0.jpg",img_0);
    cv::imwrite("img_1.jpg",img_1);
    return 0;
}
