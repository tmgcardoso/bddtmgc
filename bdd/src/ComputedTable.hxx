#ifndef BDD__COMPUTED_TABLE_HXX
#define BDD__COMPUTED_TABLE_HXX

#include "ComputedEntry.hxx"

#include <iostream>
#include <vector>

namespace bdd {

/**
 * ComputedTable class
 */
class ComputedTable
{
	public:
		// class constructor
		ComputedTable(const int size) : computed_table_(size,ComputedEntry()) { }
		// class destructor
		~ComputedTable() { std::cout << "~ComputedTable()" << std::endl; }
		
		inline void resize(const int size)
    { computed_table_.resize(size,ComputedEntry()); }
    
    inline int hash(const int id) const
    { return ComputedEntry::hash(id) % computed_table_.size(); }
    
    inline int hash(const int left, const int op,
                    const int right) const
    { return ComputedEntry::hash(left,op,right) % computed_table_.size(); }

    inline int lookupOpNot(const int id) const;

    inline int lookupOpAnd(const int left, const int right) const
    { return lookup(left,ComputedEntry::OP_AND,right); }

    inline int lookupOpOr(const int left, const int right) const
    { return lookup(left,ComputedEntry::OP_OR,right); }

    inline int lookupOpXor(const int left, const int right) const
    { return lookup(left,ComputedEntry::OP_XOR,right); }

    inline void setOpNot(const int id, const int result)
    { computed_table_[hash(id)].set(id,result); }

    inline void setOpAnd(const int left, const int right, const int result)
    { set(left,ComputedEntry::OP_AND,right,result); }
		
    inline void setOpOr(const int left, const int right, const int result)
    { set(left,ComputedEntry::OP_OR,right,result); }

    inline void setOpXor(const int left, const int right, const int result)
    { set(left,ComputedEntry::OP_XOR,right,result); }

	private:
          
    inline int lookup(const int left, const int op,
                      const int right) const;

    inline void set(const int left, const int op,
                    const int right, const int result);

    // data members
          
    std::vector<ComputedEntry> computed_table_;
};



inline int ComputedTable::lookup(const int left, const int op,
                                 const int right) const
{
  int result = -1;

  const ComputedEntry& entry = computed_table_[hash(left,op,right)];
  
  if (entry.left() == left && entry.op() == op && entry.right() == right)
    result = entry.result();
    
  return result;
}



inline int ComputedTable::lookupOpNot(const int id) const
{
  int result = -1;

  const ComputedEntry& entry = computed_table_[hash(id)];
  
  if (entry.left() == id && entry.op() == ComputedEntry::OP_NOT)
    result = entry.result();
    
  return result;
}



inline void ComputedTable::
set(const int left, const int op,
    const int right, const int result)
{
  computed_table_[hash(left,op,right)].
    set(left,op,right,result);
}



}

#endif // BDD__COMPUTED_TABLE_HXX
