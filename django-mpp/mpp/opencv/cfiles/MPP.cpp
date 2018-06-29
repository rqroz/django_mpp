#include <bits/stdc++.h>
#include <vector>
#include <opencv2/opencv.hpp>

#define ld double
#define debug cout<<"!!!\n"
using namespace std;
using namespace cv;

int cmp(ld x, ld y = 0, ld tol = 0.001) { return (x <= y + tol) ? (x + tol < y) ? -1 : 0 : 1; }
struct point{
	ld x, y;
	point():x(0),y(0){}
	point(ld X, ld Y):x(X),y(Y){}
	int cmp(point a){
        if(::cmp(x,a.x))
            return ::cmp(x,a.x);
        return ::cmp(y,a.y);
    }
    point operator+(point a){ return point(x+a.x, y+a.y); }
    point operator-(point a){ return point(x-a.x, y-a.y); }
    point operator*(ld a){ return point(x*a,y*a); }
    point operator/(ld a){ return point(x/a,y/a); }
    ld operator*(point a){ return x*a.x + y*a.y; }
    ld operator%(point a){ return x*a.y - y*a.x; }
    bool operator<(point a){ return cmp(a)==-1; }
    bool operator==(point a){ return cmp(a)==0; }
    bool operator!=(point a){ return cmp(a)!=0; }
    bool operator<=(point a){ return cmp(a)<=0; }
    point operator[](ld a){ return point(cos(a)*x-sin(a)*y, sin(a)*x+cos(a)*y); }
};

#define MAXN 32
int dx[4]={-1,0,1,0};
int dy[4]={0,-1,0,1};
int M[MAXN][MAXN], n;
vector <point> v, poly;
vector <int> cor;

bool inside(int x, int y){ return x>=0 && y>=0 && x<MAXN && y<MAXN;}
void dfs(int x, int y){
	M[x][y]= 2;
	v.push_back(point(x,y));
	for(int i=0, nx, ny; i<4; i++){
		nx= x + dx[i];
		ny= y + dy[i];
		if(inside(nx, ny) && M[nx][ny]==1)
			dfs(nx, ny);
	}
}

void extract_points(){
	for(int i=0; i<MAXN && v.empty(); i++){
		for(int j=0; j<MAXN && v.empty(); j++){
			if(M[i][j] == 1)
				dfs(i,j);
		}
	}
	n= v.size();

	vector <point> aux;
	for(int i=0; i<n; i++){
		if((v[i]-v[(i-1+n)%n])%(v[i]-v[(i+1)%n]) > 0){
			aux.push_back((v[(i-1+n)%n]+v[(i+1)%n])/2);
			cor.push_back(0); //preto
		}
		if((v[i]-v[(i-1+n)%n])%(v[i]-v[(i+1)%n]) < 0){
			aux.push_back((v[(i-1+n)%n]+v[(i+1)%n])/2);
			cor.push_back(1); //branco
		}
	}
	aux.push_back(aux[0]);

	v= aux;
	n= v.size();
}

void mpp(){
	point vk, vl=v[0], br=v[0], pr=v[0];
	poly.push_back(v[0]);

	int posCur=1, posB=0, posP=0;
	while(posCur < n){
		while(posCur < n){
			vk= v[posCur];
			if(((vl-br)%(vl-vk)) > 0){
				vl= pr= br;
				poly.push_back(vl);
				posCur= posB + 1;
				posP= posB;
				break;
			}
			else if(((vl-pr)%(vl-vk)) >= 0){
				if(cor[posCur]==0){
					posP= posCur;
					pr= v[posP];
				}
				else{
					posB= posCur;
					br= v[posB];
				}
				posCur++;
			}
			else{
				vl= br= pr;
				poly.push_back(vl);
				posCur= posP + 1;
				posB= posP;
			}
		}
	}
}

bool read_from_file(string base_path){
	string path = base_path + "/opencv/points.txt";
	cout << "Reading from: " << path << endl;
	string line;
	ifstream points_file(path.c_str());
	if(points_file.is_open()){
		// cout << "Opened points file.\n";
		while (getline(points_file, line)){
			size_t pos = line.find(" ");
			string px = line.substr(0, pos);
			string py = line.substr(pos+1);
			int x = atoi(px.c_str()), y = atoi(py.c_str());
			M[x][y]= 1;

			n++;
		}
		points_file.close();
		return true;
	}else{
		cout << "Could not open the file...\n";
		return false;
	}
}

bool write_to_file(string base_path){
	string path = base_path + "/opencv/pointsfile.txt";
	cout << "Writing to: " << path << endl;
	ofstream points_file(path.c_str());
	if(points_file.is_open()){
		for(u_int i=0; i<poly.size(); i++)
			points_file << poly[i].x << " " << poly[i].y << endl;
		points_file.close();

		Mat img(320, 320, CV_8UC1, Scalar(255));

		for(int i=0; i<MAXN; i++)
			for(int j=0; j<MAXN; j++)
				if(M[i][j]!=0){
					Point A(j*10-5, i*10-5);
					Point B(j*10+9-5, i*10+9-5);
					rectangle(img, A, B, Scalar(200), CV_FILLED, CV_AA, 0);
				}

		vector<Point> ps;
		for(u_int i=0; i<poly.size(); ++i)
			ps.push_back(Point(10*poly[i].y, 10*poly[i].x));

		for(u_int i=0; i<poly.size() - 1; ++i){
			line(img, ps[i], ps[i+1], Scalar(0), 1, CV_AA, 0);
		}
		line(img, ps[poly.size()-1], ps[0], Scalar(0), 1, CV_AA, 0);

		string save_to = base_path + "/static/mpp/img/opencv/result.png";
		bool saved = imwrite(save_to.c_str(), img);

		cout << "saved: " << saved << endl;
		return saved;
	}else{
		cout << "Could not open the file...\n";
		return false;
	}
}

bool validate_points(){
	for(int i=0; i<MAXN; i++){
		for(int j=0; j<MAXN; j++){

			if(M[i][j]==0) continue;
			int cont=0, x, y;
			for(int k=0; k<4; k++){
				x= i + dx[k];
				y= j + dy[k];
				if(inside(x,y) && M[x][y]) cont++;
			}
			if(cont != 2){
				return false;
			}
		}
	}

	for(int i=0; i<MAXN-1; i++)
		for(int j=0; j<MAXN-1; j++)
			if(M[i][j] && M[i+1][j] && M[i][j+1] && M[i+1][j+1]) return false;

	return true;
}

int main(int argc, const char **argv){
	if(argc < 2){ exit(1); }



	string base_path = argv[1];

	if(!read_from_file(base_path)){
		cout << "Could not read from file with base_path: " << base_path << endl;
		exit(1);
	}

	extract_points();

	if(!validate_points()){
		cout << "Could not validate points." << endl;
		exit(1);
	}

	mpp();

	if(!write_to_file(base_path)){
		cout << "Could not write to file with base_path: " << base_path << endl;
		exit(1);
	}

	return 0;
}
