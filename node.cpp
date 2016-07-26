#include "node.hpp"
#include <bitset>

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
