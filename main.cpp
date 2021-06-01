
#include <opencv2/opencv.hpp>

int main() {
	
	cv::VideoCapture cap;
	cap = cv::VideoCapture(0);
	int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
	int frame_heigth = cap.get(CAP_PROP_FRAME_HEIGHT);
 	bool sun_status;
	while(true) {
		cv::Mat image,gray;
		cap >> image;

		if (image.empty()) {
			break;
		}
		
		sun_status = isSunlight(image);
		cv::putText(image, std::to_string(sun_status), cv::Point2f(20,20), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0,255,0),2,8,false);
		

		cv::imshow("SunDetector",image);
		char c = (char) waitKey(25);
		if(c == 27 || c == 32){
			break;
		}
	}

	return 0;
}


bool isSunlight(cv::Mat image) {
    std::vector<int> histogram[256];

    for (int i = 0; i < 255; i++) {
        histogram[i] = 0;
    }

    for (int y = 0; y < image.rows; y++)
        for (int x = 0; x < image.cols; x++)
            histogram[(int) image.at<uchar>(y, x)]++;

    for (int i = 0; i < 256; i++)
        std::cout << histogram[i] << " ";
s
    int hist_w = 1920;
    int hist_h = 1080;
    int bin_w = cvRound((double) hist_w / 256);

    cv::Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));

    int max = histogram[0];
    for (int i = 1; i < 256; i++) {
        if (max < histogram[i]) {
            max = histogram[i];
        }
    }

    for (int i = 0; i < 255; i++) {
        histogram[i] = ((double) histogram[i] / max) * histImage.rows;
    }

    for (int i = 0; i < 255; i++) {
        line(histImage, Point(bin_w * (i), hist_h),
             Point(bin_w * (i), hist_h - histogram[i]),
             Scalar(0, 0, 0), 1, 8, 0);
    }