#include "huffman_tree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>

/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Reads the contents of file_name and constructs a
				huffman tree based on the character frequencies of the file contents
*/
huffman_tree::huffman_tree(const std::string &file_name)
{
	int frequencies[128] = {};

		std::ifstream in(file_name);
		std::string wholefile((std::istreambuf_iterator<char>(in)),
			std::istreambuf_iterator<char>());
		auto iter = wholefile.begin();
		while (iter != wholefile.end())
		{
			frequencies[(int)*iter]++;
			iter++;
		}
	std::vector<huffmanNode*> pseudoqueue;
	for (int c = 0; c < 128; c++)
	{
		if (frequencies[c] > 0)
		{
			huffmanNode *temp = new huffmanNode((char)c, frequencies[c]);
			pseudoqueue.push_back(temp);
			allleaves.push_back(temp);
		}
	}
	huffmanNode *temp = nullptr;
	int biggestindex = 0;
	for (unsigned int d = 0; d < pseudoqueue.size(); d++)
	{
		biggestindex = d;
		for (unsigned int e = d + 1; e < pseudoqueue.size(); e++)
		{
			if (pseudoqueue[e]->weight > pseudoqueue[biggestindex]->weight)
				biggestindex = e;
		}
		temp = pseudoqueue[d];
		pseudoqueue[d] = pseudoqueue[biggestindex];
		pseudoqueue[biggestindex] = temp;
	}

	//for (unsigned int f = 0; f < allleaves.size(); f++)
		//std::cout << allleaves[f]->value << ": " << allleaves[f]->weight << std::endl;

	//std::cout << std::endl;

	while (!(pseudoqueue.empty()))
	{
		if (pseudoqueue.size() == 1)
		{
			rootnode = pseudoqueue.back();
			pseudoqueue.pop_back();
		}
		else
		{
			huffmanNode *newtemp = new huffmanNode((char)200,0);

			newtemp->left = pseudoqueue.back();
			newtemp->left->parent = newtemp;
			newtemp->left->leftright = false;
			//std::cout << pseudoqueue.back()->value << ": " << pseudoqueue.back()->weight << ", ";
			pseudoqueue.pop_back();

			newtemp->right = pseudoqueue.back();
			newtemp->right->parent = newtemp;
			newtemp->right->leftright = true;
			//std::cout << pseudoqueue.back()->value << ": " << pseudoqueue.back()->weight << std::endl;
			pseudoqueue.pop_back();

			newtemp->weight += newtemp->left->weight;
			newtemp->weight += newtemp->right->weight;
			
			bool placefound = false;
			auto place = pseudoqueue.begin();
			while (!placefound && place != pseudoqueue.end())
			{
				if ((*place)->weight < newtemp->weight)
				{
					placefound = true;
				}
				place++;
			}
			if (placefound)
			{
				place--;
				pseudoqueue.insert(place, newtemp);
			}
			else
				pseudoqueue.push_back(newtemp);
		}
	}
}

huffman_tree::~huffman_tree(){

}

/*
Preconditions: Character is a character with an ASCII value
				between 0 and 127 (inclusive).
Postconditions: Returns the Huffman code for character if character is in the tree
				and an empty string otherwise.
*/
std::string huffman_tree::get_character_code(char character) const 
{
	if (allleaves.size() == 1)
	{
		if(rootnode->value == character)
			return "0";
		else return "";
	}
	bool found = false;
	int count = 0;
	while (!found && count < (int)allleaves.size())
	{
		if (allleaves[count]->value == character)
		{
			found = true;
			count--;
		}
		count++;
	}
	if (found)
	{
		std::stack<char> reversi;
		huffmanNode *current = allleaves[count];
		while (current != rootnode)
		{
			if (current->leftright == true)
				reversi.push('1');
			else reversi.push('0');
			current = current->parent;
		}
		std::string out = "";
		while (!reversi.empty())
		{
			out += reversi.top();
			reversi.pop();
		}
		return out;
	}
	else
		return "";
}

/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Returns the Huffman encoding for the contents of file_name
				if file name exists and an empty string otherwise.
				If the file contains letters not present in the huffman_tree,
				return an empty string
*/
std::string huffman_tree::encode(const std::string &file_name) const 
{
	std::string out = "";
	std::string temp;

	std::ifstream in(file_name);
	std::string total((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());
	auto iter = total.begin();
	while (iter != total.end())
	{
		temp = get_character_code(*iter);
		if (temp.empty())
		{
			return "";
		}
		else
		{
			out += temp;
		}
		iter++;
	}
	return out;
}

/*
Preconditions: string_to_decode is a string containing Huffman-encoded text
Postconditions: Returns the plaintext represented by the string if the string
				is a valid Huffman encoding and an empty string otherwise
*/
std::string huffman_tree::decode(const std::string &string_to_decode) const 
{
	if (allleaves.size() == 1)
	{
		if (string_to_decode == "0")
		{
			std::string out = "";
			out += rootnode->value;
			return out;
		}
		else return "";
	}
	auto iter = string_to_decode.begin();
	std::string out = "";
	huffmanNode *current = rootnode;
	int switchcount = 0;

	while (iter != string_to_decode.end())
	{
		//std::cout << *iter << std::endl;
		if (*iter == (char)48)
		{
			switchcount++;
			if(current->left != nullptr)
				current = current->left;
			else return "";
		}
		else if (*iter == (char)49)
		{
			switchcount++;
			if(current->right != nullptr)
				current = current->right;
			else return "";
		}
		iter++;

		if (current->left == nullptr && current->right == nullptr)
		{
			std::string fodder(1,current->value);
			out.append(fodder);
			current = rootnode;
			switchcount = 0;
		}
	}
	if(switchcount > 0)
		return "";
	else return out;
}
