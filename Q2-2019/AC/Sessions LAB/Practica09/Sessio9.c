#include <stdio.h>

int main(){

	double dx4 = 665857.0;
	double dy2 = 470832.0;

	double z1 = dx4*dx4*dx4*dx4 - 4*(dy2*dy2*dy2*dy2) - 4*(dy2*dy2);	

	float sx4 = 665857.0;
	float sy2 = 470832.0;

	float z2 = sx4*sx4*sx4*sx4 - 4*(sy2*sy2*sy2*sy2) - 4*(sy2*sy2);

	long long llx4 = 665857.0; 
	long long lly2 = 470832.0;

	long long z3 = llx4*llx4*llx4*llx4 - 4*(lly2*lly2*lly2*lly2) - 4*(lly2*lly2); 

	printf("Double precision: ");
      	printf("%f \n", z1);
        printf("Single precision: ");
        printf("%f \n", z2);
        printf("long long: ");
        printf("%lld \n", z3);
}
