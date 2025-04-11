#ifndef __BinTree__h__
#define __BinTree__h__

/**
 */
template <class _T>
class BinTree
{
public:
  using Self = BinTree;

public:
  BinTree(const _T &v)
      : m_V(v)
  {
  }
  virtual ~BinTree()
  {
    if (this->m_L != nullptr)
      delete this->m_L;
    if (this->m_R != nullptr)
      delete this->m_R;
  }

  const _T &value() const
  {
    return (this->m_V);
  }

  bool is_leaf() const {
    return (m_L == nullptr && m_R == nullptr);
  }
  
  Self* left() const {
    return m_L;
  }
  
  Self* right() const {
    return m_R;
  }
  

  void set_left(Self *l)
  {
    if (this->m_L != nullptr)
      delete this->m_L;
    this->m_L = l;
  }

  void set_right(Self *l)
  {
    if (this->m_R != nullptr)
      delete this->m_R;
    this->m_R = l;
  }

  long long height() const
  {
    long long l = (this->m_L != nullptr) ? this->m_L->height() : 0;
    long long r = (this->m_R != nullptr) ? this->m_R->height() : 0;
    return (1 + ((r < l) ? l : r));
  }
  long long weight() const
  {
    long long l = (this->m_L != nullptr) ? this->m_L->weight() : 0;
    long long r = (this->m_R != nullptr) ? this->m_R->weight() : 0;
    return (1 + l + r);
  }

  void print_preorder(std::ostream &o, const std::string &indent = "") const
  {
    o << indent << this->m_V.second << std::endl;
    if (this->m_L != nullptr)
      this->m_L->print_preorder(o, indent + " ");
    else
      o << indent << " +" << std::endl;
    if (this->m_R != nullptr)
      this->m_R->print_preorder(o, indent + " ");
    else
      o << indent << " +" << std::endl;
  }

  //*--*-*-*-*-*-*-*TALLER*--*-*-*-*-*-*-*
  void serializar_arbol(std::vector<uint8_t> &s)
  {
    if (this->m_L != nullptr && this->m_R != nullptr)
      s.push_back(static_cast<uint8_t>('0'));

    if (this->m_L != nullptr)
      this->m_L->serializar_arbol(s);

    if (this->m_R != nullptr)
      this->m_R->serializar_arbol(s);

    // Store leaf node
    if (this->m_L == nullptr && this->m_R == nullptr)
    {
      s.push_back(static_cast<uint8_t>('1'));
      s.push_back(this->m_V.second);
    }
  }

protected:
  _T m_V;
  Self *m_L{nullptr};
  Self *m_R{nullptr};
};

#endif // __BinTree__h__

// eof - BinTree.h
