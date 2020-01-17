//
//  rsaproject.c
//  
//
//  Created by 김유민 on 2019/11/21.
//

#include "rsaproject.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


//두 수 Num1 과 Num2 사이의 랜덤수를 구해야 하는 경우의 함수
long long randomRange(long num1, long num2){
    srand(time(0));
    return (long long) (rand() % num2) + num1;
}

//0부터 시작하여 랜덤한 수를 구해야 하는 경우의 함수
long long longRandomRange(long long N){
    srand(time(0));
    return rand() % N;
}

//miller Rabin test
char * millerRabinTest(long pBar,int lambda){
    long pBar2 = pBar - 1;
    long r = 0;
    long s=0;
    
    while(pBar2 % 2 == 0){
        pBar2 /= 2;
        r++;
    }
    
    s = pBar2;
    
    for(int i=1;i<=lambda;i++){
        long long a = randomRange(2,pBar-2);
        unsigned long long z = 1;
        
        for(int i=0;i<s;i++){
            z *= a;
            z %= pBar;
        }
    
        if( z != 1 && z != pBar - 1){
            for(int j=1; j<r; j++){
                z = z * z % pBar;
                if( z == 1 ){
                    return "pBar is composite";
                }
            }
            if( z != pBar - 1)
                return "pBar is composite";
        }
    }
    return "pBar is prime";
}

//oiler Pie 함수 사용하기
long oilerPie(long p,long q){
    return (p - 1) * (q - 1);
}

//euclidean 함수
long euclidean(long pieN,long i){
   if(pieN == 0)
       return i;
    return euclidean(i % pieN , pieN);
}

//extended euclidean 함수
long extendedEuclidean(long e, long pieN){
    long secretKey;
    
    for(long i = 1; i<pieN; i++){
        if((e*i) % pieN ==   1){
            secretKey = i;
        }
    }
    
    return secretKey;
}

//encryption 함수. pk = (N,e) / sk = d를 받아서 실행
long encryption(long long N, long e, long long M){
    
    //modular exponentiation : g^a mod p / m^e mod n
    
    int result[30];
    long r = 1;
    
    for(int i=0; 0<e; i++){
        result[i] = e % 2;
        e /= 2;
    }
    
    for(int k = 0; k < sizeof(result); k++){
        r = (r * r) % N;
        if(result[k] == 1)
            r = (r * M) % N;
    }
    
    printf("Encryption C : %ld \n",r);
    return r;
}

//decryption 함수. N,secretKey(d) , C 를 받아서 실행
long decryption(long long N, long secretKey, long C){
    
    int binary[30];
    
    long r = 1;
    for(int p=0;0<secretKey;p++){
        binary[p] = secretKey % 2;
        secretKey /= 2;
    }
    
    for(int idx=0; idx<sizeof(binary);idx++){
        r = (r * r) % N;
        if(binary[idx] == 1)
            r = (r * C) % N;
    }
    
    printf("Decryption M' : %ld \n",r);
    return r;
}

void keyGen(long p,long q){
    long long N,M,mPrime;
    long pieN,e,secretKey,C;
    
    printf("%s \n",millerRabinTest(p,30));
    printf("%s \n",millerRabinTest(q,30));
    
    N = p * q;
    pieN = oilerPie(p,q);
    
    //임의의 e값이기 때문에 가장 작은 값을 선택하게끔 작성했습니다.
    for(long i = 2; i < pieN; i++){
        if(euclidean(pieN,i) == 1){
            e = i;
            break;
        }
    }
    
    secretKey = extendedEuclidean(pieN,e);
    printf("public key: (%lld,%ld), secret key: %ld \n", N,e,secretKey);
    
    M = longRandomRange(N);
    
    //given the public key pk(N,e) and a plainText M (randomNumber)
    C = encryption(N,e,M);
    
    //given the private key secretKey and a cipherText C
    mPrime = decryption(N,secretKey,C);
    
}

int main(){
    long p,q;
    
    scanf("%ld",&p);
    scanf("%ld",&q);
    
    keyGen(p,q);

    
    return 0;
}
