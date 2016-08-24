#include "node.hpp"
#include <bitset>
#include <stack>

const int NODE_VAL   = 0x2A;

const char NODE_ZERO = 0x30; // "0"
const char NODE_ONE  = 0x31; // "1"

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

bool Node::is_full()
{
  return leftNode != NULL && rightNode != NULL;
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

  std::stack<Node *> node_stack;

  node_stack.push(node);

  for (int i = 1; i < input->length(); i++) {

    if (input->at(i) == NODE_ZERO) {
      Node *curr_node = new Node(0, NODE_VAL);

      if (node_stack.top() != NULL) {
        Node *parent_node = node_stack.top();

        if (parent_node->leftNode == NULL) {
          parent_node->leftNode = curr_node;
        } else {
          parent_node->rightNode = curr_node;
        }
      }
      node_stack.push(curr_node);
    } else {
      Node *parent_node = node_stack.top();

      std::bitset<8> chr(input->substr(i + 1, i + 8));
      char value = static_cast<char>(chr.to_ulong());

      if (parent_node->leftNode == NULL) {
        parent_node->leftNode = new Node(0, value);
      } else if (parent_node->rightNode == NULL) {
        parent_node->rightNode = new Node(0, value);
      }

      if (parent_node->is_full()) node_stack.pop();

      i += 8;
    }
  }

  return node;
}
