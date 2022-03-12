// #include<bits/stdc++.h>
/*
    Những thư viện cần dùng, nếu muốn nhanh có thể sử dụng #include<bits/stdc++.h>
*/
#include<iostream>
#include<vector>
#include<map>
#include<iomanip>
#include<limits.h>
#include<fstream>
using namespace std;
typedef struct CK_t{
    string id;
    double open;
    double close;
}CK;
int n, choice = -1;
vector<CK*> vec;
map<string, double> hmap;
string id;
double open, close;
CK *ck;
void menu(){
    cout << "1. Đọc file\n";
    cout << "2. Tìm kiếm theo mã chứng khoán\n";
    cout << "3. Tìm kiếm những mã chứng khoán có xu hướng tăng\n";
    cout << "4. Tìm mã có số ngày tăng lớn nhất\n";
    cout << "5. Thoát chương trình\n";
}
void f1(char *input){
    ifstream inp(input);
    inp >> n;
    for(int i=0; i<10*n; i++){
        inp >> id >> open >> close;
        ck = new CK;
        ck->id = id;
        ck->open = open;
        ck->close = close;
        vec.push_back(ck);
    }
    for(vector<CK*>::iterator it = vec.begin(); it != vec.end(); it++){
        ck = *it;
        hmap[ck->id] += (ck->close - ck->open)/10;
    }
    cout << fixed;
    cout << setprecision(3);
    for(map<string, double>::iterator it = hmap.begin(); it!= hmap.end(); it++){
        cout << it->first << ": " << it->second << endl;
    }
    inp.close();
}
void f2(){
    double max = (double)INT_MIN;
    double min = (double)INT_MAX;
    cout << "Nhập mã cổ phiếu: ";
    cin >> id;
    if(hmap[id]){
        for(vector<CK*>::iterator it = vec.begin(); it != vec.end(); it++){
            ck = *it;
            if(!id.compare(ck->id)){
                if(max < ck->close) max = ck->close;
                if(min > ck->close) min = ck->close;
            }
        }
        cout << "Giá đóng cửa cao nhất: " << max << endl;
        cout << "Giá đóng cửa thấp nhất: " << min << endl;
    }
    else{
        cout << "Không tìm thấy\n";
    }
}
void f3(){
    map<string,int> mp;
    int count = 0, max = 0;
    for(vector<CK*>::iterator it = vec.begin(); it != vec.end(); it++){
        if(count == 2*n) break;
        ck = *it;
        if(ck->close - ck->open > 0){
            mp[ck->id]++;
        }
        count++;
    }
    for(map<string,int>::iterator it = mp.begin(); it != mp.end(); it++){
        if(max < it->second) max = it->second;
    }
    for(map<string,int>::iterator it = mp.begin(); it != mp.end(); it++){
        if(max == it->second) cout << it->first << " ";
    }
    cout << endl;
}
void f4(){
    map<string,int> mp;
    int max = 0;
    for(vector<CK*>::iterator it = vec.begin(); it != vec.end(); it++){
        ck = *it;
        if(ck->close - ck->open > 0){
            mp[ck->id]++;
        }
    }
    for(map<string,int>::iterator it = mp.begin(); it != mp.end(); it++){
        if(max < it->second) max = it->second;
    }
    cout << "Số ngày tăng lớn nhất: " << max << endl;
    for(map<string,int>::iterator it = mp.begin(); it != mp.end(); it++){
        if(max == it->second) cout << it->first << " ";
    }
    cout << endl;
}
void freeVec(){
    for(vector<CK*>::iterator it = vec.begin(); it != vec.end(); it++){
        CK *tmp = *it;
        delete tmp;
    }
}
int main(int argc, char* argv[]){
    while(choice != 5){
        menu();
        cout << "Nhập vào lựa chọn của bạn: ";
        cin >> choice;
        if(choice == 1) f1(argv[1]);
        else if(choice == 2) f2();
        else if(choice == 3) f3();
        else if(choice == 4) f4();
        else if(choice == 5) cout << "Phan Minh Anh Tuấn - 20205227\n";
    }
    freeVec();
    return 0;
}