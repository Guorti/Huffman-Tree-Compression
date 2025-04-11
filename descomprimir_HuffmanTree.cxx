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

TTree* reconstruir_huffman(const std::vector<uint8_t>& s, size_t& index) {
  if (index >= s.size()) return nullptr;

  uint8_t token = s[index++];

  if (token == '1') {
      uint8_t ch = s[index++];
      return new TTree(std::make_pair(0LL, ch));  // solo importa el carácter
  } else if (token == '0') {
      TTree* left = reconstruir_huffman(s, index);
      TTree* right = reconstruir_huffman(s, index);
      TTree* node = new TTree(std::make_pair(0LL, 0));
      node->set_left(left);
      node->set_right(right);
      return node;
  }

  return nullptr; // Error de formato
}


std::string decodificar(const std::string& encoded_text, const TTree* tree) {
  std::string resultado;
  const TTree* node = tree;

  for (char bit : encoded_text) {
      node = (bit == '0') ? node->left() : node->right();

      if (node->is_leaf()) {
          resultado += node->value().second;
          node = tree;
      }
  }

  return resultado;
}


void decomprimir_archivo(const std::string& archivo, std::string& texto_resultado) {
  std::ifstream in(archivo, std::ios::binary);

  // Se lee el tamaño del árbol
  uint32_t tam_arbol;
  in.read(reinterpret_cast<char*>(&tam_arbol), sizeof(tam_arbol));

  // Se lee el árbol serializado
  std::vector<uint8_t> arbol_serializado(tam_arbol);
  in.read(reinterpret_cast<char*>(arbol_serializado.data()), tam_arbol);

  // Se lee el tamaño del bitstream
  uint32_t bits_validos;
  in.read(reinterpret_cast<char*>(&bits_validos), sizeof(bits_validos));

  // Se lee el resto (el bitstream codificado como bytes)
  std::vector<uint8_t> data_bytes;
  char c;
  while (in.get(c)) {
      data_bytes.push_back(static_cast<uint8_t>(c));
  }
  in.close();

  // Se convierten los bytes de vuelta al texto comprimido
  std::string encoded_text;
  for (uint8_t byte : data_bytes) {
      std::bitset<8> bits(byte);
      encoded_text += bits.to_string();
  }
  encoded_text = encoded_text.substr(0, bits_validos); // Quitar padding final

  // Se reconstruye el árbol
  size_t index = 0;
  TTree* tree = reconstruir_huffman(arbol_serializado, index);

  // Se decodifica el texto
  texto_resultado = decodificar(encoded_text, tree);

  delete tree;
}





// -----------------------------------------------------------------------
void process(const std::string &fname)
{
  

  // Read file
  std::cout << "==============================" << std::endl;

  std::string resultado;
  std::string archivo = "comprimido_"+fname+".huffcomprimido";

  decomprimir_archivo(archivo, resultado);

  std::ofstream out("descomprimido_"+fname+".txt", std::ios::binary);
  out.write(resultado.data(), resultado.size());
  out.close();

  std::cout << "Texto descomprimido en: descomprimido_"+fname+".txt\n";
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
