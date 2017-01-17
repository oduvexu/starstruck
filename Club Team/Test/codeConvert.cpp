#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>


using namespace std;

int main(int argc, char* argv[]){

ifstream infile;
ofstream outfile;
string numbers, command;
string series[7][650];
int z = 0;
int y = 0;
int x = 0;
int row = 0;

infile.open(argv[1], ios::in);
outfile.open("stringOut.txt", ios::out);

getline(infile,numbers);

for(int x = 0; x < 6; x++){
	for(int y = 0; y < 650; y++){
	series[x][y] = "null";
	}
}

while(infile){
x = 0;


infile >> numbers;

if(static_cast<int>(infile.peek()) == -1){
break;
}
infile >> numbers;
infile >> numbers;
series[x][y] = numbers;
numbers = "";
z = 0;

while(1){
if(series[x][y][z] == ',')
break;
numbers += series[x][y][z];
z++;
}

series[x][y] = numbers; 

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
row++;
}

y--;
series[6][y] = numbers;

for(int x = 1; x < 6; x++){
series[x][y] = series[x][y][0];
}


for(int x = 0; x < 7; x++){
switch (x){

	case 1:
	command = "signed byte _record_FL[";
	break;
	case 2:
	command = "signed byte _record_FR[";
	break;
	case 3:
	command = "signed byte _record_BR[";
	break;
	case 4:
	command = "signed byte _record_BL[";
	break;
	case 5:
	command = "signed byte _record_ARM[";
	break;
	case 6:
	command = "signed byte _record_CLAW[";
	break;
	}

outfile << x << " " << command << row << "]{";
	for(int y = 0; y < 650; y++){
	if(strcmp(series[x][y].c_str(), "null"))
	outfile << series[x][y];
	}
outfile << "};" << endl;
}

infile.close();
outfile.close();

return 0;
}
