#include <cstdlib>
#include <iostream>

#include "BddManager.hxx"

using namespace bdd;

using namespace std;

int main(int argc, char *argv[])
{
    BddManager& bddman = *(new BddManager());

    cout << "call to createVariables(5)" << endl;    
    bddman.createVariables(5);
    
    //cout << "get variable a,b,c nodes of variables 0,1,2 respectively" << endl;
    int a = bddman.variable(0);
    int b = bddman.variable(1);
    int c = bddman.variable(2);
    
    int ab = bddman.opAnd(a,b);
    cout << "a*b = " << ab << endl;
    
    int nabd = bddman.opOr(bddman.opNot(ab),bddman.variable(3));
    cout << "!(a*b)+d = " << nabd << endl;
    
//    bddman.dumpUniqueTable();
//    bddman.dumpNodes();

    bddman.swapLevels(1);
    cout << "swaped levels(1,2)" << endl;
    
//    bddman.dumpUniqueTable();
//    bddman.dumpNodes();

    int ba = bddman.opAnd(b,a);
    cout << "ba = " << ba << endl;

    //bddman.swapLevels(1);
    
    int abc = bddman.opAnd(ab,c);
    cout << "abc = " << abc << endl;
    
    int myXor = bddman.opXor(nabd,abc);
    
    //bddman.dumpNodes();

    cout << "node count before = " << bddman.nodeCount(myXor) << endl;

    bddman.simpleSifting(myXor);

    cout << "node count after = " << bddman.nodeCount(myXor) << endl;

    cout << "sifting again..." << endl;

    bddman.simpleSifting(myXor);

    cout << "node count after second run = " << bddman.nodeCount(myXor) << endl;


    //bddman.swapLevels(1);
    //cout << "swaped levels(1,2)" << endl;
    
    //bddman.dumpNodes();
    
    int cba = bddman.opAnd(c,ba);
    cout << "cba = " << cba << endl;
    
    //bddman.dumpUniqueTable();

    //bddman.swapLevels(1);
    
/*
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "c = " << c << endl;
    
    cout << "and2 = " << and2 << endl;
    cout << "and3 = " << and3 << endl;
    
    cout << "call not(and3)" << endl;
    int nand3 = bddman.opNot(and3);
    cout << "not(and3) = " << nand3 << endl;

    cout << "second call not(and3)" << endl;
    nand3 = bddman.opNot(and3);
    cout << "not(and3) = " << nand3 << endl;
    
    //cout << "call xor(not(and3),not(c)))" << endl;
    //int xorOp = bddman.opXor(nand3,bddman.opNot(c));
    //cout << "xor(not(and3),not(c))) = " << xorOp << endl;
    
    bddman.swapLevels(1);
    
    cout << "var(1) level = " << bddman.var2level(1) << endl;
    cout << "var(2) level = " << bddman.var2level(2) << endl;
    cout << "level(1) var = " << bddman.level2var(1) << endl;
    cout << "level(2) var = " << bddman.level2var(2) << endl;

    cout << "one more time!" << endl;
    
    c = bddman.variable(2);
    b = bddman.variable(1);
    a = bddman.variable(0);
    
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "c = " << c << endl;

    int and2again = bddman.opAnd(b,a);    
    int and3again = bddman.opAnd(bddman.opAnd(c,b),a);
    int nand3again = bddman.opNot(and3again);

    cout << "and2 again = " << and2again << endl;    
    cout << "and3 again = " << and3again << endl;
    cout << "nand3 again = " << nand3again << endl;
    
    */

    delete &bddman;
    //cout << "after ~BddManager()" << endl;
    
    //system("PAUSE"); // windows only
    return EXIT_SUCCESS;
}
