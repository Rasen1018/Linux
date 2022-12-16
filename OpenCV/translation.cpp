#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void translation(Mat img, Mat& dst, Point pt)
{
	Rect rect(Point(0, 0), img.size());
	dst = Mat(img.size(), img.type(), Scalar(0));

	for (int i = 0; i < dst.rows; i++) {
		for (int j = 0; j < dst.cols; j++)
		{
			Point dst_pt(j, i);
			Point img_pt = dst_pt - pt;
			if (rect.contains(img_pt))
				dst.at<uchar>(dst_pt) = img.at<uchar>(img_pt);
		}
	}
}

int main()
{
	Mat image = imread("./lake.png", 0);
	CV_Assert(image.data);

	Mat dst1, dst2;
	translation(image, dst1, Point(30, 80));
	translation(image, dst2, Point(-80, -50));

	imshow("Image", image);
	imshow("dst1 : (30, 80) move", dst1);
	imshow("dst2 : (-80, -50) move", dst2);
	waitKey();

	return 0;
}
