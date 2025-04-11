# Huffman Compressor-Decompressor

## Integrantes del equipo
- Gustavo Ortiz  
- Leonardo Velázquez

Este proyecto en C++ implementa un sistema completo de compresión y descompresión de archivos utilizando el algoritmo de **Huffman**. El programa compresor lee un archivo binario, genera el árbol de Huffman a partir de la frecuencia de los bytes, codifica el contenido en una secuencia de bits y guarda tanto el árbol serializado como los datos comprimidos en un archivo `.huffcomprimido`. Por su parte, el programa descompresor toma ese archivo, reconstruye el árbol desde su forma serializada, decodifica el contenido binario y recupera el archivo original. Este sistema permite una compresión sin pérdida eficiente, y es ideal para experimentar con algoritmos de compresión y estructuras de datos como árboles binarios.

## Requisitos

- Compilador decente como g++.
- Archivo `BinTree.h` (debe estar en el mismo directorio)

## Compilación

Puedes compilar los programas usando `g++`:

```bash
g++ -std=c++11 -o huffman_compressor comprimir_HuffmanTree.cxx
```

```bash
g++ -std=c++11 -o huffman_decompressor descomprimir_HuffmanTree.cxx
```

> Asegúrate de que `BinTree.h` esté correctamente implementado y accesible en tu directorio de trabajo.

## Ejecución
### Ejecución huffman_compressor

Para ejecutar el compresor, usa:

```bash
./huffman_compressor archivo_entrada
```

Por ejemplo:

```bash
./huffman_compressor prueba.txt
```

Esto generará un archivo comprimido con el nombre:

```
comprimido_prueba.txt.huffcomprimido
```
### Ejecución huffman_decompressor
Para ejecutar el descompresor, usa:

```bash
./huffman_decompressor nombre_original
```

Por ejemplo:

```bash
./huffman_decompressor prueba.txt
```

Este comando buscará un archivo llamado:

```
comprimido_prueba.txt.huffcomprimido
```

Y generará el archivo descomprimido como:

```
descomprimido_prueba.txt.txt
```


