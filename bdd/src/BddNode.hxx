
#ifndef BDD__BDD_NODE_HXX
#define BDD__BDD_NODE_HXX

#include <iostream>
#include <sstream>
#include <vector>

namespace bdd {
          
/**
 * BddNode class
 */
class BddNode
{
public:
  // class constructor
	//inline BddNode(const int index, const int high, const int low) :
  //  index_(index), high_(high), low_(low) { }

  BddNode() { } // cout << "BddNode :: BddNode()" << endl; }

  BddNode(const BddNode& node) { } // cout << "BddNode" << node.toString() << endl; }

	// class destructor
	~BddNode() { } // */ cout << "BddNode :: ~BddNode() -> " << toString() << endl; }
	
  // assignment operator (should not be called)
  BddNode& operator = (const BddNode&)
  { /*cout << "BddNode :: BddNode& operator = (const BddNode&)" << endl;*/ }
                                   
  int index() const { return index_; }
  
  int high() const { return high_; }
  
  int low() const { return low_; }
  
  void set(const int index, const int high, const int low)
  { index_ = index; high_ = high; low_ = low;
    /*cout << "set(" << index << "," << high << "," << low << ")" << endl;*/ }

  std::string toString() const;

protected:
		
private:

  int index_;
  int high_;
  int low_;
  int ref_count_;
};


inline std::string BddNode::toString() const
{
  std::ostringstream o;
  
  o << "(" << index_ << "," << high_ << "," << low_ << ")";
  
  return o.str();
}


}

#endif // BDD__BDD_NODE_HXX
