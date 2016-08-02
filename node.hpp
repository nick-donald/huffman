#include <iostream>

#ifndef Node_H
#define Node_H
#endif

class Node {
  public:
    Node *leftNode, *rightNode;
    int frequency;
    char value;

    Node(int freq, char val);

    Node* operator+(Node &right)
    {
      Node *ret = new Node(frequency + right.frequency, "*"[0]);
      ret->leftNode = this;
      ret->rightNode = &right;
      return ret;
    }

    std::string to_string(std::string output = "");

    bool is_node();

    static Node* from_string(std::string *input, Node *node = NULL);
};
