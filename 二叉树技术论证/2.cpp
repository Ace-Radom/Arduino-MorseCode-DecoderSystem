#include<bits/stdc++.h>
using namespace std;
struct Morse_Code_BinaryTree{
    char Root;
    Morse_Code_BinaryTree *Lbranch;
    Morse_Code_BinaryTree *Rbranch;
};

char Morse_Code_Pre_Set[100] = { ' ' , '$' , 'E' , 'I' , 'S' , 'H' , '/' , '/' , 'V' , '/' , '/' , 'U' , 'F' , '/' , '/' , '/' , 'A' , 'R' , 'L' , '/' , '/' , '/' , 'W' , 'P' , '/' , '/' , 'J' , '/' , '/' , 'T' , 'N' , 'D' , 'B' , '/' , '/' , 'X' , '/' , '/' , 'K' , 'C' , '/' , '/' , 'Y' , '/' , '/' , 'M' , 'G' , 'Z' , '/' , '/' , 'Q' , '/' , '/' , 'O' , '/' , '/' };
int Pre_Set_Counter = 0;

void SetValue( Morse_Code_BinaryTree *tree ){
    Pre_Set_Counter++;
    tree -> Root = Morse_Code_Pre_Set[Pre_Set_Counter];
    if ( tree -> Root == '/' )
    {
        return;
    }
    else
    {
        tree -> Lbranch = new Morse_Code_BinaryTree;
        tree -> Rbranch = new Morse_Code_BinaryTree;
        //printf( "%c left branch\n" , tree -> Root );
        SetValue( tree -> Lbranch );
        //printf( "%c right branch\n" , tree -> Root );
        SetValue( tree -> Rbranch );
    }
}
char MorseCode_ReadIn_Data[5];

int Counter = 0;
char Decode_Path_SearchEngine( Morse_Code_BinaryTree *tree ){
    if ( Counter == strlen( MorseCode_ReadIn_Data ) )
    {
        return tree -> Root;
    }
    if ( MorseCode_ReadIn_Data[Counter] == '.' )
    {
        cout << Counter << " " << MorseCode_ReadIn_Data[Counter] << endl;
        Counter++;
        return Decode_Path_SearchEngine( tree -> Lbranch );
    }
    if ( MorseCode_ReadIn_Data[Counter] == '-' )
    {
        cout << Counter << " " << MorseCode_ReadIn_Data[Counter] << endl;
        Counter++;
        return Decode_Path_SearchEngine( tree -> Rbranch );
    }
    return 'e';
}
/*
void printvalue( Morse_Code_BinaryTree *tree ){
    if ( tree -> Root == '/' )
        return;
    else
    {
        printf( "%c\n" , tree -> Root );
        printvalue( tree -> Lbranch );
        printvalue( tree -> Rbranch );
    }
}
*/
int main(){
	Morse_Code_BinaryTree *MCBT = new Morse_Code_BinaryTree;
	SetValue( MCBT );
    gets( MorseCode_ReadIn_Data );
    cout << Decode_Path_SearchEngine( MCBT ) << endl;
	return 0;
}