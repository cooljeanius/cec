#line 761 "IR.nw"
#ifndef _IR_HPP
#  define _IR_HPP

#  include <expat.h>
#  include <string>
#  include <map>
#  include <stack>
#  include <vector>
#  include <iostream>

#line 75 "IR.nw"
#  define IRCLASSDEFS \
  public: \
    static IR::Class _; \
    virtual const std::string className() const { return _; }
#line 147 "IR.nw"
#  define IRCLASS(s) IR::Class s::_ = IR::Class( #s, IR::constructor<s> )

#line 774 "IR.nw"
namespace IR {
  class Node;
  class XMListream;
  class XMLostream;

  
#line 750 "IR.nw"
  struct Error {
    std::string s;
    Error(std::string ss) : s(ss) {}
  };
#line 780 "IR.nw"
  
#line 122 "IR.nw"
  // Information for the classes in the IR: a string naming each,
  // and a map giving the constructor for each class
  class Class {
    typedef Node *(*createfunc)();
    typedef std::map<std::string, createfunc> createfuncmap;
    static createfuncmap *classmap;
  public:
    Class(std::string, createfunc);
    static Node *newNodeByName(const std::string);

  private:
    const std::string _className;
  public:
    operator const std::string () { return _className; }
  };
#line 781 "IR.nw"
  
#line 51 "IR.nw"
  class Node {
    friend class XMLostream;
    friend class XMListream;
  protected:
    virtual void read(XMListream &) {}
    virtual void write(XMLostream &) const {}
  public:
    static IR::Class _;
    virtual const std::string className() const { return _; }
    virtual ~Node() {}
  };
#line 782 "IR.nw"
  
#line 220 "IR.nw"
  class XMLostream {
    typedef std::map<const Node *, unsigned int> idmap;
    idmap ID;

    unsigned int nextID;
    std::ostream &o;
    
  public:
    XMLostream(std::ostream &oo) : nextID(0), o(oo) {}
    XMLostream& operator <<(const Node&);
    XMLostream& operator <<(const Node *);
    XMLostream& operator <<(const std::string);
    XMLostream& operator <<(int);
    XMLostream& operator <<(bool);
    
#line 337 "IR.nw"
    template <class T> XMLostream& operator <<(const std::vector<T*> &v) {
      typename std::vector<T*>::const_iterator i;
      for ( i = v.begin() ; i != v.end() ; i++ ) (*this) << *i;
      o << "<EOV/>" << std::endl;
      return *this;
    }
#line 235 "IR.nw"
    
#line 349 "IR.nw"
    template <class K, class V>
     XMLostream& operator <<(const std::map<K, V> &m) {
      typename std::map<K, V>::const_iterator i;
      for ( i = m.begin() ; i != m.end() ; i++ ) 
        (*this) << (*i).first << (*i).second;
      o << "<EOM/>" << std::endl;
      return *this;
    }
#line 236 "IR.nw"
  };
#line 783 "IR.nw"
  
#line 429 "IR.nw"
  struct XMLNode {
    std::string name;
    std::string body;

    typedef std::map<const std::string, const std::string> attrmap;
    attrmap attrs;
    
    XMLNode *first;
    XMLNode *next;

    XMLNode() : first(0), next(0) {}

    ~XMLNode() { delete first; delete next; }

    void print();
  };
#line 784 "IR.nw"
  
#line 390 "IR.nw"
  class XMListream {
    std::stack<XMLNode*> parents;
    XMLNode *lastsibling;
    XMLNode *current;
    XMLNode *root;

    static void startElement(void *, const char *, const char **);
    static void endElement(void *, const char *);
    static void charData(void *, const XML_Char *, int);
    void attachSibling(XMLNode *);

    // Map that tracks Nodes with IDs
    typedef std::map<const std::string, Node *> nodemap;
    nodemap nodeofid;

    Node *getNextNode();

  public:
    XMListream(std::istream &);
    ~XMListream() { delete root; }

    
#line 658 "IR.nw"
    template <class T> XMListream& operator >>(T* &f) {
      Node *n = getNextNode();
      f = (n != NULL) ? dynamic_cast<T*>(n) : NULL;
      if (!f && n) throw Error("Unexpected element " + n->className());
      return *this;
    }
#line 412 "IR.nw"
    
#line 638 "IR.nw"
  template <class N> XMListream& operator >>(N &n) {
    if ( current->name != std::string(N::_) ) // Check name of this class
      throw Error("Unexpected element " + current->name);
    parents.push(current);
    current = current->first;
    n.read(*this); // Fill in the object's fields
    if (current != NULL)
      throw Error("excess elements for " + std::string(N::_));
    current = parents.top();
    parents.pop();
    current = current->next;
    return *this;
  }
#line 413 "IR.nw"
    XMListream& operator >>(std::string&);
    XMListream& operator >>(int &);
    XMListream& operator >>(bool &);
    
#line 713 "IR.nw"
    template <class T> XMListream& operator >>(std::vector<T> &v) {
      while (current && current->name != "EOV") {
         T value;
         (*this) >> value;
         v.push_back(value);
      }
      if (!current) throw Error("vector ended without EOV");
      current = current->next;  // Skip the EOV Element
      return *this;
    }
#line 417 "IR.nw"
    
#line 729 "IR.nw"
    template <class K, class V> XMListream& operator >>(std::map<K, V> &m) {
      while (current && current->name != "EOM") {
         K key;
         V value;
	 (*this) >> key >> value;
         m.insert( typename std::map<K,V>::value_type(key, value) );
      }
      if (!current) throw Error("map ended without EOM");
      current = current->next;  // Skip the EOM Element
      return *this;
    }
#line 418 "IR.nw"
  };
#line 785 "IR.nw"
  
#line 151 "IR.nw"
template<class T> Node* constructor() { return new T(); }
#line 786 "IR.nw"
}
#endif
