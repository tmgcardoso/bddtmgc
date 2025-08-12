// BddManager.cxx
// Author: tmgcardoso

#include <cassert>
#include "BddManager.hxx" 

namespace bdd {

using namespace std;

//const int BddManager::INVALID_NODE_VARIABLE_INDEX = INT_MIN;

const int BddManager::TERMINAL_NODE_VARIABLE_INDEX = INT_MAX;

const int BddManager::INITIAL_NODE_ALLOCATION_SIZE = 16384;

const int BddManager::INITIAL_NODE_INSTANCE_COUNT = 128;

//const BddNode BddManager::DEFAULT_NODE_INSTANCE = BddNode();



int BddManager::opNotCore(const int id)
{
  int result;
  
  assert(isValidNodeId(id));

  if (id == 0)
    result = 1;
  else if (id == 1)
    result = 0;
  else {
    // cache lookup
    result = computed_table_.lookupOpNot(id);
    //cout << "cache lookup(" << id << ") = " << result << endl; 
    
    if (result == -1) {       
      const BddNode& node = nodes_[id];

      const int high = opNotCore(node.high());
      const int low = opNotCore(node.low());

      result = findOrAdd(node.index(),high,low);

      computed_table_.setOpNot(id,result);
    } 
  }

  return result;
}



int BddManager::opAndCore(const int left, const int right)
{
  //cout << "Begin of opAndCore(" << toString(left) << "," << toString(right) << ")" << endl;
  
  assert(isValidNodeId(left) && isValidNodeId(right)); 
    
  int result;
  
  if (left == 0 || right == 0)
    result = 0;
  else if (left == 1 && right == 1)
    result = 1;
//  else if (left == right)
//    result = right;
  else {
    // cache lookup
    result = computed_table_.lookupOpAnd(left,right);

    if (result == -1) {
      const BddNode& nodeLeft = nodes_[left];
      const BddNode& nodeRight = nodes_[right];
    
      const int leftLevel = varToLevelCore(nodeLeft.index());    
      const int rightLevel = varToLevelCore(nodeRight.index());
    
      int topLevel,topIndex;// = min(leftLevel,rightLevel);
      if (leftLevel < rightLevel) {
        topLevel = leftLevel;
        topIndex = nodeLeft.index();
      }
      else {
        topLevel = rightLevel;
        topIndex = nodeRight.index();
      }
      //cout << "topVar=" << topVar << endl;
      //cout << "leftLevel=" << leftLevel << endl;
      //cout << "rightLevel=" << rightLevel << endl;
           
      const int high = opAndCore(topLevel < leftLevel ? left : nodeLeft.high(),
                              topLevel < rightLevel ? right : nodeRight.high());
      const int low = opAndCore(topLevel < leftLevel ? left : nodeLeft.low(),
                              topLevel < rightLevel ? right : nodeRight.low());
    
      //cout << "high=" << high << endl;
      //cout << "low=" << low << endl;
    
      result = findOrAdd(topIndex,high,low);
    
      computed_table_.setOpAnd(left,right,result);
    }
  }

  //cout << "End of opAndCore(" << left << "," << right << ") = " << toString(result) << endl;

  return result;
}



int BddManager::opOrCore(const int left, const int right)
{
  //cout << "Begin of opOrCore(" << left << "," << right << ")" << endl;
  
  assert(isValidNodeId(left) && isValidNodeId(right));
    
  int result;
  
  if (left == 1 || right == 1)
    result = 1;
  else if (left == 0 && right == 0)
    result = 0;
  else {
    // cache lookup
    result = computed_table_.lookupOpOr(left,right);

    if (result == -1) {
      const BddNode& nodeLeft = nodes_[left];
      const BddNode& nodeRight = nodes_[right];
    
      const int leftLevel = varToLevelCore(nodeLeft.index());    
      const int rightLevel = varToLevelCore(nodeRight.index());
    
      int topLevel,topIndex;// = min(leftLevel,rightLevel);
      if (leftLevel < rightLevel) {
        topLevel = leftLevel;
        topIndex = nodeLeft.index();
      }
      else {
        topLevel = rightLevel;
        topIndex = nodeRight.index();
      }

//      const int topVar = min(leftLevel,rightLevel);
      //cout << "topVar=" << topVar << endl;
      //cout << "leftLevel=" << leftLevel << endl;
      //cout << "rightLevel=" << rightLevel << endl;
           
      const int high = opOrCore(topLevel < leftLevel ? left : nodeLeft.high(),
                             topLevel < rightLevel ? right : nodeRight.high());
      const int low = opOrCore(topLevel < leftLevel ? left : nodeLeft.low(),
                             topLevel < rightLevel ? right : nodeRight.low());
    
      //cout << "high=" << high << endl;
      //cout << "low=" << low << endl;
    
      result = findOrAdd(topIndex,high,low);

      computed_table_.setOpOr(left,right,result);
    }
  }

  //cout << "End of opOrCore(" << left << "," << right << ") = " << result << endl;

  return result;
}



int BddManager::opXorCore(const int left, const int right)
{
  //cout << "Begin of opXorCore(" << left << "," << right << ")" << endl;
  
  assert(isValidNodeId(left) && isValidNodeId(right)); 
    
  int result;
  
  if (left == 1 && right == 0 ||
      left == 0 && right == 1)
    result = 1;
  else if (left == 1 && right == 1 ||
           left == 0 && right == 0)
    result = 0;
  else {
    // cache lookup
    result = computed_table_.lookupOpXor(left,right);

    if (result == -1) {
      const BddNode& nodeLeft = nodes_[left];
      const BddNode& nodeRight = nodes_[right];
    
      const int leftLevel = varToLevelCore(nodeLeft.index());    
      const int rightLevel = varToLevelCore(nodeRight.index());
    
      int topLevel,topIndex;// = min(leftLevel,rightLevel);
      if (leftLevel < rightLevel) {
        topLevel = leftLevel;
        topIndex = nodeLeft.index();
      }
      else {
        topLevel = rightLevel;
        topIndex = nodeRight.index();
      }

//      const int topVar = min(leftLevel,rightLevel);
      //cout << "topVar=" << topVar << endl;
      //cout << "leftLevel=" << leftLevel << endl;
      //cout << "rightLevel=" << rightLevel << endl;
           
      const int high = opXorCore(topLevel < leftLevel ? left : nodeLeft.high(),
                              topLevel < rightLevel ? right : nodeRight.high());
      const int low = opXorCore(topLevel < leftLevel ? left : nodeLeft.low(),
                              topLevel < rightLevel ? right : nodeRight.low());
    
      //cout << "high=" << high << endl;
      //cout << "low=" << low << endl;
    
      result = findOrAdd(topIndex,high,low);

      computed_table_.setOpXor(left,right,result);
    }
  }

  //cout << "End of opXorCore(" << left << "," << right << ") = " << result << endl;

  return result;
}



// this function could be more efficient using node marking
int BddManager::dagIdSetCore(const int id, std::set<int>& ids)
{
  if (id > 1) {
    ids.insert(id);
    
    const BddNode& node = nodes_[id];
    
    dagIdSetCore(node.high(),ids);
    dagIdSetCore(node.low(),ids);
  }
  
  return ids.size();
}



// this function could be more efficient using node marking
int BddManager::dagIndexSetCore(const int id, std::set<int>& indexes)
{
  if (id > 1) {
    const BddNode& node = nodes_[id];
    
    indexes.insert(node.index());
    
    dagIndexSetCore(node.high(),indexes);
    dagIndexSetCore(node.low(),indexes);
  }
  
  return indexes.size();
}


// this function could be more efficient using node marking
int BddManager::dagLevelToVarCore(const int id, std::map<int,int>& levelsToVars)
{
  if (id > 1) {
    const BddNode& node = nodes_[id];

    if (levelsToVars.insert(make_pair(varToLevel(node.index()),node.index())).
          second) {
      dagLevelToVarCore(node.high(),levelsToVars);
      dagLevelToVarCore(node.low(),levelsToVars);
    }
  }

  return levelsToVars.size();
}


// this function could be more efficient using node marking
int BddManager::dagLevelSetCore(const int id, std::set<int>& levels)
{
  if (id > 1) {
    const BddNode& node = nodes_[id];
    
    levels.insert(varToLevelCore(node.index()));
    
    dagLevelSetCore(node.high(),levels);
    dagLevelSetCore(node.low(),levels);
  }
  
  return levels.size();
}



void BddManager::simpleSiftingCore(const int id)
{
  map<int,int> levelsToVars;
  dagLevelToVarCore(id,levelsToVars);
  
  const int topLevel = levelsToVars.begin()->first;
  const int bottomLevel = levelsToVars.rbegin()->first;

  for (map<int,int>::const_iterator it = levelsToVars.begin();
        it != levelsToVars.end(); ++it) {
    const int index = it->second;

//    cout << "moving index=" << index << " to border" << endl;
    dumpLevelToVar();

    int level = siftToInitialLevel(index,topLevel,bottomLevel);

//    cout << "moved to border index=" << index << endl;
    dumpLevelToVar();

    if (level == topLevel) {
      int bestLevel = topLevel;
      int bestNodeCount = nodeCountCore(id);

      for (int i = topLevel; i < bottomLevel; ) {
        swapLevels(i);
        
        if (levelsToVars.find(i) != levelsToVars.end())
          if (nodeCountCore(id) < bestNodeCount) 
            bestLevel = i++;
          else
            ++i;
      }

//      cout << "moving index=" << index << " to level " << bestLevel << endl;
      dumpLevelToVar();

      moveFromLevelToCore(bottomLevel,bestLevel);

//      cout << "moved index=" << index << " to level " << bestLevel << endl;
      dumpLevelToVar();
    }
    else {
      int bestLevel = bottomLevel;
      int bestNodeCount = nodeCountCore(id);

      for (int i = bottomLevel - 1; !(i < topLevel); ) {
        swapLevels(i);

        if (levelsToVars.find(i) != levelsToVars.end())
          if (nodeCountCore(id) < bestNodeCount) 
            bestLevel = i--;
          else
            --i;
      }

//      cout << "moving index=" << index << " to level " << bestLevel << endl;
      dumpLevelToVar();

      moveFromLevelTo(topLevel,bestLevel);
      
//      cout << "moved index=" << index << " to level " << bestLevel << endl;
      dumpLevelToVar();
    }
  }
}



}
