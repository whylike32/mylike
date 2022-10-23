#ifndef _H_printf
#define _H_printf
#include <iostream.h>
#include<stdlib.h>
#include"data_2.h"

void printfbox()
{
 system("cls");	
 cout<<"*"<<choriz[0][0]<<choriz[0][0]<<choriz[0][0]<<choriz[0][0]<<"*"<<choriz[0][1]<<choriz[0][1]<<choriz[0][1]<<choriz[0][1]<<"*"<<choriz[0][2]<<choriz[0][2]<<choriz[0][2]<<choriz[0][2]<<"*"<<choriz[0][3]<<choriz[0][3]<<choriz[0][3]<<choriz[0][3]<<"*"<<choriz[0][4]<<choriz[0][4]<<choriz[0][4]<<choriz[0][4]<<"*"<<endl;
 cout<<cvertic[0][0]<<"    "<<cvertic[0][1]<<"    "<<cvertic[0][2]<<"    "<<cvertic[0][3]<<"    "<<cvertic[0][4]<<"    "<<cvertic[0][5]<<endl;
 cout<<cvertic[0][0]<<"  "<<cshowbox[0][0]<<" "<<cvertic[0][1]<<"  "<<cshowbox[0][1]<<" "<<cvertic[0][2]<<"  "<<cshowbox[0][2]<<" "<<cvertic[0][3]<<"  "<<cshowbox[0][3]<<" "<<cvertic[0][4]<<"  "<<cshowbox[0][4]<<" "<<cvertic[0][5]<<endl;
 cout<<cvertic[0][0]<<"    "<<cvertic[0][1]<<"    "<<cvertic[0][2]<<"    "<<cvertic[0][3]<<"    "<<cvertic[0][4]<<"    "<<cvertic[0][5]<<endl;
 cout<<"*"<<choriz[1][0]<<choriz[1][0]<<choriz[1][0]<<choriz[1][0]<<"*"<<choriz[1][1]<<choriz[1][1]<<choriz[1][1]<<choriz[1][1]<<"*"<<choriz[1][2]<<choriz[1][2]<<choriz[1][2]<<choriz[1][2]<<"*"<<choriz[1][3]<<choriz[1][3]<<choriz[1][3]<<choriz[1][3]<<"*"<<choriz[1][4]<<choriz[1][4]<<choriz[1][4]<<choriz[1][4]<<"*"<<endl;
 cout<<cvertic[1][0]<<"    "<<cvertic[1][1]<<"    "<<cvertic[1][2]<<"    "<<cvertic[1][3]<<"    "<<cvertic[1][4]<<"    "<<cvertic[1][5]<<endl;
 cout<<cvertic[1][0]<<"  "<<cshowbox[1][0]<<" "<<cvertic[1][1]<<"  "<<cshowbox[1][1]<<" "<<cvertic[1][2]<<"  "<<cshowbox[1][2]<<" "<<cvertic[1][3]<<"  "<<cshowbox[1][3]<<" "<<cvertic[1][4]<<"  "<<cshowbox[1][4]<<" "<<cvertic[1][5]<<endl;
 cout<<cvertic[1][0]<<"    "<<cvertic[1][1]<<"    "<<cvertic[1][2]<<"    "<<cvertic[1][3]<<"    "<<cvertic[1][4]<<"    "<<cvertic[1][5]<<endl;
 cout<<"*"<<choriz[2][0]<<choriz[2][0]<<choriz[2][0]<<choriz[2][0]<<"*"<<choriz[2][1]<<choriz[2][1]<<choriz[2][1]<<choriz[2][1]<<"*"<<choriz[2][2]<<choriz[2][2]<<choriz[2][2]<<choriz[2][2]<<"*"<<choriz[2][3]<<choriz[2][3]<<choriz[2][3]<<choriz[2][3]<<"*"<<choriz[2][4]<<choriz[2][4]<<choriz[2][4]<<choriz[2][4]<<"*"<<endl;
 cout<<cvertic[2][0]<<"    "<<cvertic[2][1]<<"    "<<cvertic[2][2]<<"    "<<cvertic[2][3]<<"    "<<cvertic[2][4]<<"    "<<cvertic[2][5]<<endl;
 cout<<cvertic[2][0]<<"  "<<cshowbox[2][0]<<" "<<cvertic[2][1]<<"  "<<cshowbox[2][1]<<" "<<cvertic[2][2]<<"  "<<cshowbox[2][2]<<" "<<cvertic[2][3]<<"  "<<cshowbox[2][3]<<" "<<cvertic[2][4]<<"  "<<cshowbox[2][4]<<" "<<cvertic[2][5]<<endl;
 cout<<cvertic[2][0]<<"    "<<cvertic[2][1]<<"    "<<cvertic[2][2]<<"    "<<cvertic[2][3]<<"    "<<cvertic[2][4]<<"    "<<cvertic[2][5]<<endl;
 cout<<"*"<<choriz[3][0]<<choriz[3][0]<<choriz[3][0]<<choriz[3][0]<<"*"<<choriz[3][1]<<choriz[3][1]<<choriz[3][1]<<choriz[3][1]<<"*"<<choriz[3][2]<<choriz[3][2]<<choriz[3][2]<<choriz[3][2]<<"*"<<choriz[3][3]<<choriz[3][3]<<choriz[3][3]<<choriz[3][3]<<"*"<<choriz[3][4]<<choriz[3][4]<<choriz[3][4]<<choriz[3][4]<<"*"<<endl;
 cout<<cvertic[3][0]<<"    "<<cvertic[3][1]<<"    "<<cvertic[3][2]<<"    "<<cvertic[3][3]<<"    "<<cvertic[3][4]<<"    "<<cvertic[3][5]<<endl;
 cout<<cvertic[3][0]<<"  "<<cshowbox[3][0]<<" "<<cvertic[3][1]<<"  "<<cshowbox[3][1]<<" "<<cvertic[3][2]<<"  "<<cshowbox[3][2]<<" "<<cvertic[3][3]<<"  "<<cshowbox[3][3]<<" "<<cvertic[3][4]<<"  "<<cshowbox[3][4]<<" "<<cvertic[3][5]<<endl;
 cout<<cvertic[3][0]<<"    "<<cvertic[3][1]<<"    "<<cvertic[3][2]<<"    "<<cvertic[3][3]<<"    "<<cvertic[3][4]<<"    "<<cvertic[3][5]<<endl;
 cout<<"*"<<choriz[4][0]<<choriz[4][0]<<choriz[4][0]<<choriz[4][0]<<"*"<<choriz[4][1]<<choriz[4][1]<<choriz[4][1]<<choriz[4][1]<<"*"<<choriz[4][2]<<choriz[4][2]<<choriz[4][2]<<choriz[4][2]<<"*"<<choriz[4][3]<<choriz[4][3]<<choriz[4][3]<<choriz[4][3]<<"*"<<choriz[4][4]<<choriz[4][4]<<choriz[4][4]<<choriz[4][4]<<"*"<<endl;
 cout<<cvertic[4][0]<<"    "<<cvertic[4][1]<<"    "<<cvertic[4][2]<<"    "<<cvertic[4][3]<<"    "<<cvertic[4][4]<<"    "<<cvertic[4][5]<<endl;
 cout<<cvertic[4][0]<<"  "<<cshowbox[4][0]<<" "<<cvertic[4][1]<<"  "<<cshowbox[4][1]<<" "<<cvertic[4][2]<<"  "<<cshowbox[4][2]<<" "<<cvertic[4][3]<<"  "<<cshowbox[4][3]<<" "<<cvertic[4][4]<<"  "<<cshowbox[4][4]<<" "<<cvertic[4][5]<<endl;
 cout<<cvertic[4][0]<<"    "<<cvertic[4][1]<<"    "<<cvertic[4][2]<<"    "<<cvertic[4][3]<<"    "<<cvertic[4][4]<<"    "<<cvertic[4][5]<<endl;
 cout<<"*"<<choriz[5][0]<<choriz[5][0]<<choriz[5][0]<<choriz[5][0]<<"*"<<choriz[5][1]<<choriz[5][1]<<choriz[5][1]<<choriz[5][1]<<"*"<<choriz[5][2]<<choriz[5][2]<<choriz[5][2]<<choriz[5][2]<<"*"<<choriz[5][3]<<choriz[5][3]<<choriz[5][3]<<choriz[5][3]<<"*"<<choriz[5][4]<<choriz[5][4]<<choriz[5][4]<<choriz[5][4]<<"*"<<endl;
}
#endif