#include "./hoffmanCode.h"

int main(){
    char * str1 = "csdvfsvcxscvsddv  cs";
    HoffManTree* tree = enCodeHoffMan(str1);
    char * str2 = deCodeHoffMan(tree);
    printf("%s\n",str2);
    for(int i = 0 ; i < strlen(str1); i ++){
        if(str1[i] != str2[i]){
            printf("error\n");
            break;
        }
    }
    printf("success\n");
}