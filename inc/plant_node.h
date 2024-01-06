#ifndef PLANT_NODE_H
#define PLANT_NODE_H

// Include any necessary libraries or headers
typedef void (*plant_node_dryness_threshold_observer)(int);
// Define your class or struct
void plant_node_init(void);
int plant_node_get_dryness(void);
int plant_node_set_max_dryness(int threshold);
int plant_node_get_max_dryness(void);
void plant_node_set_dryness_threshold_observer(plant_node_dryness_threshold_observer);
// Declare any member variables or functions

#endif // PLANT_NODE_H
