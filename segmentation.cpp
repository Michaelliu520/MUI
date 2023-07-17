#include "segmentation.h"


Segmentation::Segmentation()
{
//    _frame = NULL;
}
Segmentation::~Segmentation(){

}

void Segmentation::init(){

}

ncnn::Mat Segmentation::pre_handle_pic(cv::Mat &image){
    cv::Size newSize(960, 540); // 新的图像大小
    cv::resize(image, image, newSize);
    int img_w=image.cols;
    int img_h=image.rows;

    int w = img_w;
    int h = img_h;
    ncnn::Mat input = ncnn::Mat::from_pixels_resize(image.data, ncnn::Mat::PIXEL_BGR2RGB, img_w, img_h, w, h);
    return input;
}

cv::Mat Segmentation::last_handle_pic(ncnn::Mat& image){
    // 处理推理结果
    // 将ncnn::Mat对象转换为OpenCV的Mat对象
    cv::Mat outputImage(image.h, image.w, CV_32FC1, image.data);
    return  outputImage;
}

cv::Mat Segmentation::segmentation_mobilenetv2(cv::Mat& image){
    // 创建NCNN推理实例
    ncnn::Net mobilenetv2;
    ncnn::Extractor ex = mobilenetv2.create_extractor();
    mobilenetv2.load_param("M_Hu-CoC_E500.opt.net"); // 加载模型的参数
    mobilenetv2.load_model("M_Hu-CoC_E500.opt.weight");   // 加载模型的权重
    ex = mobilenetv2.create_extractor();
    ncnn::Mat image_input = pre_handle_pic(image);
    ncnn::Mat output;
    ex.input("input", image_input);
    ex.extract("output", output);
    return  last_handle_pic(output);
}
void Segmentation::receiveFrame(const cv::Mat& frame){
    _frame = frame;
    qDebug()<<"segmentation frame";
}

void Segmentation::run(){
    while(true){
        qDebug()<<"segmentation start";
//        _frame = segmentation_mobilenetv2(_frame);
    }
}
