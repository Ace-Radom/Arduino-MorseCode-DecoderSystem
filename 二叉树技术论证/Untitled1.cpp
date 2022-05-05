#include<bits/stdc++.h>
using namespace std;
struct Morse_Code_BinaryTree{
    char Root;
    Morse_Code_BinaryTree *Lbranch;
    Morse_Code_BinaryTree *Rbranch;
};

char Morse_Code_Pre_Set[31] = { '$' , 'E' , 'I' , 'S' , 'H' , 'V' , 'U' , 'F' , '/' , 'A' , 'R' , 'L' , '/' , 'W' , 'P' , 'J' , 'T' , 'N' , 'D' , 'B' , 'X' , 'K' , 'C' , 'Y' , 'M' , 'G' , 'Z' , 'Q' , 'O' , '/' , '/' };
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
        SetValue( tree -> Lbranch );
        SetValue( tree -> Rbranch );
    }
}
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
int main(){
	Morse_Code_BinaryTree *MCBT = new Morse_Code_BinaryTree;
	SetValue( MCBT );
	printvalue( MCBT );
	return 0;
}
