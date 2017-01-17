#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>


using namespace std;

int main(int argc, char* argv[]){

ifstream infile;
ofstream outfile;
string numbers;
string series[7][600];
int y = 0;
int x = 0;

infile.open(argv[1], ios::in);
outfile.open("stringOut.txt", ios::out);

getline(infile,numbers);

for(int x = 0; x < 6; x++){
	for(int y = 0; y < 600; y++){
	series[x][y] = "null";
	}
}

while(infile){

x = 0;


infile >> numbers;

if(static_cast<int>(infile.peek()) == -1){
break;
}

series[x][y] = numbers;
infile >> numbers;
series[x][y] += numbers;
infile >> numbers;
series[x][y] += numbers;

for(int x = 1; x < 6; x++){
infile >> numbers;
series[x][y] = numbers;
infile >> numbers;
series[x][y] += numbers;
}
infile >> numbers;
series[6][y] = numbers;
series[6][y] += ",";
y++;
}
y--;
series[6][y] = numbers;

for(int x = 1; x < 6; x++){
series[x][y] = series[x][y][0];
}


for(int x = 0; x < 7; x++){
outfile << x << "{";
	for(int y = 0; y < 600; y++){
	if(strcmp(series[x][y].c_str(), "null"))
	outfile << series[x][y];
	}
outfile << "}" << endl;
}

infile.close();
outfile.close();

return 0;
}
