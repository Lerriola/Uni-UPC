#include <stdio.h>

int main(int argc, int argv[]) {
	
	double x = 665857.0;
	double y = 470832.0;
	
	double res = (x*x*x*x) - 4*(y*y*y*y) - 4*(y*y);
	
	printf("La x val: %.10f i la y val: %.10f\n", x, y);
	
	printf("Z double = %lf\n", res);
	
	float x2 = 665857.0;
	float y2 = 470832.0;
	//printf("Entra la X i la Y:");
	//scanf("%f %f", &x, &y);
	
	float res2 = (x2*x2*x2*x2) - 4*(y2*y2*y2*y2) - 4*(y2*y2);
	
	printf("La x val: %.10f i la y val: %.10f\n", x2, y2);
	
	printf("Z float = %f\n", res2);
	
	long long  x3 = 467857.0;
	long long  y3 = 8970832.0;

	long long  res3 = (x3*x3*x3*x3) - 4*(y3*y3*y3*y3) - 4*(y3*y3);
	
	printf("La x val: %lld i la y val: %lld \n", x3, y3);
	
	printf("Z int = %lld\n", res3);
	

	
	return 0;
}
