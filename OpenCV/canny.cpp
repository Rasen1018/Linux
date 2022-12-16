#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void calc_direct(Mat Gy, Mat, Gx, Mat& direct)
{
	direct.create(Gy.size(), CV_8U);

	for (int i = 0; i < direct.rows; i++) {
		for (int j = 0; j < direct.cols; j++) {
			float gx = Gx.at<float>(i, j);
			float gy = Gy.at<float>(i, j);
			int theat = int(fastAtan2(gy, gx) / 45);
			direct.at<uchar>(i, j) = theat % 4;
		}
	}

}

void supp_nonMax(Mat sobel, Mat direct, Mat& dst)
{
	
}

void trace(Mat max_so, Mat& pos_ck, Mat& hy_img, Point pt, int low)
{
	Rect rect(Point(0, 0), pos_ck.size());
	if (!rect.contains(pt)) return;

	if (pos_ck.at<uchar>(pt) == 0 && max_so.at<float>(pt) > low)
	{
		pos_ck.at<uchar>(pt) = 1;
		hy_img.at<uchar>(pt) = 255;

		trace(max_so, pos_ck, hy_img, pt + Point(-1, -1), low);
		trace(max_so, pos_ck, hy_img, pt + Point(0, -1), low);
		trace(max_so, pos_ck, hy_img, pt + Point(1, -1), low);
		trace(max_so, pos_ck, hy_img, pt + Point(-1, 0), low);

		trace(max_so, pos_ck, hy_img, pt + Point(1, 0), low);
		trace(max_so, pos_ck, hy_img, pt + Point(-1, 1), low);
		trace(max_so, pos_ck, hy_img, pt + Point(0, 1), low);
		trace(max_so, pos_ck, hy_img, pt + Point(1, 1), low);
	}
}

int main()
{
	Mat image = imread("./lake.png", 0);
	CV_Assert(image.data);
	Mat gau_img, Gx, Gy, direct, sobel, max_sobel, hy, img, canny;

	GaussianBlur(image, gau_img, Size(5, 5), 0.3);
	Sobel(gau_img, Gx, CV_32F, 1, 0, 3);
	Sobel(gau_img, Gy, CV_32F, 0, 1, 3);
	sobel = abs(Gx) + abs(Gy);

	calc_direct(Gy,Gx, direct);
	supp_nonMax(sobel, direct, max_sobel);
	hysteresis_th(max_sobel, hy_img, 100, 150);
	Canny(image, canny, 100, 150);

	imshow("Image", image);
	imshow("canny", hy_img);
	imshow("OpenCV_canny", canny);
	waitKey();

	return 0;
	
}
