#ifndef BDD__BDD_MANAGER_HXX
#define BDD__BDD_MANAGER_HXX

#include "BddNode.hxx"
#include "UniqueTable.hxx"
#include "ComputedTable.hxx"

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <set>

namespace bdd {

/**
 * BddManager class
 */
class BddManager
{
public:
  // class constructor
  inline BddManager();
		
  // compiler generated copy constructor
		
  // class destructor
  inline ~BddManager();
		
  // compiler generated assignment operator
		
  int variableCount() const { return var_to_level_.size(); }

  inline void createVariables(const int nVariables);
		
  inline int variable(const int nVariable) const;
		
  int varToLevel(const int index) const
  { return isValidVariableIndex(index) ? varToLevelCore(index) : -1; }

  int level2var(const int level) const
  { return isValidVariableIndex(level) ? level2varCore(level) : -1; }

  bool isValidNodeId(const int id) const
  { return id >= 0 && id < node_count_; }
    
  int index(const int id) const
  { return isValidNodeId(id) ? indexCore(id) : -1; }

  int high(const int id) const
  { return isValidNodeId(id) ? highCore(id) : -1; }
    
  int low(const int id) const
  { return isValidNodeId(id) ? lowCore(id) : -1; }
    
  int opNot(const int id)
  { return isValidNodeId(id) ? opNotCore(id) : -1; }
		
  int opAnd(const int left, const int right) {
    return isValidNodeId(left) && isValidNodeId(right) ?
        opAndCore(left,right) : -1;
  }

  int opOr(const int left, const int right) {
    return isValidNodeId(left) &&
        isValidNodeId(right) ? opOrCore(left,right) : -1;
  }
    
  int opXor(const int left, const int right) {
    return isValidNodeId(left) &&
        isValidNodeId(right) ? opXorCore(left,right) : -1;
  }

  void swapLevels(const int parentLevel) {
    if(parentLevel < (var_to_level_.size() - 1)) {
      swapLevelsCore(parentLevel);
    }
  }
    
  void moveFromLevelTo(const int fromLevel, const int toLevel) {
    if (isValidVariableIndex(fromLevel) &&
        isValidVariableIndex(toLevel)) {
      moveFromLevelToCore(fromLevel,toLevel);
    }
  }
 
  void simpleSifting(const int id)
  { if (isValidNodeId(id)) simpleSiftingCore(id); }
 
  std::string toString(const int id) const
  { return isValidNodeId(id) ? toStringCore(id) : "ERROR"; }
 
  int nodeCount(const int id)
  { return isValidNodeId(id) ? nodeCountCore(id) : -1; }
    
  inline void dumpUniqueTable() const;
    
  inline void dumpNodes() const;
    
  inline void dumpLevelToVar() const;

  // initial number of nodes instantiated with std::vector<BddNode>::resize();
  static const int INITIAL_NODE_INSTANCE_COUNT;
    
private:

    inline int findOrAdd(const int index, const int high, const int low);

    inline bool isValidVariableIndex(const int index) const
    { return index >= 0 && index < var_to_level_.size() ||
      index == TERMINAL_NODE_VARIABLE_INDEX; }

		inline int varToLevelCore(const int variableIndex) const;

		inline int level2varCore(const int level) const;

    inline int indexCore(const int id) const
    { return nodes_[id].index(); }

    inline int highCore(const int id) const
    { return nodes_[id].high(); }

    inline int lowCore(const int id) const
    { return nodes_[id].low(); }

    int opNotCore(const int id);

    int opAndCore(const int left, const int right);

    int opOrCore(const int left, const int right);

    int opXorCore(const int left, const int right);
		
    inline void swapLevelsCore(const int parentLevel);
		
    int dagIdSetCore(const int id, std::set<int>& ids);

    int dagIndexSetCore(const int id, std::set<int>& indexes);

    int dagLevelSetCore(const int id, std::set<int>& levels);

    int dagLevelToVarCore(const int id, std::map<int,int>& levelsToVars);

    void simpleSiftingCore(const int id);
		
    inline int siftToInitialLevel(const int index, const int topLevel,
                                  const int bottomLevel);
                                  
    inline void moveFromLevelToCore(const int fromLevel, const int toLevel);

    inline std::string toStringCore(const int id) const;
		
    inline int nodeCountCore(const int id)
    { std::set<int> ids; return dagIdSetCore(id,ids); }

    // data members
		
    int node_count_;
    std::vector<BddNode> nodes_;
    std::vector<int> var_to_level_;
    std::vector<int> level_to_var_;
    UniqueTable unique_table_;
    ComputedTable computed_table_;
 
    // class data members
    static const int TERMINAL_NODE_VARIABLE_INDEX;
    // initial number of nodes allocated with std::vector<BddNode>::reserve();
    static const int INITIAL_NODE_ALLOCATION_SIZE;
    //static const BddNode DEFAULT_NODE_INSTANCE;
};



inline BddManager::BddManager() :
  computed_table_(INITIAL_NODE_INSTANCE_COUNT << 3)
{
  //std::cout << "Begin of BddManager::BddManager()" << std::endl;

  //std::cout << "call reserve(" << INITIAL_NODE_ALLOCATION_SIZE << ")" << std::endl;
  nodes_.reserve(INITIAL_NODE_ALLOCATION_SIZE);
  //std::cout << "reserved" << std::endl;

  //std::cout << "call to resize(" << INITIAL_NODE_INSTANCE_COUNT << ")" << std::endl;
  nodes_.resize(INITIAL_NODE_INSTANCE_COUNT,BddNode());
  //std::cout << "resized" << std::endl;
  
  //computed_table_.resize(INITIAL_NODE_INSTANCE_COUNT << 3);

  nodes_[0].set(TERMINAL_NODE_VARIABLE_INDEX,0,0);
  nodes_[1].set(TERMINAL_NODE_VARIABLE_INDEX,1,1);

  node_count_ = 2;

  //std::cout << "End of BddManager::BddManager()" << std::endl;
}



inline BddManager::~BddManager()
{
  std::cout << "BddManager :: ~BddManager()\n-> " << node_count_ << " nodes from " << nodes_.size() << std::endl;

  for (std::vector<int>::const_iterator it = level_to_var_.begin();
        it != level_to_var_.end(); ++it)
    std::cout << *it << std::endl;
}



inline int BddManager::findOrAdd(const int index, const int high, const int low)
{
  int result;
  
  if (high == low)
    result = low;
  else {
    result = unique_table_.find(index,high,low);
  
    if (result == -1) {
      result = node_count_;

      // node vector resize trigger
      // \todo make resize memory secure
      if (result == nodes_.size())
        nodes_.resize(result << 1);

      // add new node to unique table
      if (!unique_table_.add(index,high,low,result))
        std::cout << "Error: BddManager :: unique table collision" << std::endl;

      // set new node
      nodes_[result].set(index,high,low);
      
      // increase node counter
      ++node_count_;
    }
  }

  return result;
}



inline void BddManager::createVariables(const int nVariables)
{
  if (nVariables > 0) {
    const int size = var_to_level_.size() + nVariables;
     
    unique_table_.extend(nVariables);

    var_to_level_.reserve(size);
    level_to_var_.reserve(size);
     
    for (int i = var_to_level_.size(); i < size; ++i) {
      var_to_level_.push_back(i);
      level_to_var_.push_back(i);
      
      findOrAdd(i,1,0);
      findOrAdd(i,0,1);
    }
  }
}



inline int BddManager::variable(const int variableIndex) const
{
  int result = -1;
  
  if (variableIndex >= 0 && variableIndex < var_to_level_.size())
    result = unique_table_.find(variableIndex,1,0);
  else
    std::cout << "BddManager :: Invalid variable index " << variableIndex << std::endl;
    
  return result;
}



inline int BddManager::varToLevelCore(const int variableIndex) const
{
  assert(isValidVariableIndex(variableIndex));

  return variableIndex == TERMINAL_NODE_VARIABLE_INDEX ? 
    TERMINAL_NODE_VARIABLE_INDEX : var_to_level_[variableIndex];
}



inline int BddManager::level2varCore(const int level) const
{
  assert(isValidVariableIndex(level));

  return level == TERMINAL_NODE_VARIABLE_INDEX ? 
    TERMINAL_NODE_VARIABLE_INDEX : level_to_var_[level];
}



inline void BddManager::swapLevelsCore(const int parentLevel)
{
  assert(parentLevel >= 0 && parentLevel < (var_to_level_.size() - 1));

  const int childrenLevel = parentLevel + 1;
  const int parentIndex = level_to_var_[parentLevel];
  const int childrenIndex = level_to_var_[childrenLevel];
    
  LevelMap& parentLevelMap = unique_table_.levelMap(parentIndex);

  // iterates over unique table's level map
  for (LevelMap::iterator it = parentLevelMap.begin();
    it != parentLevelMap.end();) {

    const int parentId = it->second;
    const int parentHighId = it->first.first;
    const int parentLowId = it->first.second;
    
    BddNode& parent = nodes_[parentId];
    const BddNode& parentHigh = nodes_[parentHighId];
    const BddNode& parentLow = nodes_[parentLowId];
    
    const int parentHighLevel = varToLevelCore(parentHigh.index());
    const int parentLowLevel = varToLevelCore(parentLow.index());

    if ((parentHighLevel == childrenLevel) ||
        (parentLowLevel == childrenLevel)) {
      
      parentLevelMap.erase(it++);

      int F11,F10,F01,F00;

      if (parentHighLevel != childrenLevel)
        F11 = F10 = parentHighId;
      else {
        F11 = parentHigh.high();
        F10 = parentHigh.low();
      }

      if (parentLowLevel != childrenLevel)
        F01 = F00 = parentLowId;
      else {
        F01 = parentLow.high();
        F00 = parentLow.low();
      }
      
      const int high = findOrAdd(parentIndex,F11,F01);
      const int low = findOrAdd(parentIndex,F10,F00);

      parent.set(childrenIndex,high,low);
      
      unique_table_.add(childrenIndex,high,low,parentId);
    }
    else
      ++it;
  }

  var_to_level_[parentIndex] = childrenLevel;
  var_to_level_[childrenIndex] = parentLevel;
  
  level_to_var_[parentLevel] = childrenIndex;
  level_to_var_[childrenLevel] = parentIndex;
}



inline std::string BddManager::toStringCore(const int id) const
{
  std::ostringstream out;

  const BddNode& node = nodes_[id];
  
  out << id;
  out << ".";
  out << node.toString();
  
/*  out << ".(";
  out << node.index();
  out << ",";
  out << node.high();
  out << ",";
  out << node.low();
  out << ")";
*/  
  return out.str();
}



inline void BddManager::dumpUniqueTable() const
{
  for (int i = 0; i < var_to_level_.size(); ++i) {
      
    const LevelMap& levelMap = unique_table_.levelMap(i);

    const int lastId = levelMap.rbegin()->second;

    // iterates over unique table's level map
    for (LevelMap::const_iterator it = levelMap.begin();
      it != levelMap.end(); ++it) {
      std::cout << toString(it->second);
      if (it->second != lastId)
        std::cout << ",";
    }
    
    std::cout << std::endl;
  }
}



inline void BddManager::dumpNodes() const
{
  for (int i = 0; i < node_count_; ++i) {
    std::cout << toString(i) << std::endl;
  }
}


inline int BddManager::
siftToInitialLevel(const int index, const int topLevel, const int bottomLevel)
{
  int result;
  // move variable to closest bound level
  const int level = varToLevel(index);
  // \bug should be relative to dag position, not shared BDD levels
  const int halfLevel = topLevel + ((bottomLevel - topLevel) / 2);
  if (level <= halfLevel) {
    moveFromLevelToCore(level,topLevel);
    result = topLevel;
  }
  else {
    moveFromLevelToCore(level,bottomLevel);
    result = bottomLevel;
  }

  return result;
}


inline void BddManager::moveFromLevelToCore(const int fromLevel, const int toLevel)
{
  //cout << "moving from level " << fromLevel << " to level " << toLevel << endl;

  int level = fromLevel;
  
  if (toLevel < fromLevel)
    while (!(--level < toLevel))
      swapLevelsCore(level);
  else if (toLevel > fromLevel)
    while (level < toLevel)
      swapLevelsCore(level++);
}



inline void BddManager::dumpLevelToVar() const
{
  for (int i = 0; i < level_to_var_.size(); ++i)
    std::cout << i << " -> " << level_to_var_[i] << std::endl;
}



}

#endif // BDD__BDD_MANAGER_HXX
