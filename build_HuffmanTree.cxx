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
    //std::cout << e << " " << P[e] << std::endl;
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

void codificar(const TTree* node, const std::string& path, std::vector<std::string>& codes) {
  if (!node) return;

  if (node->is_leaf()) {
      codes[node->value().second] = path;
  } else {
      codificar(node->left(), path + "0", codes);
      codificar(node->right(), path + "1", codes);
  }
}

std::string decodificar(const std::string& bitstream, const TTree* tree) {
  std::string result;
  const TTree* node = tree;

  for (char bit : bitstream) {
      node = (bit == '0') ? node->left() : node->right();

      if (node->is_leaf()) {
          result += node->value().second;
          node = tree;
      }
  }

  return result;
}


void guardar_comprimido(const std::vector<uint8_t>& arbol_serializado, const std::string& bitstream, const std::string& nombre_archivo) {
  std::ofstream out(nombre_archivo, std::ios::binary);

  // Se guarda el tamaño del árbol
  uint32_t tam_arbol = arbol_serializado.size();
  out.write(reinterpret_cast<const char*>(&tam_arbol), sizeof(tam_arbol));

  // Se guarda el árbol serializado
  out.write(reinterpret_cast<const char*>(arbol_serializado.data()), tam_arbol);

  // Se convierte el bitstream (string de '0' y '1') a bytes
  std::vector<uint8_t> data_bytes;
  for (size_t i = 0; i < bitstream.size(); i += 8) {
      std::string byte_str = bitstream.substr(i, 8);
      while (byte_str.length() < 8) byte_str += '0'; // Padding si el último byte es incompleto
      uint8_t byte = static_cast<uint8_t>(std::bitset<8>(byte_str).to_ulong());
      data_bytes.push_back(byte);
  }

  // Se guarda el tamaño real del bitstream (para decodificar correctamente)
  uint32_t bits_validos = bitstream.size();
  out.write(reinterpret_cast<const char*>(&bits_validos), sizeof(bits_validos));

  // Se guarda los bytes codificados
  out.write(reinterpret_cast<const char*>(data_bytes.data()), data_bytes.size());

  out.close();
}


void decomprimir_archivo(const std::string& archivo, std::string& texto_resultado) {
  std::ifstream in(archivo, std::ios::binary);

  // Leer tamaño del árbol
  uint32_t tam_arbol;
  in.read(reinterpret_cast<char*>(&tam_arbol), sizeof(tam_arbol));

  // Leer árbol serializado
  std::vector<uint8_t> arbol_serializado(tam_arbol);
  in.read(reinterpret_cast<char*>(arbol_serializado.data()), tam_arbol);

  // Leer tamaño del bitstream
  uint32_t bits_validos;
  in.read(reinterpret_cast<char*>(&bits_validos), sizeof(bits_validos));

  // Leer el resto (el bitstream codificado como bytes)
  std::vector<uint8_t> data_bytes;
  char c;
  while (in.get(c)) {
      data_bytes.push_back(static_cast<uint8_t>(c));
  }
  in.close();

  // Convertir los bytes de vuelta al bitstream
  std::string bitstream;
  for (uint8_t byte : data_bytes) {
      std::bitset<8> bits(byte);
      bitstream += bits.to_string();
  }
  bitstream = bitstream.substr(0, bits_validos); // Quitar padding final

  // Reconstruir el árbol
  size_t index = 0;
  TTree* tree = reconstruir_huffman(arbol_serializado, index);

  // Decodificar texto
  texto_resultado = decodificar(bitstream, tree);

  delete tree;
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
  //tree->print_preorder(std::cout);

  std::vector<uint8_t> s;
  std::string bitstream;

  tree->serializar_arbol(s);
  std::cout << "==============================" << std::endl;
  std::cout << "Serializacion de arbol" << std::endl;
  std::cout << "Contenido del arbol en bytes: ";

  for (uint8_t byte : s)
    std::cout << static_cast<int>(byte) << ' ';


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

  std::cout << "==============================" << std::endl;

  std::cout << "==============================" << std::endl;
  //Codificar el texto original

  std::vector<std::string> codes(256);
  codificar(tree, "", codes);

  std::string encoded_text;
  for (unsigned char c : data) {
      encoded_text += codes[c];
  }

  std::cout << "Texto codificado en bits: " << encoded_text << std::endl;

  std::cout << "==============================" << std::endl;

  std::cout << "==============================" << std::endl;



  size_t index = 0;
  TTree* restored_tree = reconstruir_huffman(s, index);

  //::cout << "Árbol reconstruido desde los bytes:" << std::endl;
  //restored_tree->print_preorder(std::cout);
  // Paso 4: Decodificar usando el árbol restaurado
  std::string decoded_text = decodificar(encoded_text, restored_tree);

  std::cout << "Texto decodificado: " << decoded_text << std::endl;

  std::cout << "==============================" << std::endl;

  std::cout << "==============================" << std::endl;

  guardar_comprimido(s, encoded_text, "comprimido.huffcomprimido");
  std::cout << "Archivo comprimido como: " << "comprimido.huffcomprimido" << std::endl;

  std::string resultado;
  decomprimir_archivo("comprimido.huffcomprimido", resultado);

  std::ofstream out("descomprimido.txt", std::ios::binary);
  out.write(resultado.data(), resultado.size());
  out.close();

  std::cout << "Texto descomprimido en: descomprimido.txt\n";
  delete restored_tree;


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
