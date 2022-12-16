#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void scaling(Mat img, Mat& dst, Size size)
{
	dst = Mat(size, img.type(), Scalar(0));
	double ratioY = (double)size.height / img.rows;
	double ratioX = (double)size.width / img.cols;

	for (int i = 0; i <img.rows; i++) {
		for (int j = 0; j < img.cols; j++)
		{
			int x = (int)(j * ratioX);
			int y = (int)(i * ratioY);
			dst.at<uchar>(y, x) = img.at<uchar>(i, j);
		}
	}
}

void scaling_nearest(Mat img, Mat& dst, Size size)
{
	dst = Mat(size, CV_8U, Scalar(0));
	double ratioY = (double)size.height / img.rows;
	double ratioX = (double)size.width / img.cols;

	for (int i = 0; i < dst.rows; i++) {
		for (int j = 0; j < dst.cols; j++) {
			int x = (int)cvRound(j / ratioX);
			int y = (int)cvRound(i / ratioY);
			dst.at<uchar>(i, j) = img.at<uchar>(y, x);
		}
	}
}

int main()
{
	Mat image = imread("./lake.png", 0);
	CV_Assert(image.data);

	Mat dst1, dst2;
	scaling(image, dst1, Size(1024, 1024));
	scaling_nearest(image, dst2, Size(1024, 1024));

	imshow("image", image);
	imshow("dst1 - forward", dst1);
	imshow("dst2 - nearest kneibourhood", dst2);
	waitKey();

	return 0;
}
