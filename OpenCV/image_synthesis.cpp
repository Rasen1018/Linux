#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	Mat image1= imread("./lake.png", IMREAD_UNCHANGED);
	Mat image2= imread("./box.jpg", IMREAD_UNCHANGED);
	CV_Assert(!(image1.empty() || image2.empty()));

	double alpha = 0.6, beta = 0.7;
	Mat add_img1 = image1 + image2;
	Mat add_img2 = image1 * 0.5 + image2 * 0.5;
	Mat add_img3 = image1 * alpha + image2 * (1 - alpha);
	Mat add_img4;
	addWeighted(image1, alpha, image2, beta, 0, add_img4);

	imshow("Image1", image1); imshow("Image2", image2);
	imshow("add_img1", add_img1), imshow("add_img2", add_img2);
	imshow("add_img3", add_img3), imshow("add_img4", add_img4);

	// fwrite(image, sizeof(unsigned char), imageSize, fp);

	waitKey(0);
	return 0;
}
