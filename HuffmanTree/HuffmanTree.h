#include <iostream>
#include <unordered_map>
#include <map>
#include <queue>
#include <vector>
#include <fstream>
using namespace std;

//Node of Huffman tree
struct Node {
    char letter;
    int frequency;
    Node* left;
    Node* right;

    Node() {
        letter = '#';
        frequency = 0;
        left = nullptr;
        right = nullptr;
    }

    Node(char letter, int frequency) {
        this->frequency = frequency;
        this->letter = letter;
        left = nullptr;
        right = nullptr;
    }

};



class Huffman_tree {

public:

    Huffman_tree() {
        root = nullptr;
        content = "";
        encoded_content = "";
    }

    Huffman_tree(const string& content) {

        this->content = content;


        if (content.size() == 1) {
            char ch = content[0];
            letter_to_bitstream[ch] = "0";
            encoded_content = "0";
            root = new Node(ch, 1);
            return;
        }

        // create tree and return root
        root = build_huffman_tree(content);

        //traverse to fill letter_to_bitstream table with right codes
        traverse(root, "");

        //encode original text
        encoded_content = encode(content);


    }

    void print_bitstreams_in_output_file() {


        ofstream output_file("output_file.txt");


        for (auto i : content) {


            output_file << letter_to_bitstream[i];
        }

        output_file.close();

        cout << "File saved.\n";
    }

    void print_code_table_in_output_file() {
       ofstream output_file("code_table.txt");


        
        for (auto i : letter_to_bitstream) {
            output_file << i.first <<i.second<<"\n";
        }

        output_file.close();

        
        cout << "Code table saved.\n";
    }

    void alternative_print_in_output_file() {

        ofstream output_file("alternative.txt");


        //split into strings of size 8
        string eight_bits;

        for (auto i : encoded_content) {
            eight_bits += i;

            if (eight_bits.size() >= 8) {
                int decimal = binary_to_decimal(eight_bits);
                output_file << decimal << " ";
                eight_bits = "";
            }


        }


        //add zeros to last uncomplete byte
        while (eight_bits.size() < 8) {

            eight_bits.append("0");
        }

        //and add to file
        int decimal = binary_to_decimal(eight_bits.substr(0, 8));
        output_file << decimal << " ";

        cout << "File saved in alternative mode.\n";

        output_file.close();

    }

    void decompress() {

        //building the tree from code_table.txt
        Node* new_tree = new Node();

        ifstream tree_file("code_table.txt");

        string code_line;
        while (getline(tree_file, code_line)) {
            char ch = code_line[0];
            add_code_to_tree(code_line.substr(1, code_line.size()), 0, new_tree, ch);
        }



        //reading the compressed data and writing decompressed to new file
        ifstream encoded_file("output_file.txt");
        ofstream decompressed("decompressed.txt");


        Node* itr = new_tree;

        char ch;
        
        while (encoded_file >> noskipws >> ch) {
            if (ch == '0') {
                itr = itr->left;
            }
            if (ch == '1') {
                itr = itr->right;
            }

            if (itr->letter != '#') {
                decompressed << itr->letter;
                itr = new_tree;
            }


        }
        cout << "Data decompressed.\n";

        encoded_file.close();
        decompressed.close();
        tree_file.close();
    }

    void print_compression_rate() {

        double size_orig = (content.size() * 8);
        double result = encoded_content.size() / size_orig;
        result *= 100;

        cout << "\nCompression rate is " << (int)result << "%\n";
        return;
    }

    void print_level_of_compression() {
        double size_orig = (content.size() * 8);
        double size_enc = encoded_content.size();
        double result = size_orig / size_enc;
       

        cout << result<<"\n";
    }

   

private:
   
    Node* root;
    unordered_map<char, string> letter_to_bitstream;
    string content;
    string encoded_content;
   

    int binary_to_decimal(string s_binary) {
        int binary = stoi(s_binary);

        int decimal = 0;
        int temp = binary;
        int base = 1;

        while (temp != 0) {
            int last_digit = temp % 10;
            temp = temp / 10;

            decimal += last_digit * base;

            base *= 2;
        }

        return decimal;

    }

    void traverse(Node* node, string bitstream) {
        //traverses the tree and fills the letter_to_bitstream table with found string

        if (node == nullptr) return;
        if (node->left == nullptr && node->right == nullptr) {
            letter_to_bitstream[node->letter] = bitstream;
            reverse(bitstream.begin(), bitstream.end());
            return;
        }
        if (node->left) {
            string new_bitstream = bitstream;
            new_bitstream.append("0");
            traverse(node->left, new_bitstream);
        }
        if (node->right) {
            string new_bitstream = bitstream;
            new_bitstream.append("1");
            traverse(node->right, new_bitstream);
        }
        return;


    }

    string encode(const string& content) {

        //uses the letter_to_bitstream table to encode the original text
        string encoded = "";

        for (auto i : content) {
            encoded.append(letter_to_bitstream[i]);
        }

        return encoded;
    }

    struct Compare {
        bool operator() (Node* a, Node* b) {
            return a->frequency > b->frequency;

        }

    };

    Node* build_huffman_tree(const string& text) {

    
        
        unordered_map<char, int> frequency = calculate_frequency(text);

        //compares the characters by frequency
        priority_queue<Node*, vector<Node*>, Compare> min_heap;

        for (auto i : frequency) {

            Node* node = new Node(i.first, i.second);
            min_heap.push(node);

        }

        //builds the tree with the characters as leaf nodes
        while (min_heap.size() != 1) {

            Node* one = min_heap.top();
            min_heap.pop();
            Node* two = min_heap.top();
            min_heap.pop();

            Node* internal_node = new Node('#', one->frequency + two->frequency);
            internal_node->left = one;
            internal_node->right = two;

            min_heap.push(internal_node);


        }

        Node* root = min_heap.top();


        return root;


    }

    unordered_map<char, int> calculate_frequency(const string& text)
    { 
        //calculates how many of each character are there 


        unordered_map<char, int> frequency;


        for (auto i : text) {
            if (frequency.find(i) == frequency.end()) {
                frequency[i] = 1;
            }
            else frequency[i]++;

        }
        return frequency;
    }

    void add_code_to_tree(const string& code, int pos, Node*& root, char letter) {
        //reads code and for each symbol adds new Node to the left if 0 and to the right if 1 

        if (pos >= code.size()) {

            root->letter = letter;
            return;
        }
        if (code[pos] == '1') {
            if (root->right == nullptr)
                root->right = new Node('#', 0);
            add_code_to_tree(code, pos + 1, root->right, letter);
        }
        if (code[pos] == '0') {

            if (root->left == nullptr)
                root->left = new Node('#', 0);
            add_code_to_tree(code, pos + 1, root->left, letter);
        }

        return;
    }

};

