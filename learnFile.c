#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main() {
        FILE* fp; 
        fp = fopen("tem.txt", "r"); 
        if(fp==NULL) { 
              printf("Error in reading\n"); 
              return 0; 
        } 
        char buf[100]; 
        while(fscanf(fp,"%s", buf)==1) { 
		uint8_t t = atoi(buf);
		printf("%d\n", t);
	} 
 
        return 0; 
}

