#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void print_locateROI(String str, Mat m)
{
	Size size;
	Point pt;
	m.locateROI(size, pt);

	cout << "parents size" << size << ",";
	cout << "[" << str << "] =";
	cout << m.size() << " from" << pt << endl;
}

int main() 
{
	Range r1(0, 3), r2(3, 7);
	int data[] = {
		10, 11, 12, 13, 14, 15, 16,
		20, 21, 22, 23, 24, 25, 26,
		30, 31, 32, 33, 34, 35, 36,
		40, 41, 42, 43, 44, 45, 46,
	};

	Mat m1(5, 7, CV_32S, data);
	Mat m2 = m1(r1, r2);
	Mat m3 = m1(r1, r1);

	print_locateROI("m2", m2);
	print_locateROI("m3", m3);

	m2.adjustROI(-1, 1, 2, -1);
	m3.adjustROI(0, -1, -2, 2);

	cout << "[m1 : no.2 row] =" << m1.row(2) << endl;
	cout << "[m1 : no.1 column] =" << endl << m1.col(1) << endl;
	cout << "[m1 : all row] =" << endl << m1.rowRange(r1) << endl << endl;
	cout << "[m1] =" << endl << m1 << endl << endl;
	cout << "[m2 (0~2 row 3~6 column) ref] =" << endl << m2 << endl << endl;
	m2.setTo(50);
	cout << "[m2] =" << endl << m2 << endl;
	cout << "[m1] =" << endl << m1 << endl << endl;
	
	return 0;
}
