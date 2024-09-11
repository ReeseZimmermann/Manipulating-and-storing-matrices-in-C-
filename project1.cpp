/**
 * Author: Reese Zimmermann
 * Date: 08/26/2024
 * Instructor: Sridhar Radhakrishnan
 * Course: CS 2413-001
 * School: University of Oklahoma
 **/

//Header
#include <iostream>
//#include <fstream> // for file input/output
using namespace std;

// defining all classes
// write methods after the class definition

class SparseRow {
protected: 
    int row; // row number
    int col; // column number
    int value; // value assuming all values are integers

public:
    // default constructor, row = -1; col = -1; val = 0;
    SparseRow() : row(-1), col(-1), value(0) {} 
    SparseRow(int r, int c, int v) : row(r), col(c), value(v) {} // parameterized constructor

    //corrected during the 
    void display() const { // print Row #, Col #, and Value
        cout << row << ", " << col << ", " << value << endl;
    }
    /** copilot assistance for the following method:
         * @brief Overloads the insertion operator to allow printing a SparseRow object.
         * 
         * @param s The output stream to write to.
         * @param sr The SparseRow object to be printed.
         * @return The output stream after writing the SparseRow object.
     */
    friend ostream& operator<<(ostream& s, const SparseRow& sr) {
        s << sr.row << ", " << sr.col << ", " << sr.value;
        return s;
    }
    // getter and setter methods
    int getRow() const { return row; } // getter for row
    int getCol() const { return col; } // getter for column
    int getValue() const { return value; } // getter for value
    void setRow(int r) { row = r; } // setter for row
    void setCol(int c) { col = c; } // setter for column
    void setValue(int v) { value = v; } // setter for value
};

// construct sparse matrix class
class SparseMatrix {
protected:
    int noRows; // Number of rows of the original matrix
    int noCols; // Number of columns of the original matrix
    int commonValue; // read from input
    int noNonSparseValues; // Number of non-sparse values in the matrix
    SparseRow* myMatrix; // Pointer to an array of SparseRow objects

public:
    // default constructor initializes the initial values for the class members
    //*nullptr is used to point to nothing in memory*
    SparseMatrix() : noRows(0), noCols(0), commonValue(0), noNonSparseValues(0), myMatrix(nullptr) {}
    SparseMatrix(int n, int m, int cv, int nsv) : noRows(n), noCols(m), commonValue(cv), noNonSparseValues(nsv) {
        myMatrix = new SparseRow[nsv]; // allocate memory for the non-sparse values
    }
    ~SparseMatrix() { // destructor
        delete[] myMatrix; // deallocate memory for the non-sparse
    }
    // Transpose method : help of copilot extension to breakdown the Logic
    //Transpose method: creates and returns new SparseMatrix object that is the transpose of the original matrix
    SparseMatrix* Transpose() { 
        // create a new SparseMatrix object with the number of columns and rows swapped
        SparseMatrix* transposed = new SparseMatrix(noCols, noRows, commonValue, noNonSparseValues);
        // for each non-sparse value in the original matrix
        for (int i = 0; i < noNonSparseValues; ++i) {
            // set the row, col, value of the i-th non-sparse value in the transposed matrix to the
            // column, row, and value of the i-th non-sparse value in the original matrix
            transposed->myMatrix[i].setRow(myMatrix[i].getCol());
            transposed->myMatrix[i].setCol(myMatrix[i].getRow());
            transposed->myMatrix[i].setValue(myMatrix[i].getValue());
        }
        return transposed; // return the transposed matrix
    }
    /* Copilot Extension helped to generate the following code - Logic breakdown
     * @brief Multiply method: creates and returns a new SparseMatrix object that is the 
     *              product of the original matrix and matrix M.
     * 
     * @param M The matrix to multiply by.
     * @return A pointer to a new SparseMatrix object that is the product of the original matrix and matrix M.
     */
    SparseMatrix* Multiply(SparseMatrix& M) {
        //checks number of columns in the current matrix is equal to the number of rows in matrix M
        //if not, multiplication is not possible, returns nullptr
        if (noCols != M.noRows) {
            cout << "Matrix multiplication is not possible" << endl;
            return nullptr;
        }
        // create a new array to store the result of the multiplication
        int* result = new int[noRows * M.noCols]();
        // for each non-sparse value in the current matrix
        for (int i = 0; i < noNonSparseValues; ++i) {
            // for each non-sparse value in matrix M
            for (int j = 0; j < M.noNonSparseValues; ++j) {
                // if the column of current matrix is = to row of matrix M
                if (myMatrix[i].getCol() == M.myMatrix[j].getRow()) {
                    int r = myMatrix[i].getRow(); // row of current matrix
                    int c = M.myMatrix[j].getCol(); // column of matrix M
                    // multiply the values and add to the result array
                    result[r * M.noCols + c] += myMatrix[i].getValue() * M.myMatrix[j].getValue();
                }
            }
        }
        // corrected by copilot to track the number of non-zero values in the result array
        // count the number of non-zero values in the result array
        int countNonZero = 0;
        // for each value in the result array
        for(int i = 0; i < noRows * M.noCols; i++) {
            // if the value is not zero, increment the count
            if(result[i] != 0) {
                countNonZero++;
            }
        }
        /* Using prompt "Logic to create a new SparseMatrix object to store the product of the two matrices"
         * Helped to structure the following code by providing steps to implement the multiplication
         * 1. Create a new SparseMatrix object to store the product of the two matrices
         * 2. Initialize the number of non-sparse values to 0
         * 3. Allocate memory for the non-sparse values in the result matrix
        */
        SparseMatrix* multipliedMatrix = new SparseMatrix(noRows, M.noCols, commonValue, countNonZero);
        int k = 0;
        // for each row in the result array
        for(int i = 0; i < noRows; i++) {
            // for each column in the result array
            for(int j = 0; j < M.noCols; j++) {
                // if the value is not zero
                if(result[i * M.noCols + j] != 0) {
                    // set the row, column, and value of the k-th non-sparse value in the result matrix
                    multipliedMatrix->myMatrix[k].setRow(i);
                    multipliedMatrix->myMatrix[k].setCol(j);
                    multipliedMatrix->myMatrix[k].setValue(result[i * M.noCols + j]);
                    // increment k to move to the next non-sparse value
                    k++;
                }
            }
        }
        // deallocate memory for the result array
        delete[] result;
        // return the multiplied matrix
        return multipliedMatrix;
    }
    /* Copilot Extension helped to generate the following code - Logic breakdown to perform Addition operation
     * @brief Add method: creates and returns a new SparseMatrix object that is the 
     *              sum of the original matrix and matrix M.
     * 
     * @param M The matrix to add.
     * @return A pointer to a new SparseMatrix object that is the sum of the original matrix and matrix M.
     */
    SparseMatrix* Add(SparseMatrix& M) {
        // check if the dimensions of the two matrices are the same
        if (noRows != M.noRows || noCols != M.noCols) {
            // if not, return nullptr
            cout << "Matrix addition is not possible" << endl;
            return nullptr;
        } // create a new array to store the result of the addition
        int* result = new int[noRows * noCols]();
        // for each non-sparse value in the current matrix
        for (int i = 0; i < noNonSparseValues; ++i) {
            int r = myMatrix[i].getRow(); // row of the current matrix
            int c = myMatrix[i].getCol();  // column of the current matrix
            result[r * noCols + c] += myMatrix[i].getValue(); // add the value to the result array
        } // for each non-sparse value in matrix M
        for (int j = 0; j < M.noNonSparseValues; ++j) {
            int r = M.myMatrix[j].getRow();
            int c = M.myMatrix[j].getCol();
            result[r * noCols + c] += M.myMatrix[j].getValue(); // add the value to the result array
        }
        int countNonZero = 0; // count the number of non-zero values in the result array
        for (int i = 0; i < noRows * noCols; i++) { // for each value in the result array
            if (result[i] != 0) { // if the value is not zero, increment the count to move to the next non-sparse value
                countNonZero++;
            }
        } // create a new SparseMatrix object to store the sum of the two matrices
        SparseMatrix* addedMatrix = new SparseMatrix(noRows, noCols, commonValue, countNonZero);
        int k = 0;
        for (int i = 0; i < noRows; i++) { // for each row in the result array
            for (int j = 0; j < noCols; j++) { // for each column in the result array
                if (result[i * noCols + j] != 0) {  // if the value is not zero
                    addedMatrix->myMatrix[k].setRow(i);
                    addedMatrix->myMatrix[k].setCol(j);
                    addedMatrix->myMatrix[k].setValue(result[i * noCols + j]); // set the row, column, and value of the k-th non-sparse value in the result matrix
                    k++;
                }
            }
        }
        delete[] result; // deallocate memory for the result array
        return addedMatrix; // return the added matrix
    }

    // Overload the insertion operator to allow printing a SparseMatrix object
    // Takes an output stream (ostream) and a SparseMatrix (const SparseMatrix&)
    // using 's' as the output stream and 'sm' as the SparseMatrix object
    friend ostream& operator<<(ostream& s, const SparseMatrix& sm) {
        for(int i = 0; i < sm.noNonSparseValues; i++) { // for each non-sparse value in the matrix
            s << sm.myMatrix[i] << endl; // print the i-th non-sparse value in the matrix
        }
        return s; // return the output stream
    }
    
    void displayMatrix() const {
        // Copilot extension "Explain the displayMatrix of the original format, and allocate memory for the full matrix"
        // 1. Nested loop to iterate through the rows (outer) and columns (inner) of the matrix
        // 2. Check if the current position is less than the total number of non-sparse value and the row and column 
        // of the current position matches the row and column of the non-sparse value.
        // 3. If the condition is true, print the value of the non-sparse value and increment the position.
        // 4. If the condition is false, print the common value to fill the sparse position.
        int k = 0;
        for (int i = 0; i < noRows; ++i) {
            for (int j = 0; j < noCols; ++j) {
                if (k < noNonSparseValues && myMatrix[k].getRow() == i && myMatrix[k].getCol() == j) {
                    cout << myMatrix[k].getValue() << " ";
                    ++k;
                } else {
                    cout << commonValue << " ";
                }
            }
            cout << endl;
        }
    }
    // Getters and setters for the class members
    int getNoRows() const {return noRows;}
    int getNoCols() const {return noCols;}
    int getCommonValue() const{return commonValue;}
    int getNoNonSparseValues() const {return noNonSparseValues;}
    SparseRow* getMyMatrix() const {return myMatrix;}
    void setNoRows(int n) {noRows = n;}
    void setNoCols(int m) {noCols = m;}
    void setCommonValue(int cv) {commonValue = cv;}
    void setNoNonSparseValues(int nsv) {noNonSparseValues = nsv;}
    void setMyMatrix(SparseRow* matrix) { myMatrix = matrix;}
};
// Main method
int main() {

    // ifstream inputFile("input1.txt");
    // if (!inputFile) {
    //     cout << "Error opening file." << endl;
    //     return 1;
    // }

    int n, m, cv, noNSV;
    
    SparseMatrix* temp; // temporary pointer to a SparseMatrix object

    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* firstOne = new SparseMatrix(n, m, cv, noNSV); // create a new SparseMatrix object firstOne

    // Read in the first matrix
    int index = 0; 
    for (int i = 0; i < n; ++i) { // iterate through the rows
        for (int j = 0; j < m; ++j) { // iterate through the columns
            int v; // value
            cin >> v; // read the value from the input file
            if (v != cv) { // if the value is not the common value
                // set the row, column, and value of the index-th non-sparse value in the matrix
                firstOne->getMyMatrix()[index].setRow(i);  
                firstOne->getMyMatrix()[index].setCol(j);
                firstOne->getMyMatrix()[index].setValue(v);
                ++index; // increment the index to move to the next non-sparse value
            }
        }
    }

    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* secondOne = new SparseMatrix(n, m, cv, noNSV); // create a new SparseMatrix object secondOne
    // // Read in the second matrix
    index = 0;
    for(int i = 0; i < n; ++i) { // iterate through the rows
        for(int j = 0; j < m; ++j) { // iterate through the columns
            int v; // value
            cin >> v;
            if(v != cv) { // if the value is not the common value
                secondOne->getMyMatrix()[index].setRow(i);
                secondOne->getMyMatrix()[index].setCol(j);
                secondOne->getMyMatrix()[index].setValue(v);
                ++index;
            }
        }
    }
    
    // Corrected from the original code (commented) to display the matrix in sparse format
    cout << "First one in sparse matrix format" << endl;
    //(*firstOne).displayMatrix();
    cout << *firstOne;

    cout << "After transpose" << endl;
    //SparseMatrix* transposed = firstOne->Transpose();
    //transposed->displayMatrix();
    SparseMatrix* firstTransposed = firstOne->Transpose();
    cout << *firstTransposed;

    cout << "First one in matrix format" << endl;
    firstOne->displayMatrix();

    cout << "Second one in sparse matrix format" << endl;
    //(*secondOne).displayMatrix();
    cout << *secondOne;

    cout << "After transpose" << endl;
    // secondOne->Transpose();
    // transposed->displayMatrix();
    SparseMatrix* secondTransposed = secondOne->Transpose();
    cout << *secondTransposed;

    cout << "Second one in matrix format" << endl;
    secondOne->displayMatrix();

    cout << "Matrix addition result" << endl;
    temp = firstOne->Add(*secondOne); 
    if (temp != nullptr){ // if the result is not null
        temp->displayMatrix(); // display the result
        delete temp; // deallocate memory for the result
    }

    cout << "Matrix multiplication result" << endl;
    temp = firstOne->Multiply(*secondOne); // multiply the two matrices
    if (temp != nullptr) { // if the result is not null
        temp->displayMatrix(); // display the result
        delete temp; // deallocate memory for the result
    }

    delete firstOne; // deallocate memory for the first matrix
    delete secondOne; // deallocate memory for the second matrix
    delete firstTransposed; // deallocate memory for the transposed first matrix
    delete secondTransposed; // deallocate memory for the transposed second matrix

    return 0; // return 0 to indicate successful completion
}
// End of the program

/**     Use of Copilot Extension:
 * Copilot extension was used to generate the logic for the Transpose, Multiply, and Add methods.
 * The extension helped to breakdown the logic into steps and provided a structured approach to implement the methods.
 * The extension also provided a detailed explanation of the logic to create a new SparseMatrix object to 
 * store the product of the two matrices. 
 * The extension use is listed in the comments above the methods it helped to generate and the logic provided.
 * While the extension suggested the logic, the implementation was done manually to ensure the code was executing what was being called.
 * 
 */
/**  Debugging and Testing:
 * After formatting the program given in the Project 1 instructions, I began to implement the methods.
 * With the help of the copilot extension, I was able to understand how each method works.
 * I implemented the fstream operator to test whether the input file was being read correctly.
 * Many errors occured during the intial test, most of which were due to incorrect implementation of the methods.
 * I decided to comment out the code I originally implemented to test whether the original matrix was being displayed correctly in the terminal. 
 * After testing, I was able to see that there were issues in handling the zero values read in, due to the common value being multiplied in a for loop I had written.
 * I corrected the issue by adding a condition to check if the value was not equal to the common value before setting the row, column, and value of the non-sparse value.
 * After correcting the zero value issue, I was able to successfully read the original matrix values and print to the terminal. 
 * Although the code was running, the format of the original matrix was being displayed not the Sparse format. 
 * I was given help by the TA to understand that the issue was in the displayMatrix method and incorrect implementation of the Multiplcation and Add methods.
 * After using copilot to understand what the method I had wrote was doing, I saw that I wasn't using the correct For Loop correctly for the First and Second SparseMatrix object. 
 * In addition, I was not calling the ostream operator to display the correct matrix in the terminal, which was causing formmatting issues in the terminal. 
 * I corrected the issue by fixing the ostream method to set the "," of the sparseMatrix, and then fixed the second issue by using a nested loop to iterate 
 * through the rows and columns of the matrix and check if the current position was less than the total number of non-sparse values.
 * The methods were corrected using the copilot extension to generate the logic that I missed in my original implementation.
 * After correcting the methods, I ran the code again to see the terminals reading of the input file.
 * The output was incorrect, and quickly realized the issue was with the main program and the way the methods were being called.
 * I incorporated if statements to check if the result of the methods was not null using the temp value before displaying the matrix. 
 * Furthermore, I added delete statements to deallocate memory for the result of the methods.
 * After making the changes, I ran the code again and was able to see the correct output in the terminal.
 * The last step was to test the code with the remaining input files to ensure the program was working correctly.
 * After testing, I was able to see the correct output in the terminal.
 * 
 */