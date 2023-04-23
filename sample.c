#include<stdio.h>

void func2(int s, float y){
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
	int res = func2(y,z);
	print res;
	return 0;
}
