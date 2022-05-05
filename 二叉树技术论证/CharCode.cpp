#include<bits/stdc++.h>
using namespace std;
char c[] = "Hello World";
//char cc[] = "";
int main(){
    puts( c );
    strcat( c , "!" );
    puts( c );
    strcpy( c , "" );
    puts( c );
    cout << "End" << endl;
    if ( strlen( c ) == 0 )
    {
        cout << "Yes" << endl;
    }
    return 0;
}