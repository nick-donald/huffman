#include <cstdio>
#include <map>
#include <fstream>
#include <string>
#include <iterator>
#include <queue>
#include "node.hpp"

typedef std::map<char, int> freq_map_t;
typedef std::map<char, std::string> codemap;

bool debug;

freq_map_t build_freq_list(std::string str)
{
  freq_map_t frequency_map;

  for (std::string::iterator i = str.begin(); i != str.end(); i++) {
    if (frequency_map.count(*i)) {
      frequency_map[*i] ++;
    }
    else {
      frequency_map[*i] = 1;
    }
  }

  return frequency_map;

}

class compare
{
  public:
    bool operator()(Node *n1, Node *n2)
    {
      if (n1->frequency == n2->frequency)
      {
        return (n1->value > n2->value);
      }
      else
      {
        return (n1->frequency > n2->frequency);
      }
    }
};

void generate_codes(Node *root, std::string str, codemap *cmap)
{
  if (!root)
    return;

   if (root->value != "*"[0])
     (*cmap)[root->value] = str;
     std::cout << "CODED VAL: " << root->value << ": " << str << "\n";


  if (debug)
  {
    std::cout << "root :" << root->value << ": " << root->frequency << "\n";

    if (root->leftNode)
      std::cout << root->leftNode->value << ": " << root->leftNode->frequency << "\n";

    if (root->rightNode)
      std::cout << root->rightNode->value << ": " << root->rightNode->frequency << "\n";
  }

  generate_codes(root->leftNode, str + "0", cmap);
  generate_codes(root->rightNode, str + "1", cmap);
}

Node* build_node_tree(freq_map_t map)
{
  freq_map_t::iterator iter = map.begin();
  std::priority_queue<Node*, std::vector<Node*>, compare> node_queue;

  Node *left, *right, *top;

  while (iter != map.end())
  {
    node_queue.push(new Node(iter->second, iter->first));
    iter ++;
  }

  while (node_queue.size() > 1)
  {

    left = node_queue.top();
    node_queue.pop();

    right = node_queue.top();
    node_queue.pop();

    // std::cout << "left: " << left->value << " : " << left->frequency << "\n";
    // std::cout << "right: " << right->value << " : " << right->frequency << "\n";

    top = new Node(left->frequency + right->frequency, "*"[0]);
    top->leftNode = left;
    top->rightNode = right;


    node_queue.push(top);
  }

  // std::cout << "generating for: " << top->frequency << " | " << top->value << "\n";

  // generate_codes(node_queue.top(), "");
  return node_queue.top();
}

std::string encode_string(std::string str, codemap *cmap, std::string *tree_str)
{
  std::string encoded;
  std::string packed;
  std::string buf;

  encoded += *tree_str;

  for (std::string::iterator iter = str.begin(); iter < str.end(); iter++)
  {
    encoded += cmap->at(*iter);
  }

  size_t hang_bits = encoded.length() & 7;
  packed += static_cast<char>(hang_bits);

  size_t tree_str_len = tree_str->length();
  packed += static_cast<char>(tree_str_len);

  for (int i = 0; i < encoded.length(); i++)
  {
    int loc = i & 7;
    buf += encoded[i];
    if (i == (encoded.length() - 1)) {
      for (int j = 0; j < (8 - hang_bits); j++)
      {
        buf += "0";
      }
      std::bitset<8> byte(buf);
      packed += static_cast<char>(byte.to_ulong());
    } else if (loc == 7) {
      // std::cout << "packing: " << buf << std::endl;
      std::bitset<8> byte(buf);
      packed += static_cast<char>(byte.to_ulong());
      buf = "";
    }
  }

  return packed;
}

void hexdump(const unsigned char *bytes, int nbytes) {
    int i, j;

    for (i = 0; i < nbytes; i += 16) {
        printf("%06x: ", i);
        for (j = 0; j < 16; j++) {
            if (i + j < nbytes) {
                printf("%02x ", bytes[i + j]);
            } else {
                printf("   ");
            }
        }
        printf(" ");
        for (j = 0; j < 16; j++)
            if (i + j < nbytes)
                printf("%c", isprint(bytes[i+j]) ? bytes[i+j] : '.');
        printf("\n");
    }
}

void write_file(std::string encoded, std::ofstream *fileout)
{

}

std::string file_to_str(std::string fname)
{
  std::ifstream filein(fname);
  std::string str;

  filein.seekg(0, std::ios::end);
  str.reserve(filein.tellg());
  filein.seekg(0, std::ios::beg);
  str.assign(std::istreambuf_iterator<char>(filein), std::istreambuf_iterator<char>());

  return str;
}

char char_from_node_tree(std::string bufin, Node *root)
{
  Node *n = root;

  for (std::string::iterator striter = bufin.begin(); striter != bufin.end(); striter++)
  {
    if (*striter == "0"[0]) {
      n = n->leftNode;
    } else {
      n = n->rightNode;
    }
  }

  return n->value;
}

std::string decode_string(std::string encoded_str, Node *root)
{
  std::string encoded_buf;
  encoded_buf.reserve(255);

  for (std::string::iterator iter = encoded_str.begin() + 1; iter != encoded_str.end(); iter++)
  {
    std::bitset<8> key(*iter);
    encoded_buf += key.to_string();
  }

  std::string buf;
  std::string out;
  for (std::string::iterator iterr = encoded_buf.begin(); iterr != encoded_buf.end(); iterr++)
  {
    buf += *iterr;
    char val = char_from_node_tree(buf, root);

    if (val != "*"[0]) {
      out += val;
      buf = "";
    }
  }

  return out;
}

// TODO sort frequency map by frequency
int main(int argc, char *argv[])
{

  codemap cmap;
  std::string test = "Hello this is a test of the compressor";

  std::string fname = argv[1];

  std::string file_str = file_to_str(fname);

  freq_map_t map = build_freq_list(file_str);
  Node *root = build_node_tree(map);
  std::string tree_str = root->to_string("0");
  generate_codes(root, "", &cmap);

  std::string encoded = encode_string(file_str, &cmap, &tree_str);

  std::cout << "Node tree string: " << tree_str << std::endl;

  std::ofstream fileout;
  fileout.open("compressed.huffzip");
  fileout << encoded;
  fileout.close();

  std::string encoded_f = file_to_str("compressed.huffzip");

  // TODO construct tree from decoded output
  // 1. get number of hang bits
  // 2. get num bits in encoded tree
  // 3. extract encoded tree from encoded string
  // 4. construct tree
  // 5. decode from constructed tree
  std::string decoded = decode_string(encoded_f);

  std::cout << decoded << std::endl;

  // hexdump((unsigned char *)(encoded.c_str()), encoded.size());
}
