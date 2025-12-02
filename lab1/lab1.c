#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"

int grid[3][3] = {{1,2,3},{4,5,6},{7,8,9}};

void print_grid(int r,int c){
    printf("\n");
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++)
            if(i==r && j==c) printf(GREEN "%d " RESET,grid[i][j]);
            else printf("%d ",grid[i][j]);
        printf("\n");
    }
    printf("\n");
}

int handle_move(char in[],int *r,int *c){
    if(strcmp(in,"up")==0 && *r>0) (*r)--;
    else if(strcmp(in,"down")==0 && *r<2) (*r)++;
    else if(strcmp(in,"left")==0 && *c>0) (*c)--;
    else if(strcmp(in,"right")==0 && *c<2) (*c)++;
    else if(strcmp(in,"enter")==0) return 0;
    else { printf(RED "Invalid!\n" RESET); return -1; }
    return 1;
}

int lock(){
    printf("Navigation: up, down, left, right, enter\n");
    int r=1,c=1,ans=grid[r][c];
    char in[20];
    print_grid(r,c);
    while(1){
        scanf("%s",in);
        int s=handle_move(in,&r,&c);
        if(s==0) break;
        if(s==1){
            ans=ans*10+grid[r][c];
            print_grid(r,c);
            printf("Password: " GREEN "%d\n" RESET,ans);
        }
    }
    return ans;
}

void unlock(int pwd){
    printf("Navigation: up, down, left, right, enter\n");
    int r=1,c=1,ans=grid[r][c];
    char in[20];
    print_grid(r,c);
    while(1){
        scanf("%s",in);
        int s=handle_move(in,&r,&c);
        if(s==0) break;
        if(s==1){
            ans=ans*10+grid[r][c];
            print_grid(r,c);
            printf("Password: " GREEN "%d\n" RESET,ans);
        }
    }
    if(ans==pwd) printf(GREEN "Correct Password\n" RESET);
    else printf(RED "Incorrect Password\n" RESET);
}

int main(){
    printf("\nLOCK\n");
    int pwd=lock();
    printf("\nUNLOCK\n");
    unlock(pwd);
    return 0;
}
