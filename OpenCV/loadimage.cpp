#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main()
{
	Mat image = imread("sample.jpg", IMREAD_COLOR);

	imshow("Load Image", image);
	waitKey(3000);

	return 0;
}
