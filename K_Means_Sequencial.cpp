// Adaptado de: https://reasonabledeviations.com/2019/10/02/k-means-in-cpp/

#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <omp.h>

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
void kMeansClustering(vector<Point> points, int epochs, int k) {
    int n = points.size();

    // Randomly initialise centroids
    // The index of the centroid within the centroids vector
    // represents the cluster label.
    vector<Point> centroids;
    srand(time(0));

    for (int i = 0; i < k; ++i) {
        centroids.push_back(points.at(rand() % n));
    }

    for (int i = 0; i < epochs; ++i) {
/*
        // For each centroid, compute distance from centroid to each point
        // and update point's cluster if necessary
        for (vector<Point>::iterator c = begin(centroids); c != end(centroids); ++c) {
            int clusterId = c - begin(centroids);

            for (vector<Point>::iterator it = points.begin(); it != points.end(); ++it) {
                Point p = *it;
                double dist = c->distance(p);
                if (dist < p.minDist) {
                    p.minDist = dist;
                    p.cluster = clusterId;
                }
                *it = p;
            }
        }
*/
        // For each centroid, compute distance from centroid to each point
        // and update point's cluster if necessary
        for (int j = 0; j < centroids.size(); j++) {
            int clusterId = j;

            for (int k = 0; k < points.size(); k++) {
                Point p = points[k];

                double dist = centroids[j].distance(p);

                if (dist < p.minDist)
                {
                    p.minDist = dist;
                    p.cluster = clusterId;
                }

                points[k] = p;
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
/*
        // Iterate over points to append data to centroids
        for (vector<Point>::iterator it = points->begin(); it != points->end(); ++it) {
            int clusterId = it->cluster;
            nPoints[clusterId] += 1;
            sumX[clusterId] += it->x;
            sumY[clusterId] += it->y;

            it->minDist = __DBL_MAX__;  // reset distance
        }
*/
        // Iterate over points to append data to centroids
        for (int j = 0; j < points.size(); j++) {
            int clusterId = points[j].cluster;
            nPoints[clusterId] += 1;
            sumX[clusterId] += points[j].x;
            sumY[clusterId] += points[j].y;

            points[j].minDist = __DBL_MAX__;  // reset distance
        }
/*
        // Compute the new centroids
        for (vector<Point>::iterator c = begin(centroids); c != end(centroids); ++c) {
            int clusterId = c - begin(centroids);
            c->x = sumX[clusterId] / nPoints[clusterId];
            c->y = sumY[clusterId] / nPoints[clusterId];
        }
*/
        // Compute the new centroids
        for (int j = 0; j < centroids.size(); j++) {
            int clusterId = j;
            centroids[j].x = sumX[clusterId] / nPoints[clusterId];
            centroids[j].y = sumY[clusterId] / nPoints[clusterId];
        }
    }

    // Write to CSV
    ofstream myfile;
    
    myfile.open("Output/Output_Sequencial.csv");

    myfile << "X,Y,C" << endl;

    for (vector<Point>::iterator it = points.begin(); it != points.end(); ++it) {
        myfile << it->x << "," << it->y << "," << it->cluster << endl;
    }

    myfile.close();
}

int main() {
    vector<Point> points = readcsv();

    double executionTime = omp_get_wtime();

    kMeansClustering(points, 128, 5);

    printf("[S] %lfs\n", omp_get_wtime() - executionTime);
}
