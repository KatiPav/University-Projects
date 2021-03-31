#include <iostream>
#include <fstream>
#include <string>

#include "HuffmanTree.h"
using namespace std;


int main() {
    
    string filename;
    cout << "Enter the name of the file you want to compress: ";
    cin >> filename;

    string filename_txt = filename + ".txt";



   
    ifstream input_file(filename_txt);
    //check if file is opened successfully
    if (!input_file.is_open()) {
        std::cout << "Error opening file";
        return 0;
    }




    //read file line by line and write to string
    string content;
    string content_line;
    
    while (getline(input_file, content_line)) {
       
        content += content_line;
    }


  
    //create huffman tree
    Huffman_tree htree(content);


    int input;
    while(1) {

        system("CLS");

        cout << "This is a simple compressor using Huffman's Algorithm\n\n";
        cout << "To save compressed data in file named \"output_file\":  1\n";
        cout << "To save compressed data in alternative mode in file named \"alternative\":  2\n";
        cout << "To save table with codes for characters in file named \"code_table\":  3\n";
        cout << "To show compression rate:  4\n";
        cout << "To show level of compression:  5\n";
        cout << "To decompress data in file named \"decompressed\":  6\n";
        cout << "To close:  7\n";

        cin >> input;

        if (input == 1) {

            htree.print_bitstreams_in_output_file();         
            system("pause");
        }
        else if (input == 2) {
            htree.alternative_print_in_output_file();
            system("pause");
        }
        else if (input == 3) {
            htree.print_code_table_in_output_file();
            system("pause");
        }
        else if (input == 4) {
            htree.print_compression_rate();
            system("pause");
        }
        else if (input == 5) {
            htree.print_level_of_compression();
            system("pause");
        }
        else if (input == 6) {
            htree.decompress();
            system("pause");
        }
        else if (input == 7) return 0;


    } 



    input_file.close();
}
