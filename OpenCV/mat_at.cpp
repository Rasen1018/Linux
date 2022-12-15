#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	Mat m1(3, 5, CV_32SC1);
	Mat m2(3, 5, CV_32FC1);
	Mat m3(3, 5, CV_8UC2);
	Mat m4(3, 5, CV_32SC3);

	for (int i = 0, k =0; i < m1.rows; i++) {
		for (int j = 0; j < m1.cols; j++, k++) {
			m1.at<int>(i, j) = k;
			Point pt(i, j);
			m2.at<float>(pt) = (float)j;

			int idx[2] = {i, j};
			m3.at<Vec2b>(idx) = Vec2b(0, 1);

			m4.at<Vec3i>(i, j)[0] = 0;
			m4.at<Vec3i>(i, j)[1] = 1;
			m4.at<Vec3i>(i, j)[2] = 2;
		}
	}

	cout << "[m1] = " << endl << m1 << endl;
	cout << "[m2] = " << endl << m2 << endl;
	cout << "[m3] = " << endl << m3 << endl;
	cout << "[m4] = " << endl << m4 << endl;

	return 0;
}
