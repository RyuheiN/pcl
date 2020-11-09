#include <stdio.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
using namespace std;

//型
struct pt_t {
    float x, y, z;
    pt_t() { ; }
    pt_t(float _x, float _y, float _z) {
        x = _x;
        y = _y;
        z = _z;
    }
};

//split関数
vector<string> split(string& input, char delimiter)
{
    istringstream stream(input);
    string field;
    vector<string> result;
    while (getline(stream, field, delimiter)) {
        result.push_back(field);
    }
    return result;
}

#define RGB(r, g, b) ((int(r) << 16) + (int(g) << 8) + int(b) )


int main(){

    ifstream ifs("/Users/ryuhei/Documents/PCL/maptest.csv");
    
    vector<pt_t> pts;
    string line;

    //一行ずつ読み込み,でsplit
    while (getline(ifs, line)) {
        
        vector<string> s = split(line, ',');

        pt_t pt(stof(s.at(0)),stof(s.at(1)),stof(s.at(2)));

        pts.push_back(pt);   
    }
    //***********************************************************
    // pcdファイル（バイナリ形式）に出力
    FILE *fp = fopen("/Users/ryuhei/Documents/PCL/maptest.pcd", "wb");
    // ヘッダ
    fprintf(fp, "# .PCD v.7 - Point Cloud Data file format\n");
    fprintf(fp, "VERSION .7\nFIELDS x y z rgb\nSIZE 4 4 4 4\nTYPE F F F U\nCOUNT 1 1 1 1\n");
    fprintf(fp, "WIDTH %lu\nHEIGHT 1\nVIEWPOINT 0 0 0 1 0 0 0\nPOINTS %lu\nDATA binary\n", pts.size(), pts.size());
    // 中身をバイナリで書き込み
    for (int i = 0; i < pts.size(); i++) {
        // 適当に色を付ける
        unsigned int col = RGB(pts[i].z * 5, 128 + 3*pts[i].x, 128 + 3 * pts[i].y);
        float xyz[3] = { pts[i].x, pts[i].y, pts[i].z};
        fwrite( xyz, sizeof(float), 3, fp);
        fwrite(&col, sizeof(unsigned int), 1, fp);
    }
    fclose(fp);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);

    //読み込み
    if (pcl::io::loadPCDFile<pcl::PointXYZ> ("/Users/ryuhei/Documents/PCL/maptest.pcd", *cloud) == -1) //* load the file
    {
        PCL_ERROR ("Couldn't read file test_pcd.pcd \n");
        return (-1);
    }

    pcl::io::savePCDFileASCII ("/Users/ryuhei/Documents/PCL/maptest.pcd", *cloud);

}