
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

bool isSunlight(cv::Mat image, int width, int height) {
    
    int histogram[256];

    for (int i = 0; i < 255; i++) {
        histogram[i] = 0;
    }

    for (int y = 0; y < image.rows; y++)
        for (int x = 0; x < image.cols; x++)
            histogram[(int) image.at<uchar>(y, x)]++;

    for (int i = 0; i < 256; i++)
        std::cout << histogram[i] << " ";

    int hist_w = width;
    int hist_h = height;
    std::cout  << "Width :" << hist_w << std::endl;
    std::cout  << "height :" << hist_h << std::endl;
    
    std::cout  << "Width 2 :" << image.cols << std::endl;
    std::cout  << "height 2 :" << image.rows << std::endl;
    int bin_w = cvRound((double) hist_w / 256);

    cv::Mat histImage(hist_h, hist_w, CV_8UC1, cv::Scalar(255, 255, 255));

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
        line(histImage, cv::Point(bin_w * (i), hist_h),
             cv::Point(bin_w * (i), hist_h - histogram[i]),
             cv::Scalar(0, 0, 0), 1, 8, 0);
    }

    int totPixeles = width * height;
    int numWhite = max;
    float percentageWhites = (numWhite * 100) / totPixeles;
    std::cout << "Whites :" << percentageWhites << std::endl;
    if (percentageWhites > 5) {
        return true;
    } else {
        return false;
    }

}

int main() {
	
	cv::VideoCapture cap;
	cap = cv::VideoCapture("/home/fcs10/1.jpg");
	int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	int frame_heigth = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
 	bool sun_status;
	while(true) {
		cv::Mat image,gray;
		cap >> image;

		if (image.empty()) {
			break;
		}
		
		sun_status = isSunlight(image,frame_width, frame_heigth);
		cv::putText(image, std::to_string(sun_status), cv::Point2f(20,20), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0,255,0),2,8,false);
		std::cout << "Sun status: " << sun_status << std::endl;


		cv::imshow("SunDetector",image);
		char c = (char) cv::waitKey(25);
		if(c == 27 || c == 32){
			break;
		}
	}

	return 0;
}


