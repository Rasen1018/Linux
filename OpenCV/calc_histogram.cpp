#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void draw_histo(Mat hist, Mat &hist_img, Size size = Size(256, 200));
void draw_hist_hue(Mat hist, Mat &hist_img, Size size = Size(256, 200));
void calc_Histo(const Mat& image, Mat& hist, int bins, int range_max = 256)
{
	int histSize[] = { bins };
	float range[] = {0, (float)range_max};
	int channels[] = {0};
	const float* ranges[] = {range};

	calcHist(&image, 1, channels, Mat(), hist, 1, histSize, ranges);
}

//if link draw_histogram or hue_histogram to calc_histogram, use main
#if 0
int main()
{
#if 0
	// when linking draw_histogram
	Mat image = imread("./lake.png", IMREAD_GRAYSCALE);
	CV_Assert(!image.empty());

	Mat hist, hist_img;
	calc_Histo(image, hist, 256);
	draw_histo(hist, hist_img);

	imshow("image", image);
	imshow("hist_img", hist_img);
	waitKey();
#else
	// when linking hue_histogram
	Mat image = imread("./sonic.png", 1);
	CV_Assert(!image.empty());

	Mat HSV_img, HSV_arr[3];
	cvtColor(image, HSV_img, COLOR_BGR2HSV);
	split(HSV_img, HSV_arr);

	Mat hue_hist, hue_hist_img;
	calc_Histo(HSV_arr[0], hue_hist, 18, 180);
	draw_hist_hue(hue_hist, hue_hist_img, Size(360, 200));

	imshow("image", image);
	imshow("Hue_hist_img", hue_hist_img);
	waitKey();

#endif
	return 0;
}
#endif
