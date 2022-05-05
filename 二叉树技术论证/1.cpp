#include<iostream>
struct Tree{
    char Root;
    Tree *Rbranch;
    Tree *Lbranch;
};
void setvalue( Tree *tree ){
    printf( " plese give value" );
    scanf( "%c" , &( tree -> Root ) );
    fflush( stdin );
    if ( tree -> Root == '/' )
        return;
    else
    {
        tree -> Lbranch = new Tree;
        tree -> Rbranch = new Tree;
        printf( "%c left branch" , tree -> Root );
        setvalue( tree -> Lbranch );
        printf( "%c right branch" , tree -> Root );
        setvalue( tree -> Rbranch );
    }
}
void printvalue( Tree *tree ){
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
    Tree *test = new Tree;
    setvalue( test );
    puts( "ending\n" );
    printvalue( test );
    getchar();
    return 0;
}