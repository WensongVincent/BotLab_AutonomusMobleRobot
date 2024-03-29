#include <planning/frontiers.hpp>
#include <planning/motion_planner.hpp>
#include <utils/grid_utils.hpp>
#include <common_utils/timestamp.h>
#include <slam/occupancy_grid.hpp>
#include <mbot_lcm_msgs/robot_path_t.hpp>
#include <queue>
#include <set>
#include <cassert>


bool is_frontier_cell(int x, int y, const OccupancyGrid& map);
frontier_t grow_frontier(Point<int> cell, const OccupancyGrid& map, std::set<Point<int>>& visitedFrontiers);
mbot_lcm_msgs::robot_path_t path_to_frontier(const frontier_t& frontier,
                                              const mbot_lcm_msgs::pose_xyt_t& pose,
                                              const OccupancyGrid& map,
                                              const MotionPlanner& planner);
mbot_lcm_msgs::pose_xyt_t nearest_navigable_cell(mbot_lcm_msgs::pose_xyt_t pose,
                                                  Point<float> desiredPosition,
                                                  const OccupancyGrid& map,
                                                  const MotionPlanner& planner);
mbot_lcm_msgs::pose_xyt_t search_to_nearest_free_space(Point<float> position,
                                                        const OccupancyGrid& map,
                                                        const MotionPlanner& planner);
double path_length(const mbot_lcm_msgs::robot_path_t& path);


std::vector<frontier_t> find_map_frontiers(const OccupancyGrid& map, 
                                           const mbot_lcm_msgs::pose_xyt_t& robotPose,
                                           double minFrontierLength)
{
    /*
    * To find frontiers, we use a connected components search in the occupancy grid. Each connected components consists
    * only of cells where is_frontier_cell returns true. We scan the grid until an unvisited frontier cell is
    * encountered, then we grow that frontier until all connected cells are found. We then continue scanning through the
    * grid. This algorithm can also perform very fast blob detection if you change is_frontier_cell to some other check
    * based on pixel color or another condition amongst pixels.
    */
    std::vector<frontier_t> frontiers;
    std::set<Point<int>> visitedCells;
    
    Point<int> robotCell = global_position_to_grid_cell(Point<float>(robotPose.x, robotPose.y), map);
    std::queue<Point<int>> cellQueue;
    cellQueue.push(robotCell);
    visitedCells.insert(robotCell);
  
    // Use a 4-way connected check for expanding through free space.
    const int kNumNeighbors = 4;
    const int xDeltas[] = { -1, 1, 0, 0 };
    const int yDeltas[] = { 0, 0, 1, -1 };
    
    // Do a simple BFS to find all connected free space cells and thus avoid unreachable frontiers
    while(!cellQueue.empty())
    {
        Point<int> nextCell = cellQueue.front();
        cellQueue.pop();
        
        // Check each neighbor to see if it is also a frontier
        for(int n = 0; n < kNumNeighbors; ++n)
        {
            Point<int> neighbor(nextCell.x + xDeltas[n], nextCell.y + yDeltas[n]);
            
            // If the cell has been visited or isn't in the map, then skip it
            if(visitedCells.find(neighbor) != visitedCells.end() || !map.isCellInGrid(neighbor.x, neighbor.y))
            {
                continue;
            }
            // If it is a frontier cell, then grow that frontier
            else if(is_frontier_cell(neighbor.x, neighbor.y, map))
            {
                frontier_t f = grow_frontier(neighbor, map, visitedCells);
                
                // If the frontier is large enough, then add it to the collection of map frontiers
                if(f.cells.size() * map.metersPerCell() >= minFrontierLength)
                {
                    frontiers.push_back(f);
                }
            }
            // If it is a free space cell, then keep growing the frontiers
            else if(map(neighbor.x, neighbor.y) < 0)
            {
                visitedCells.insert(neighbor);
                cellQueue.push(neighbor);
            }
        }
    }    
    return frontiers;
}

struct CompareCentroids
{
    CompareCentroids(mbot_lcm_msgs::pose_xyt_t robotPose) { this->robotPose = robotPose;}
    inline bool operator() (const Point<double>& centr_1, const Point<double>& centr_2)
    {
        // Diff 1
        float diff_1_x = robotPose.x - centr_1.x;
        float diff_1_y = robotPose.y - centr_1.y;
        float diff_1 = diff_1_x * diff_1_x + diff_1_y * diff_1_y;
        // Diff 2
        float diff_2_x = robotPose.x - centr_2.x;
        float diff_2_y = robotPose.y - centr_2.y;
        float diff_2 = diff_2_x * diff_2_x + diff_2_y * diff_2_y;

        return (diff_1 < diff_2);
    }
    mbot_lcm_msgs::pose_xyt_t robotPose;
};

frontier_processing_t plan_path_to_frontier(const std::vector<frontier_t>& frontiers, 
                                            const mbot_lcm_msgs::pose_xyt_t& robotPose,
                                            const OccupancyGrid& map,
                                            const MotionPlanner& planner)
{
    ///////////// TODO: Implement your strategy to select the next frontier to explore here //////////////////
    /*
    * NOTES:
    *   - If there's multiple frontiers, you'll need to decide which to drive to.
    *   - A frontier is a collection of cells, you'll need to decide which one to attempt to drive to.
    *   - The cells along the frontier might not be in the configuration space of the robot, so you won't necessarily
    *       be able to drive straight to a frontier cell, but will need to drive somewhere close.
    */

    // First, choose the frontier to go to
    // Initial alg: find the nearest one

    int unreachable_frontiers = 0;

    if (frontiers.size() == 0) {
        mbot_lcm_msgs::robot_path_t path;
        
        return frontier_processing_t(path, unreachable_frontiers);
    }
    
    // calculate the number of unreachable frontiers
    
    frontier_t closest_frontier = frontiers.at(0);
    CompareCentroids compare(robotPose);

    //find the closest frontier
    for (auto& frontier : frontiers) {
        if(compare(find_frontier_centroid(frontier), find_frontier_centroid(closest_frontier))) {
                closest_frontier = frontier;
        }
    }
    // std::cout << "FRONTIERS: Check 1" << std::endl;

    // initialize the closest reachable point in global grid
    Point<double> closestPointGlobal; closestPointGlobal.x = robotPose.x; closestPointGlobal.y = robotPose.y;

    // find the closest reachable point near the closest frontier
    // centriod of current frontier
    Point<int> f_c = global_position_to_grid_cell(find_frontier_centroid(closest_frontier), map); 
    // BFS parameters
    std::queue<Point<int>> cellQueue;
    cellQueue.push(f_c);
    std::set<Point<int>> visitedCell;
    const int kNumNeighbors = 8;
    const int xDeltas[] = { -1, -1, -1, 1, 1, 1, 0, 0 };
    const int yDeltas[] = {  0,  1, -1, 0, 1,-1, 1,-1 };
    
    // do BFS for 3000 points near the centriod of current frontier
    int count = 0;
    bool while_break = false;
    while (count < 3000 && !while_break) { // threshold to find near point
        // take the first point
        Point<int> nextCell = cellQueue.front();
        cellQueue.pop();
        // find all points near the current point
        for (int i =0; i<kNumNeighbors; i++) {
            Point<int> neighbor(nextCell.x + xDeltas[i], nextCell.y + yDeltas[i]);
            Point<double> neighborGlobal = grid_position_to_global_position(neighbor, map);
            // printf("FRONTIERS: Neighbor(Global): (%f,%f)\n", neighborGlobal.x, neighborGlobal.y);
            // if point is not yet explored, put it into set and queue, add count 
            if (visitedCell.find(neighbor) == visitedCell.end()) {
                visitedCell.insert(neighbor);
                cellQueue.push(neighbor);
                count++;
                // if point is reachable, assign it to closestPointGlobal
                if (is_centroid_reachable(neighborGlobal, robotPose, map, planner)) {
                    closestPointGlobal.x = neighborGlobal.x;
                    closestPointGlobal.y = neighborGlobal.y;
                    while_break = true;
                    break;

                    // std::cout << "FRONTIERS: neighbor is reachable" << std::endl;
                    // if (compare(neighborGlobal, closestPointGlobal)){
                    //     closestPointGlobal.x = neighborGlobal.x;
                    //     closestPointGlobal.y = neighborGlobal.y;
                    //     // printf("FRONTIERS: ClosestNeighbor(Global): (%f,%f)\n", neighborGlobal.x, neighborGlobal.y);
                    // }
                }
            }
        }
    }
    std::cout << "FRONTIERS: count:" << count << std::endl;

    // // frontier is unreachable
    // if (!is_centroid_reachable(, robotPose, map, planner)) {
    //     unreachable_frontiers ++;
    //     std::cout << "FRONTIERS: unreachable_frontiers: " << unreachable_frontiers << std::endl;
    // }
    // // frontier is reachable, judge whether it is the closest one so far
    // else {
    //     // CompareCentroids compare the distance of centriod of this frontier with the closest one
        
    // }

    //plan path to go to or appproach to the nearby valid point of center of nearest frontier_t 
    mbot_lcm_msgs::pose_xyt_t goal;
    goal.utime = utime_now();
    goal.theta = 0;
    goal.x = closestPointGlobal.x;
    goal.y = closestPointGlobal.y;

    mbot_lcm_msgs::robot_path_t path;
    path.utime = utime_now();

    printf("Frontier start: (%f,%f)\n", robotPose.x, robotPose.y);
    printf("Frontier goal: (%f,%f)\n", goal.x, goal.y);
    path = planner.planPath(robotPose, goal); // planPath will check whether the goal is valid, and return the length 1 path when 
    std::cout << "FRONTIERS: current path length: " << path.path_length << std::endl;
    // std::cout << "FRONTIERS: Check 2" << std::endl;

    // int count = 1; bool foundCell = false;
    // if(map(f_c.x, f_c.y) < 0) {count = 30; foundCell = true;}
    // while (count < 30 || foundCell) {
    //     f_c.x += count;
    //     if(map(f_c.x, f_c.y) < 0) {foundCell = true; break;}
    //     f_c.x -= 2*count;
    //     if(map(f_c.x, f_c.y) < 0) {foundCell = true; break;}
    //     f_c.x += count; f_c.y += count;
    //     if(map(f_c.x, f_c.y) < 0) {foundCell = true; break;}
    //     f_c.y -= 2*count;
    //     if(map(f_c.x, f_c.y) < 0) {foundCell = true; break;}
    //     f_c.y += count;

    //     count += 1;
    // }
    // int unreachable_frontiers = 0;
    // if (foundCell) {
    //     Point<double> f_g = grid_position_to_global_position(f_c, map);
    //     goal.x = f_g.x; goal.y = f_g.y;
    //     path = planner.planPath(robotPose, goal);
        
    // }
    // else {
    //     // Returnable path
    //     path.path_length = 1;
    //     path.path.push_back(robotPose);
    //     unreachable_frontiers = 0;
    // }

    return frontier_processing_t(path, unreachable_frontiers);
}


bool is_frontier_cell(int x, int y, const OccupancyGrid& map)
{
    // A cell is a frontier if it has log-odds 0 and a neighbor has log-odds < 0
    
    // A cell must be in the grid and must have log-odds 0 to even be considered as a frontier
    if(!map.isCellInGrid(x, y) || (map(x, y) != 0))
    {
        return false;
    }
    
    const int kNumNeighbors = 4;
    const int xDeltas[] = { -1, 1, 0, 0 };
    const int yDeltas[] = { 0, 0, 1, -1 };
    
    for(int n = 0; n < kNumNeighbors; ++n)
    {
        // If any of the neighbors are free, then it's a frontier
        // Note that logOdds returns 0 for out-of-map cells, so no explicit check is needed.
        if(map.logOdds(x + xDeltas[n], y + yDeltas[n]) < 0)
        {
            return true;
        }
    }
    
    return false;
}


frontier_t grow_frontier(Point<int> cell, const OccupancyGrid& map, std::set<Point<int>>& visitedFrontiers)
{
    // Every cell in cellQueue is assumed to be in visitedFrontiers as well
    std::queue<Point<int>> cellQueue;
    cellQueue.push(cell);
    visitedFrontiers.insert(cell);
    
    // Use an 8-way connected search for growing a frontier
    const int kNumNeighbors = 8;
    const int xDeltas[] = { -1, -1, -1, 1, 1, 1, 0, 0 };
    const int yDeltas[] = {  0,  1, -1, 0, 1,-1, 1,-1 };
 
    frontier_t frontier;
    
    // Do a simple BFS to find all connected frontier cells to the starting cell
    while(!cellQueue.empty())
    {
        Point<int> nextCell = cellQueue.front();
        cellQueue.pop();
        
        // The frontier stores the global coordinate of the cells, so convert it first
        frontier.cells.push_back(grid_position_to_global_position(nextCell, map));
        
        // Check each neighbor to see if it is also a frontier
        for(int n = 0; n < kNumNeighbors; ++n)
        {
            Point<int> neighbor(nextCell.x + xDeltas[n], nextCell.y + yDeltas[n]);
            if((visitedFrontiers.find(neighbor) == visitedFrontiers.end()) 
                && (is_frontier_cell(neighbor.x, neighbor.y, map)))
            {
                visitedFrontiers.insert(neighbor);
                cellQueue.push(neighbor);
            }
        }
    }
    
    return frontier;
}

Point<double> find_frontier_centroid(const frontier_t& frontier)
{
    // Using the mid point of the frontier
    Point<double> mid_point;
    int index = (int)(frontier.cells.size() / 2.0);
    // printf("index: %d, size: %d\n", index, frontier.cells.size());
    mid_point = frontier.cells[index];
    printf("Mid point of frontier: (%f,%f)\n", mid_point.x, mid_point.y);

    return mid_point;
}

bool is_centroid_reachable(const Point<double>& centroid, 
                            const mbot_lcm_msgs::pose_xyt_t& robotPose,
                            const OccupancyGrid& map,
                            const MotionPlanner& planner) 
{
    mbot_lcm_msgs::pose_xyt_t goal;
    goal.theta = 0; goal.utime = utime_now();
    goal.x = centroid.x; goal.y = centroid.y;
    // if the path length is 1, return false
    if (planner.planPath(robotPose, goal).path_length <= 1) {
        // std::cout << "FRONTIERS: This point is unreachable" << std::endl;
        return false;   
    }
    return true;
    // std::cout << "FRONTIERS: This point is reachable!" << std::endl;
}