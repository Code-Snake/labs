#include "activities.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <iostream> 


using namespace std;

bool is_compatible(const Activity &activity1, const Activity &activity2) {
  return activity1.finish <= activity2.start ||
         activity2.finish <= activity1.start;
}

vector<Activity>get_max_activities_full_search(const vector<Activity> &activities) {
  int n = activities.size();

  if (n <= 1) {
    return activities; // Если вектор имеет 0 или 1 элемент, вернуть его как
                       // максимальное подмножество.
  }

  vector<Activity>
      maxActivities; // Вектор для хранения максимального подмножества.

  // Перебираем все возможные подмножества элементов activities.
  for (int i = 0; i < pow(2, n); i++) {
    vector<Activity> currentActivities; // Вектор для текущего подмножества.

    for (int j = 0; j < n; j++) {
      if ((i / static_cast<int>(pow(2, j))) % 2 == 1) {
        currentActivities.push_back(activities[j]);
      }
    }

    bool isCompatible = true; // Флаг, указывающий, совместимы ли все элементы
                              // текущего подмножества.

    // Проверяем, совместимы ли все элементы в текущем подмножестве.
    for (int k = 0; k < currentActivities.size(); k++) {
      for (int l = k + 1; l < currentActivities.size(); l++) {
        if (!is_compatible(currentActivities[k], currentActivities[l])) {
          isCompatible = false;
          break;
        }
      }
      if (!isCompatible) {
        break;
      }
    }

    // Если текущее подмножество совместимо и больше, чем текущее максимальное
    // подмножество, обновляем maxActivities.
    if (isCompatible && currentActivities.size() > maxActivities.size()) {
      maxActivities = currentActivities;
    }
  }

  return maxActivities; // Возвращаем максимальное совместимое подмножество.
}

vector<Activity> get_max_activities(const vector<Activity> &activities) {
  vector<Activity> sortedActivities = activities;
  if (activities.size() <= 1) {
    return activities;
  }
  sort(sortedActivities.begin(), sortedActivities.end(),
       [](const Activity &a, const Activity &b) { return a.finish < b.finish; });
    
  int current = 0;
  vector<Activity> result;
  
  while (current < sortedActivities.size()) {
    result.push_back(sortedActivities[current]);
    int next = current + 1;
    while (next < sortedActivities.size() && 
           sortedActivities[next].start < sortedActivities[current].finish) {
      next++;
    }
    current = next;
  }  
  return result;
}
