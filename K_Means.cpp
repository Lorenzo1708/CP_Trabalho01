// Adaptado de: https://reasonabledeviations.com/2019/10/02/k-means-in-cpp/

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <omp.h>

#define NO_EXECUTIONS 1

using namespace std;

struct Point {
    double x, y;     // coordinates
    int cluster;     // no default cluster
    double minDist;  // default infinite distance to nearest cluster

    Point() : x(0.0), y(0.0), cluster(-1), minDist(__DBL_MAX__) {}
    Point(double x, double y) : x(x), y(y), cluster(-1), minDist(__DBL_MAX__) {}

    /**
     * Computes the (square) euclidean distance between this point and another
     */
    double distance(Point p) {
        return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y);
    }
};

/**
 * Reads in the data.csv file into a vector of points
 * @return vector of points
 *
 */
vector<Point> readcsv() {
    vector<Point> points;
    string line;
    ifstream file("Data/MallData.csv");

    while (getline(file, line)) {
        stringstream lineStream(line);
        string bit;
        double x, y;
        getline(lineStream, bit, ',');
        x = stof(bit);
        getline(lineStream, bit, '\n');
        y = stof(bit);

        points.push_back(Point(x, y));
    }
    return points;
}

/**
 * Perform k-means clustering
 * @param points - pointer to vector of points
 * @param epochs - number of k means iterations
 * @param k - the number of initial centroids
 */
void kMeansClustering(vector<Point>* points, int epochs, int k, int noThreads) {
    omp_set_num_threads(noThreads);
    
    int n = points->size();

    // Randomly initialise centroids
    // The index of the centroid within the centroids vector
    // represents the cluster label.
    vector<Point> centroids;
    srand(time(0));
    for (int i = 0; i < k; ++i) {
        centroids.push_back(points->at(rand() % n));
    }

    for (int i = 0; i < epochs; ++i) {
        // For each centroid, compute distance from centroid to each point
        // and update point's cluster if necessary
        for (vector<Point>::iterator c = begin(centroids); c != end(centroids); ++c) {
            int clusterId = c - begin(centroids);

            for (vector<Point>::iterator it = points->begin(); it != points->end(); ++it) {
                Point p = *it;
                double dist = c->distance(p);
                if (dist < p.minDist) {
                    p.minDist = dist;
                    p.cluster = clusterId;
                }
                *it = p;
            }
        }

        // Create vectors to keep track of data needed to compute means
        vector<int> nPoints;
        vector<double> sumX, sumY;
        for (int j = 0; j < k; ++j) {
            nPoints.push_back(0);
            sumX.push_back(0.0);
            sumY.push_back(0.0);
        }

        // Iterate over points to append data to centroids
        for (vector<Point>::iterator it = points->begin(); it != points->end(); ++it) {
            int clusterId = it->cluster;
            nPoints[clusterId] += 1;
            sumX[clusterId] += it->x;
            sumY[clusterId] += it->y;

            it->minDist = __DBL_MAX__;  // reset distance
        }

        // Compute the new centroids
        for (vector<Point>::iterator c = begin(centroids); c != end(centroids); ++c) {
            int clusterId = c - begin(centroids);
            c->x = sumX[clusterId] / nPoints[clusterId];
            c->y = sumY[clusterId] / nPoints[clusterId];
        }
    }

    // Write to CSV
    ofstream myfile;
    
    switch (noThreads)
    {
        case 1:
            myfile.open("Output/Output_1_Thread.csv");
            
            break;

        case 2:
            myfile.open("Output/Output_2_Threads.csv");
            
            break;

        case 4:
            myfile.open("Output/Output_4_Threads.csv");
            
            break;

        case 8:
            myfile.open("Output/Output_8_Threads.csv");
            
            break;

        default:
            break;
    }

    myfile << "X,Y,C" << endl;

    for (vector<Point>::iterator it = points->begin(); it != points->end(); ++it) {
        myfile << it->x << "," << it->y << "," << it->cluster << endl;
    }

    myfile.close();
}

int main(int argc, char* argv[]) {
    int noThreads = atoi(argv[argc - 1]);

    vector<Point> points = readcsv();

    double executionsTime = 0.0;

    for (int j = 0; j < (int) (NO_EXECUTIONS); j++)
    {
        double executionTime = omp_get_wtime();

        kMeansClustering(&points, 262144, 5, noThreads);

        executionsTime += omp_get_wtime() - executionTime;
    }

    printf("[%d] %lfs\n", noThreads, executionsTime / (double) (NO_EXECUTIONS));
}
