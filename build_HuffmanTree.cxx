#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "BinTree.h"

#include "algorithm"

#include <bitset>

// -----------------------------------------------------------------------
using TElement = unsigned char;
using TFrequency = long long;
using TTree = BinTree<std::pair<TFrequency, TElement>>;

// -----------------------------------------------------------------------
TTree *BuildHuffman(const std::vector<long long> &P)
{
  auto cmp = [](const TTree *a, const TTree *b) -> bool
  {
    return (b->value().first < a->value().first);
  };

  std::vector<TTree *> q;
  for (int e = 0; e < P.size(); ++e)
  {
    std::cout << e << " " << P[e] << std::endl;
    TTree *n = new TTree(std::make_pair(P[e], TElement(e)));
    q.push_back(n);
    std::push_heap(q.begin(), q.end(), cmp);
  } // end for

  while (q.size() > 1)
  {
    std::pop_heap(q.begin(), q.end(), cmp);
    TTree *r = q.back();
    q.pop_back();

    std::pop_heap(q.begin(), q.end(), cmp);
    TTree *l = q.back();
    q.pop_back();

    TTree *n = new TTree(std::make_pair(l->value().first + r->value().first, 0));
    n->set_left(l);
    n->set_right(r);
    q.push_back(n);
    std::push_heap(q.begin(), q.end(), cmp);

  } // end while
  return (q[0]);
}

// -----------------------------------------------------------------------
void process(const std::string &fname)
{
  std::ifstream fstr(fname.c_str(), std::ios::binary | std::ios::ate);
  long long size = fstr.tellg();
  fstr.seekg(0, std::ios::beg);

  std::vector<unsigned char> data(size);
  char *buffer = reinterpret_cast<char *>(data.data());
  fstr.read(buffer, data.size());
  fstr.close();

  // Build histograms
  std::vector<long long> P(256, 0);
  for (long long i = 0; i < data.size(); ++i)
    P[data[i]] += 1;

  // Build tree
  TTree *tree = BuildHuffman(P);

  // Get most and least repeated data
  long long most = 0, least = 0;
  for (long long i = 1; i < P.size(); ++i)
  {
    if (P[most] < P[i])
      most = i;
    if (P[i] < P[least])
      least = i;
  } // end for

  // Some data
  std::cout << "==============================" << std::endl;
  std::cout << "Preoder" << std::endl;
  std::cout << "==============================" << std::endl;
  tree->print_preorder(std::cout);

  std::vector<uint8_t> s;
  std::string bitstream;

  tree->store_tree(s);
  std::cout << "==============================" << std::endl;
  std::cout << "STORE TREE" << std::endl;
  std::cout << "Tree bytes as numeric values: ";
  for (uint8_t byte : s)
    std::cout << static_cast<int>(byte) << ' ';

    for (uint8_t byte : s) {
        std::bitset<8> bits(byte);
        bitstream += bits.to_string(); // Adds 8 chars: '0' or '1'
    }

    std::cout << bitstream;

  std::cout << std::endl;

  std::cout << "==============================" << std::endl;

  std::cout << "==============================" << std::endl;
  std::cout << "Number of tokens: " << P.size() << std::endl;
  std::cout << "Height: " << tree->height() << std::endl;
  std::cout << "Weight: " << tree->weight() << std::endl;
  std::cout
      << "Theoretical height: "
      << std::ceil(std::log2(tree->height()))
      << std::endl;
  std::cout << "Most repeated element: " << most << " (f = " << P[most] << ")" << std::endl;
  std::cout << "Least repeated element: " << least << " (f = " << P[least] << ")" << std::endl;

  // Finish
  delete tree;
}

// -----------------------------------------------------------------------
int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " input" << std::endl;
    return (EXIT_FAILURE);
  }
  else
  {
    std::string fname = argv[1];
    process(fname);
    return (EXIT_SUCCESS);
  } // end if
}

// eof - build_OptOrdBinTree.cxx
