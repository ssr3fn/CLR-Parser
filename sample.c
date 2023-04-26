#include<stdio.h>

float func2(int s, float y){
	return (s+y)*3;
}

int main(){
	int y = 4;
	int x = 5;
	float z;
	read z;
	print x+z;

	if(x < z){
		print z;
	}
	else{
		print x;
	}
	float r = func2(y,z);
	print r;
	return 0;
}
