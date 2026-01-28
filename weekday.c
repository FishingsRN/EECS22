/* 	weekday.c: Assignment #1
	
	Author: Richard Nguyen
		
	Modifications:
       	01/09/2026 RN initial version
	01/13/2026 RN modified with comments
*/

#include <stdio.h>

/*Zeller Function - as stated in assignment details (implicit floor) */
int Zeller(int q, int m, int y){
	if (m==1 || m==2){
		m += 12;
		y -= 1;
	}
	int U = y%100;
	int V = y/100;
	int f1 =((m+1)*13)/5; /*seperated into floor blocks */
	int f2 = U/4;
	int f3 = V/4;
	int result = (q+f1+U+f2+f3-2*V)%7; /* full function with all blocks */
	return result;

}

int main(){
	int q, m, y = 0;
	printf("Please enter a calendar date:\n");
	printf("Day, q = ");
	scanf("%d", &q);
	printf("Month, m = ");
	scanf("%d", &m);
	printf("Year, y =  ");
	scanf("%d", &y); /*all input variables taken */
	printf("For the calendar date %d/%d/%d, the day of the week is %d.\n", m,q,y,Zeller(q,m,y)); /* function call and return value of days in int 0-6 for print statement*/
	switch (Zeller(q,m,y)) { /* switch case for last print statement, string for days*/
		case 0: printf("This is a Saturday.\n");
		break;
		case 1: printf("This is a Sunday.\n");
		break;
		case 2: printf("This is a Monday.\n");
		break;
		case 3: printf("This is a Tuesday.\n");
		break;
		case 4: printf("This is a Wednesday.\n");
		break;
		case 5: printf("This is a Thursday.\n");
		break;
		case 6: printf("This is a Friday.\n");
		break;
		default:
		printf("This is an invalid input.");
			
	}
	
	return 0;
}

/* EOF */
