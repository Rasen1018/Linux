#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	Mat image = imread("./sonic.png", 0);
	CV_Assert(image.data);

	Scalar avg = mean(image) / 2.0;
	Mat dst1 = image*0.5;
	Mat dst2 = image*2.0;
	Mat dst3 = image*0.5 + avg[0];
	Mat dst4 = image*2.0 - avg[0];

	imshow("image", image);
	imshow("dst1-constrast", dst1), imshow("dst2-constrast", dst2);
	imshow("dst3-constrast by mean", dst3);
	imshow("dst4-constrast by mean", dst4);
	waitKey();

	return 0;
}
