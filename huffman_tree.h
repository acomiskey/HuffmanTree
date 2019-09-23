#ifndef _HUFFMAN_TREE_H_
#define _HUFFMAN_TREE_H_

#include <iostream>
#include <vector>

class huffman_tree {
	public:
		struct huffmanNode
		{
			char value = (char)200;
			int weight = 0;
			huffmanNode *left = nullptr;
			huffmanNode *right = nullptr;
			huffmanNode *parent = nullptr;
			bool leftright; // left = false, right = true
			huffmanNode(char v, int w)
			{
				value = v;
				weight = w;
			}
		};
		huffmanNode *rootnode = nullptr;
		std::vector<huffmanNode*> allleaves;
		huffman_tree(const std::string &file_name);
		~huffman_tree();
		
		std::string get_character_code(char character) const;
		std::string encode(const std::string &file_name) const;
		std::string decode(const std::string &string_to_decode) const;	
};

#endif