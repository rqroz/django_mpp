#include <bits/stdc++.h>

#define ld double
#define debug cout<<"!!!\n"
using namespace std;

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
void extrai_pontos(){
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

bool ler_arquivo(){
	char path[] = "/home/rqroz/Desktop/polygon_finder/polygons/website/opencv/points.txt";
	string line;
	ifstream points_file(path);
	if(points_file.is_open()){
		while (getline(points_file, line)){
			size_t pos = line.find(" ");
			string px = line.substr(0, pos);
			string py = line.substr(pos+1);
			int x = atoi(px.c_str()), y = atoi(py.c_str());
			M[x][y]= 1;
			n++;
		}
		points_file.close();
	}
	else cout << "Could not open the file to read points...\n";
	return n >= 8;
}
void imprime_arquivo(){
	char path[] = "/home/rqroz/Desktop/polygon_finder/polygons/website/opencv/pointsfile.txt";
	ofstream points_file(path);
	if(points_file.is_open()){
		for(int i=0; i<poly.size(); i++)
			points_file << poly[i].x << " " << poly[i].y << endl;
		points_file.close();
	}
	else cout << "Could not open the file to write points...\n";


}
bool ler_padrao(){
	cin >> n;
	for(int i=0, x, y; i<n; i++){
		cin >> x >> y;
		x--; y--;
		M[x][y]= 1;
	}
	for(int i=0; i<MAXN; i++){
		for(int j=0; j<MAXN; j++){
			if(M[i][j]==1) cout << "#";
			else cout << " ";
		}
		cout << endl;
	}
	return n>=8;
}

void imprime_padrao(){
	cout << "size(Poly): " << poly.size() << endl;
	for(int i=0; i<poly.size(); i++) cout << "(" << poly[i].x << ", " << poly[i].y << ")\n";
	//for(int i=0; i<v.size(); i++) cout << "(" << v[i].x+1. << ", " << v[i].y+1. << ")\n";
}
bool entrada_valida(){
	for(int i=0; i<MAXN; i++){
		for(int j=0; j<MAXN; j++){
			if(M[i][j]==0) continue;
			int cont=0, x, y;
			for(int k=0; k<4; k++){
				x= i + dx[k];
				y= j + dy[k];
				if(inside(x,y) && M[x][y]!=0) cont++;
			}

			if(cont != 2)
				return false;
		}
	}

	return true;
}
int main(){
	//if(ler_padrao() == false) return 0;
	if(ler_arquivo() == false) return 0;
	
	extrai_pontos();
	if(entrada_valida())
		mpp();
	imprime_arquivo();
	//imprime_padrao();
}
