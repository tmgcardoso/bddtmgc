
#ifndef BDD__COMPUTED_ENTRY_HXX
#define BDD__COMPUTED_ENTRY_HXX

#include <iostream>
#include <climits>


namespace bdd {

/**
 * ComputedEntry class
 */
class ComputedEntry
{

public:

  enum OpCode {
    OP_NOT = 1,
    OP_AND = 2,
    OP_OR = 3,
    OP_XOR = 5
  };

	// class constructor
	ComputedEntry() : left_(INT_MIN) { }

	// copy constructor
	ComputedEntry(const ComputedEntry& entry) : left_(entry.left_) { }

	// class destructor
	~ComputedEntry() { /* cout << "~ComputedEntry()" << endl; */ }

	// class operator = (should not be called)
	// must be public for the STL vector container
	ComputedEntry& operator = (const ComputedEntry&) { }
	
	inline void set(const int left, const int op, const int right, const int result)
	{ left_ = left; op_ = op; right_ = right; result_ = result; }
	
	inline void set(const int left, const int result)
	{ left_ = left; op_ = OP_NOT; result_ = result; }

	inline static int hash(const int left, const int op, const int right)
  { return left * 378551 + op * 63689 + right * 271; }

	inline static int hash(const int id)
	{ return id * 378551 * 263; }
	
	inline int left() const { return left_; }
	
	inline int op() const { return op_; }
	
	inline int right() const { return right_; }
	
	inline int result() const { return result_; }

protected:

private:

  int left_;
  int op_;
  int right_;
  int result_;

}; // class ComputedEntry




} // namespace bdd

#endif // BDD__COMPUTED_ENTRY_HXX
