#ifndef BDD__UNIQUE_TABLE_HXX
#define BDD__UNIQUE_TABLE_HXX

#include <iostream>
#include <vector>
#include <map>
#include <cassert>

namespace bdd {
          
typedef std::map<std::pair<int,int>,int> LevelMap;

/**
 * UniqueTable class
 */
class UniqueTable
{
public:
       
    inline ~UniqueTable();
		
		inline int find(const int index, const int high, const int low) const;
		
		inline int extend(const int size = 1);
		
		inline bool
    add(const int index, const int high, const int low, const int root);
    
    inline LevelMap& levelMap(const int level)
    { return *(unique_table_[level]); }
		
    inline const LevelMap& levelMap(const int level) const
    { return *(unique_table_[level]); }
		
private:
        
   std::vector<LevelMap*> unique_table_;
};


inline UniqueTable::~UniqueTable()
{
  std::cout << "~UniqueTable()" << std::endl;
  for (std::vector<LevelMap*>::iterator it = unique_table_.begin();
    it != unique_table_.end(); ++it)
    delete *it;
    
  //std::cout << "all node id's erased successfully" << endl;
}


inline int UniqueTable::
find(const int index, const int high, const int low) const
{
  assert(index >= 0 && index < unique_table_.size());

  const LevelMap& levelMap = *(unique_table_[index]); 

  LevelMap::const_iterator it =
    levelMap.find(std::make_pair(high,low));

  return it != levelMap.end() ?
    it->second : -1;
}



inline int UniqueTable::extend(const int size)
{
  assert(size > 0);

  for (int i = 0; i < size; ++i)
    unique_table_.push_back(new LevelMap());

  return unique_table_.size();
}


inline bool UniqueTable::
add(const int index, const int high, const int low, const int id)
{
  assert(index >= 0 && index < unique_table_.size());
  
  return unique_table_[index]->
    insert(std::make_pair(std::make_pair(high,low),id)).second;
}


}

#endif // BDD__UNIQUE_TABLE_HXX
