#include "node.hpp"
#include <bitset>

const int NODE_VAL = 0x2A;

Node::Node(int freq, char val)
{
  frequency = freq;
  value = val;
  leftNode = rightNode = NULL;
}

bool Node::is_node()
{
  return value == "*"[0];
}

std::string Node::to_string(std::string output)
{

  if (is_node()) {
    if (leftNode->is_node()) {
      output += "0";
      output += leftNode->to_string();
    } else if (!leftNode->is_node()) {
      output += "1";
      output += std::bitset<8>(leftNode->value).to_string();
    }

    if (rightNode->is_node()) {
      output += "0";
      output += rightNode->to_string();
    } else if (!rightNode->is_node()) {
      output += "1";
      output += std::bitset<8>(rightNode->value).to_string();
    }
  }

  return output;
}

Node* Node::from_string(std::string *input, Node *node)
{
  if (node == NULL) {
    node = new Node(0, NODE_VAL);
  }

  Node *curr_node = node;

  for (int i = 1; i < input->length(); i++) {
    std::cout << input->at(i) << std::endl;

    if (input->at(i) == NODE_VAL) {
      if (curr_node->leftNode == NULL) {
        curr_node->leftNode = new Node(0, NODE_VAL);
        curr_node = curr_node->leftNode;
      } else {
        curr_node->rightNode = new Node(0, NODE_VAL);
        curr_node = curr_node->rightNode;
      }
    } else {
      std::bitset<8> chr(input->substr(i + 1, i + 8));
      std::cout << "char: " << input->substr(i + 1, i + 8) << std::endl;
      char value = static_cast<char>(chr.to_ulong());

      if (curr_node->leftNode == NULL) {
        curr_node->leftNode = new Node(0, value);
      } else {
        curr_node->rightNode = new Node(0, value);
      }
      // std::cout << value << std::endl;

      i = i + 9;
    }
  }
  return node;
}
