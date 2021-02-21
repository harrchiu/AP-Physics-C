#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

// since many 2d vectors will be used, declare new type
// long double used to prevent math overflow and allow arithmetic precision
typedef vector<vector<long double> > Matrix;

vector<int> newv;

// return long double value for exponents
long double exp(long double base, int power){
    long double cur = 1;
    for (int x = 0; x < power; x ++){
        cur *= base;
    }
    return cur;
}

// return the string length of a number (i.e. 1234 --> 4, -56 --> 3)
int len(long double num){
    
    // take care of numbers != 0 between -1 and 1 (0.132, -0.3534, etc.)
    if (num < 1 && num > -1){
        if (num < 0){
            return 2;
        }
        else{
            if (num == 0){             // square off exact zeros to clear neg. signs
                num = exp(num,2);
            }
            return 1;
        }
    }
    // add another digit for the negative sign if num is negative
    if (num < 0){
        return (log10(-num)+2);
    }
    // otherwise use log to find length of num
    else if (num == 0){
        num = exp(num,2);
        return 1;
    }
    return (log10(num)+1);
}

// return new, empty Matrix using specified sizes (must be 2D but one dim. can equal 1)
Matrix newMatrix(int rows, int cols){
    // make row vector with 'cols' int elements
    vector<long double> rowV(cols);
    
    // declare full Matrix using previous row vector
    Matrix full(rows,rowV);
    return full;
}

// display Matrix with values right justified
void printJustMatrix(Matrix mtrx){
    
    // find the 'longest' value per column (using len(x)). store each longest length
    // add appropriate # of spaces to line up number in column, based off vector
    vector<int> colLens(mtrx[0].size(),0);
    
    // filling colLens
    for (int r = 0; r < mtrx.size(); r ++){
        for (int c = 0; c < mtrx[0].size(); c ++){
            if (len(mtrx[r][c]) > colLens[c]){
                colLens[c] = len(mtrx[r][c]);
            }
        }
    }
    
    // iterate through mtrx and print val with correct # of spaces based on len()
    for (int r = 0; r < mtrx.size(); r ++){
        cout << "\n";
        for (int c = 0; c < mtrx[0].size(); c ++){
            printf("%*.4Lf", colLens[c]+5, mtrx[r][c]);
            cout << string(2, ' ' );
        }
    }
}

// transposes first matrix and stores in new, return new
Matrix transposeMatrix(Matrix firstMtrx){
    
    // create a transposed matrix with swapped dimensions as original (e.g. 2x3 -> 3x2)
    // +1 for 0th degree (i.e. n^0)
    Matrix transpMtrx = newMatrix(firstMtrx[0].size(),firstMtrx.size());
    
    // assign values from original to new matrix (swap coordinates from original)
    for (int r = 0; r < firstMtrx.size(); r ++){
        for (int c = 0; c < firstMtrx[0].size(); c ++){
            transpMtrx[c][r] = firstMtrx[r][c];
        }
    }
    return transpMtrx;      // return result matrx
}

// return product of firstMtrx multiplied by secondMtrx (e.g.(M1)(M2))
Matrix multiplyMatrix(Matrix firstMtrx, Matrix secondMtrx){
    
    // new Matrix must have dimensions (rows of firstMtrx x columns of firstMtrx)
    Matrix productMtrx = newMatrix(firstMtrx.size(),secondMtrx[0].size());
    
    // iterate through rows then columns through product mtrx
    for (int r = 0; r < firstMtrx.size(); r ++){
        for (int c = 0; c < secondMtrx[0].size(); c ++){
            // set current value to 0 to start
            // (used as running total for cur. position's value)
            productMtrx[r][c] = 0;
            
            // iterate through the row/col of the mtrx's
            for (int z = 0; z < firstMtrx[0].size(); z ++){
                // add product of the corresponding row&col values to the running total
                productMtrx[r][c] += firstMtrx[r][z]*secondMtrx[z][c];
            }
        }
    }
    return productMtrx;      // return result matrx
}

// used for slotting values to first 'free' index of vectors in getMinor()
// will find the first '-1' default value and replace it with a given val
void addVal(vector<int>& input, int val){
    for (int x = 0; x < input.size(); x ++){
        if (input[x] == -1){
            input[x] = val;
            return;
        }
    }
}

// given a (nxn) square matrix and one of its indices, return a square
// matrix of (n-1)x(n-1), where all values from the 1st matrix are included
// excluding those in the same row or col as the given index
// xPos and yPos are mtrx position of the current value you're getting the minor of
// i.e.
//     [1 2 3]
// a = [4 5 6]
//     [7 8 9]
//
// the minor of position with value 2 (getMinor(input = a, xPos = 0, yPos = 1)) is:
//     [4 6]
//     [7 9]

Matrix getMinor(Matrix input, int xPos, int yPos){
    Matrix minor = newMatrix(input.size()-1, input[0].size()-1);
    
    // to know which values from original Matrix to add,
    // keep track of which rows and columns to carry over to minor Matrix
    vector<int> rowsV(minor.size(),-1);      // set all values to -1 by default
    vector<int> colsV(minor.size(),-1);      // for addVal function
    
    for (int x = 0; x < input.size(); x++){
        if (x != xPos){
            addVal(rowsV, x);}
        if (x != yPos){
            addVal(colsV, x);}
    }
    
    // iterate through new matrix, set each value
    for (int r = 0; r < minor.size(); r ++){
        for (int c = 0; c < minor[0].size(); c ++){
            minor[r][c] = input[rowsV[r]][colsV[c]];
        }
    }
    return minor;
}

// get determinant of a matrix
long double determinant(Matrix input){
    
    // take care of base case of 2x2
    if (input.size() == 2 && input[0].size() == 2){
        return input[0][0]*input[1][1]-input[0][1]*input[1][0];
    }
    
    long double det = 0;                            // set running total
    // alternating signs, add the product of each first row
    // value with the determinant of its minor
    for (int x = 0; x < input.size(); x ++){
        det += exp(-1,x) * input[0][x]*determinant(getMinor(input,0,x));
    }
    return det;
}

// return the adjunct of a matrix (using determinants)
Matrix getAdjMatrix(Matrix input){
    Matrix newMtrx = newMatrix(input.size(),input[0].size());
    
    // handle odd case of 2x2 matrix
    if (input.size() == 2 && input[0].size() == 2){
        newMtrx[0][0] = input[1][1];
        newMtrx[0][1] = -input[0][1];
        newMtrx[1][0] = -input[1][0];
        newMtrx[1][1] = input[0][0];
        return newMtrx;
    }
    
    // alternating signs throughout position of adjunct,
    // set value equal to det. of that position's minor
    for (int r = 0; r < input.size(); r ++){
        for (int c = 0; c < input[0].size(); c ++){
            newMtrx[r][c] = exp(-1,r+c)*determinant(getMinor(input,r,c));
        }
    }
    // adjunct matrix must finally be its transpose
    Matrix flippedMtrx = transposeMatrix(newMtrx);
    return flippedMtrx;
}

// return the inverse of a matrix, given its adjunct
Matrix getInverseMatrix(Matrix adjMtrx, Matrix originalMtrx){
    Matrix inverseMtrx = newMatrix(adjMtrx.size(),adjMtrx[0].size());
    for (int r = 0; r < adjMtrx.size(); r ++){
        for (int c = 0; c < adjMtrx[0].size(); c ++){
            inverseMtrx[r][c] = adjMtrx[r][c]/determinant(originalMtrx);
        }
    }
    return inverseMtrx;
}

//////////////////////////////////////////////////////////
////////////////  MAIN PROGRAM  //////////////////////////
//////////////////////////////////////////////////////////
int main(int argc, const char * argv[]) {
    
    // change degree of V --> 3 for cubic fit
    int degree = 3;
    // option to prevent user from choosing degree at runtime
    bool askForDegree = true;
    if (askForDegree == true){
        cout << "desired degree: ";
        cin >> degree;
    }
    
    // get input of how many expected coordinate pairs
    int coorPairs;
    cout << "# of coordinate pairs: ";
    cin >> coorPairs;
    
    // initialize indiv. coordinate value arrays of appropriate size
    Matrix xs = newMatrix(coorPairs,1);
    Matrix ys = newMatrix(coorPairs,1);
    
    // main Vandermonde Matrix
    Matrix vMtrx = newMatrix(coorPairs,degree+1); // +1 for 0th degree (i.e. n^0)
    
    // collecting appropriate # of coordinate pairs
    for (int i = 0; i < coorPairs; i ++){
        
        // ask for current x-coordinate then append to x-values array
        cout << "---\nEnter x" + to_string(i+1) + ": ";
        cin >> xs[i][0];
        
        // ask for current y-coordinate then append to y-values array
        cout << "Enter y" + to_string(i+1) + ": ";
        cin >> ys[i][0];
        
        // initialize V Matrix using the set degree
        for (int z = 0; z < degree+1; z ++){        // n^3, n^2, n^1, n^0
            vMtrx[i][z] = (long double)exp(xs[i][0],degree-z);
        }
    }
    
    // create new tranposed matrix from V
    Matrix v_T_mtrx = transposeMatrix(vMtrx);
    
    // store product matrix (vTv) into new matrix
    Matrix v_T_v_mtrx = multiplyMatrix(v_T_mtrx,vMtrx);
    
    //////////////////////////////////////////////////////////
    ///////////////////  PRINTING  ///////////////////////////
    //////////////////////////////////////////////////////////
    
    // X inputs
    cout << "\n---x inputs";
    printJustMatrix(xs);
    
    // Y inputs
    cout << "\n---y inputs";
    printJustMatrix(ys);
    
    // Main Vandermonde matrix (v)
    cout << "\n\n---v:";
    printJustMatrix(vMtrx);
    
    // transposed Vandermonde matrix (vT)
    cout << "\n\n---vT:";
    printJustMatrix(v_T_mtrx);
    
    // transposed V matrix multiplied by original Vandermonde matrix (vTv)
    cout << "\n\n---vTv:";
    printJustMatrix(v_T_v_mtrx);
    
    // if the det. of the product matrix is 0, the curve is ambiguous or "unsolvable"
    if (determinant(v_T_v_mtrx) != 0){
        
        // to find (vTv)^-1, first find adjunct of vTv
        Matrix adjMtrx = getAdjMatrix(v_T_v_mtrx);
        
        // print inverse using getInverseMatrix
        cout << "\n\n---inverse(vTv): ";
        Matrix inverted_vTvMtrx = getInverseMatrix(adjMtrx, v_T_v_mtrx);
        printJustMatrix(inverted_vTvMtrx);
        
        // print vTy
        cout << "\n\n---vTy: ";
        Matrix vT_y_mtrx = multiplyMatrix(v_T_mtrx, ys);
        printJustMatrix(vT_y_mtrx);
        
        // finally print the coefficient matrix (found with ((vTv)^-1)(vTy)
        cout << "\n\n---c matrix (coefficients): \n\n";
        Matrix c_mtrx = multiplyMatrix(inverted_vTvMtrx, vT_y_mtrx);

        // keep track of longest string value to line up results
        int longest = 0;
        for (int r = 0; r < c_mtrx.size(); r ++){
            for (int c = 0; c < c_mtrx[0].size(); c ++){
                if (len(c_mtrx[r][c]) > longest){
                    longest = len(c_mtrx[r][c]);
                }
            }
        }
        
        // go through coeffs. for x's of degree higher than 1 (i.e. x^2, x^3, ...)
        for (int x = 0; x < c_mtrx.size()-2; x ++){
            cout << "x^" + to_string(c_mtrx.size()-1-x) + " => ";
            printf("%*.8Lf", longest+9,c_mtrx[x][0]);
            cout << "\n";
        }
        
        // print remaining coefficients (x & constant value)
        cout << "  x => ";
        printf("%*.8Lf",longest+9,c_mtrx[c_mtrx.size()-2][0]);
        cout << "\n";
        
        cout << " +c => ";
        printf("%*.8Lf",longest+9,c_mtrx[c_mtrx.size()-1][0]);
        cout << "\n\n\n";
    }
    else{       // no solution (det = 0)
        cout << "\n\nmatrix is singular. no solution as there is no inverse.";
    }
    return 0;
}
