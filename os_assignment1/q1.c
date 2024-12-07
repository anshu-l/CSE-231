#include <stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>

int child(){
    int sum = 0;

    srand(getpid()); // Seed the random number generator

    for (int i = 0; i < 5; i++) {
        int randomNumber = rand()%100+1; 
        printf("%d \n", randomNumber);
        sum += randomNumber; // Add random number to sum
    }

    printf("average is: %d\n", sum/4); // Print the average
    exit(0);
    
    
}
int main(){
    for(int i=1; i<=7;i++){
        int resultfork=fork();
        
        if (resultfork<0){
            printf("ERROR");
        }
        else if(resultfork==0){
            // x
            child();


        }
        else{
            wait(NULL);
        }



    }

return 0;
}
