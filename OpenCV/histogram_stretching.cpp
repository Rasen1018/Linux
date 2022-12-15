#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void calc_Histo(const Mat& image, Mat& hist, int bins, 
					int image_max = 256);
void draw_histo(Mat hist, Mat &hist_img, Size size = Size(256, 200));

int search_valueIdx(Mat hist, int bias = 0)
{
	for (int i = 0; i < hist.rows; i++) {
		int idx = abs(bias - i);
		if (hist.at<float>(idx) > 0) return idx;
	}
	return -1;
}

int main()
{
	Mat image = imread("./lake.png", 0);
	CV_Assert(!image.empty());
	Mat hist, hist_dst, hist_img, hist_dst_img;
	int histsize = 64, ranges = 256;
	calc_Histo(image, hist, histsize, ranges);

	float bin_width = (float)ranges / histsize;
	int low_value = (int)(search_valueIdx(hist, 0) * bin_width);
	int high_value = (int)(search_valueIdx(hist, hist.rows-1) * bin_width);
	cout << "high_value = " << high_value << endl;
	cout << "low_value = " << low_value << endl;

	int d_value = high_value - low_value;
	Mat dst = (image - low_value) * (255.0 / d_value);

	calc_Histo(dst, hist_dst, histsize, ranges);
	draw_histo(hist, hist_img);
	draw_histo(hist_dst, hist_dst_img);

	imshow("Image", image), imshow("Hist_img", hist_img);
	imshow("dst", dst), imshow("hist_dst_img", hist_dst_img);
	waitKey();
	
	return 0;
}
