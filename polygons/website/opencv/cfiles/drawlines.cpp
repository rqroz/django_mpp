#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void populateVectors(vector<int>& vec_x, vector<int>& vec_y){
    char path[] = "/home/rqroz/Desktop/polygon_finder/polygons/website/opencv/points.txt";
    string line;
    ifstream points_file(path);
    if (points_file.is_open()){
      cout << "Opened points file.\n";
      while (getline(points_file, line)){
        size_t pos = line.find(" ");
        string px = line.substr(0, pos);
        string py = line.substr(pos+1);
        int x = atoi(px.c_str()), y = atoi(py.c_str());
        vec_x.push_back(x);
        vec_y.push_back(y);
      }
      points_file.close();
    }
    else cout << "Could not open the file...\n";
}

int main () {
  vector<int> points_x;
  vector<int> points_y;
  populateVectors(points_x, points_y);
  vector<Point> points;
  cout << points_x.size() << endl;
  for(u_int i=0; i<points_x.size(); ++i){
    cout << "x: " << points_x[i] << ", y: " << points_y[i] << endl;
    points.push_back(Point(points_x[i], points_y[i]));
  }
  count << points.size() << endl;
  for(u_int i=0; i<points.size(); ++i){
    cout << points[i] << endl;
  }

  // Mat img(32, 32, CV_8UC1, Scalar(255));
  // fillConvexPoly(img, points, Scalar(0), CV_AA, 0);
  // namedWindow("img", WINDOW_AUTOSIZE);
  // imshow("img", img);
  // imwrite("img.png", img);

  ofstream results_file("/home/rqroz/Desktop/polygon_finder/polygons/website/opencv/result.txt");
  if(results_file.is_open()){
    cout << "\nOpened result file...\n";
    results_file << "10 11\n";
    results_file << "10 12\n";
    results_file << "12 15\n";
    results_file.close();
  }else cout << "Could not open result file...\n";
  return 0;
}
