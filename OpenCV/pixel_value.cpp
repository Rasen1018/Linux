#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	Mat image = imread("./sample.jpg", IMREAD_GRAYSCALE);
	if (image.empty())
	{
		cout << "can not read a image" << endl;
		exit(1);
	}

	Rect roi(135, 95, 20, 15);
	Mat roi_img = image(roi);
	cout << "[roi_img] =" << endl;

	for (int i = 0; i < roi_img.rows; i++) {
		for (int j = 0; j < roi_img.cols; j++) {
			cout.width(5);
			cout << (int)roi_img.at<uchar>(i, j);
		}
		cout << endl;
	}
	rectangle(image, roi, Scalar(255), 1);
	imshow("Image", image);
	waitKey();

	return 0;
}
