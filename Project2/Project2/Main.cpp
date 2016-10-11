#include <iostream>
#include<iomanip>
#include <cmath>
#include <fstream>
#include<sstream>
#include<string>
#include<vector>
#include <cstdlib>
//#include <set>
#include<algorithm>
#include <math.h>
#include<ctime>
#include<time.h>
//#include<utility>
#include "Treap.h" 
// Basic treap.h by Mark Allen Weiss with various modifications performed by me.
// Changes included finding max tree size, shortest distance function and key variable.

using namespace std;

#define y second
#define x first

typedef pair<int, int> pointpair;

extern int countrlc, countrrc, x1, x2, y3, y2, maxsize; //Variables present in Treap.h


int main() {
	
	int choice=0;
	ifstream inp;
	Treap<int> t;
	vector <pointpair> points;
	pointpair p1, p2;
	istringstream iss;
	string str;
	int num;
	clock_t cstart, cend;

	do {

		cout << "Hello. Choose one of the following files" << std::endl;
		cout << "1: File A (1000)" << endl << "2: File B(10000)" << endl << "3:File C(1000000)" << endl << "4:quit" << endl;
		cin >> choice;
		if (choice == 1)
			 inp.open("lab1fall16a.txt");
		else if (choice == 2)
			 inp.open("lab1fall16b.txt");
		else if (choice == 3)
			 inp.open("lab1fall16c.txt");
		else if (choice == 4)
			continue;
		else
		{
			cout << "Enter correct input" << endl;
			continue;
		}

		if (!inp)
			cout << "No file";
		
		std::getline(inp, str);
		std::istringstream iss(str);
		iss >> num;
		std::cout << "Number of values: "<< num << endl;

		points.resize(num);
		t.makeEmpty();

		for (int i = 0; i < num; i++)
		{
			std::getline(inp, str);
			std::istringstream iss(str);
			iss >> points[i].x;
			iss >> points[i].y;

		}

		if (choice != 3)
		{
			cstart = clock();
			int min = 100000000;
			int dis;
			cout << "Brute Force"<<endl;
			for (int i = 0; i < num - 1; i++)
			{
				for (int j = i + 1; j < num; j++)
				{
					dis = ((points[j].y - points[i].y)*(points[j].y - points[i].y)) + ((points[j].x - points[i].x)*(points[j].x - points[i].x));
					//cout << dis << endl;
					if (dis < min)
					{
						min = dis;
						x1 = points[i].x;
						y3 = points[i].y;
						x2 = points[j].x;
						y2 = points[j].y;

					}
				}
			}
			cout << "The closest pair is (" << x1 << "," << y3 << ") and (" << x2 << "," << y2 << ")" << endl;
			cout << "Shortest distance : " << min << endl;
			cend = clock();
			cout << std::fixed << setprecision(2) << "CPU time used by Brute force: "
				<< 1000.0 * (cend - cstart) / CLOCKS_PER_SEC << " ms" << endl; 

		}

		
		cout << "Sorting " << endl;
		cstart = clock();

		sort(points.begin(), points.end());

		cend = clock();
		cout << std::fixed << setprecision(2) << "CPU time for Sorting: "
			<< 1000.0 * (cend - cstart) / CLOCKS_PER_SEC << " ms" << endl;


		//cout << points.size() << endl;
			//points[0].x << points[0].y;
		cout << "Sweepline Algorithm" << endl;

		cstart = clock();
		//shortest distance initialised to the distance between first two points
		int dist = ((points[1].y - points[0].y)*(points[1].y - points[0].y) + (points[1].x - points[0].x)*(points[1].x - points[0].x));

		int leftmost = 0;
		x1 = points[1].x;
		y3 = points[1].y;
		x2 = points[0].x;
		y2 = points[0].y;

		//int in = 2, rem = 0; //Number of inserts and removals from tree
		//int xdist = 0;

		//First two points inserted into the treap
		t.insert(points[0].y, points[0].x);
		t.insert(points[1].y, points[1].x);


		for (int pos = 2; pos < num; pos++)
		{
			/*delete all points from treap whose distance
			  between corresponding x-values is greater than shortest distance dist*/
			while ((points[pos].x - points[leftmost].x)*(points[pos].x - points[leftmost].x) > dist)
			{
				t.remove(points[leftmost].y);
				//rem++;
				leftmost++;
			}

           // The below commented part of checking y axis is done in treap.h, calcDist()

			/*for (int i = leftmost; i < pos; i++)
			{
				//xdist = ((points[pos].x - points[i].x)*(points[pos].x - points[i].x));
				//if (((points[pos].x - points[i].x)*(points[pos].x - points[i].x) <= dist)
					//if((((points[pos].y - points[i].y)*(points[pos].y - points[i].y)) <= dist))

				
					//Determine points to be inserted based on distance of y values. 
					//Since we are taking distance in its square value, we don't need y-dist and y+dist distances
					/*if (((points[pos].y - points[i].y)*(points[pos].y - points[i].y)) <= dist)
					{
						t.insert(points[i].y, points[i].x);
						in++;
					}*/

				/*else
				{   if (t.contains(points[i].y))
					{
						t.remove(points[i].y);  //remove points with larger distances
						rem++;
						
					}
										
				}
			}*/
		

			//t.sizeTree(); //--> to find max size of tree. 
			
			/* Find shorter distance from all inserted points in treap. Also max tree size.
			   Implemented in treap.h by me */

			dist = t.calcDist(points[pos].y, points[pos].x, dist);

			t.insert(points[pos].y, points[pos].x); //Insert the current point
			//in++;

		}

		cend = clock();

		//cout << in << " " << rem << endl;
		cout << "Left Rotations: " << countrlc << endl;
		cout << "Right Rotations : " << countrrc << endl;
		cout << "The closest pair is (" << x1 << "," << y3 << ") and (" << x2 << "," << y2 << ")" << endl;
		cout << "Shortest distance : " << dist << endl;
		cout << "Max BST size :" << maxsize << endl;
		inp.close();


		cout << std::fixed << setprecision(2) << "CPU time for Sweepline Algorithm: "
			<< 1000.0 * (cend - cstart) / CLOCKS_PER_SEC << " ms" << endl <<endl;

		points.clear();

		//t.printTree();
		  
	} while(choice != 4);

	std::cin.get();
	return 0;
}