/*
 * OctoMap - An Efficient Probabilistic 3D Mapping Framework Based on Octrees
 * http://octomap.github.com/
 *
 * Copyright (c) 2009-2013, K.M. Wurm and A. Hornung, University of Freiburg
 * All rights reserved.
 * License: New BSD
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of Freiburg nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <octomap/octomap.h>
#include <octomap/OcTree.h>
#include <octomap/ColorOcTree.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;
using namespace octomap;
using namespace cv;

int pos_x, pos_y;
int x, y, z, lebar;
long int panjang1 = 0;
long int panjang2 = 10;
long int lebar1 = -10;
long int lebar2 = 100;
int flag = 0;
int flag2;

long int tl1 = 0;
long int tl2 = 10;

void print_query_info(point3d query, OcTreeNode* node) {
  if (node != NULL) {
    cout << "occupancy probability at " << query << ":\t " << node->getOccupancy() << endl;
  }
  else 
    cout << "occupancy probability at " << query << ":\t is unknown" << endl;    
}

int main(int argc, char** argv) {

  cout << endl;
  cout << "generating example map" << endl;

  OcTree tree (0.1);  // create empty tree with resolution 0.1
  ColorOcTree treeCol(0.1);

  // insert some measurements of occupied cells


  // OPEN CV TEST
  //cv::Mat img;
  //img = cv::imread("predictions.jpg", CV_LOAD_IMAGE_COLOR);

  VideoCapture cap("out.mp4");
  double fps = cap.get(CV_CAP_PROP_FPS);

  // Define value
  int panjang, storex, storey, line1, line2;

  if(!cap.isOpened())
  	cout << "Error" << endl;

  while(1)
  {
  	Mat imagw, gray_image, gray_image2, lines_image2, canny_image2, src_morph2;
  	Mat frame;

  	cap >> frame;
  	if(frame.empty())
  	{
  		printf("Enyee");
  		break;
  	}

  	cvtColor(frame, gray_image, COLOR_BGR2GRAY);
  	Mat copy = gray_image.clone();

  	Rect gambarroi2 = Rect(20, 890, 70, 100);
  	Mat roi2 = copy(gambarroi2);
  	threshold(roi2, gray_image2, 120, 255, THRESH_BINARY);
  	Mat element55(2, 2, CV_8U, cv::Scalar(1));
  	morphologyEx(gray_image2, src_morph2, MORPH_ERODE, element55);
  	Canny(src_morph2, canny_image2, 100, 200, 3);
  	cvtColor(canny_image2, lines_image2, CV_GRAY2BGR);

  	// Detect Linme
  	std::vector<Vec4i> lines2;
  	HoughLinesP(canny_image2, lines2, 1, CV_PI / 180, 10, 8, 0);

  	for(size_t i = 0; i < lines2.size(); i++)
  	{
  		Vec4i l2 = lines2[i];
  		line(lines_image2, Point(l2[0], l2[1]), Point(l2[2], l2[3]), Scalar(0, 0, 255), 3, CV_AA);
  		panjang = l2[3] - l2[1];
  		int abs_panjang = abs(panjang);
  		if(19 <= l2[0] && l2[0] <= 22)
  		{
  			cout << "Belok kanan biasa" << endl;
  			lebar = 1;
  			flag2 = 1;
  		}
  		else if(10 <= l2[0] && l2[0] <= 18)
  		{
  			cout << "Belok kanan tajam" << endl;
  			lebar = 2;
  			flag2 = 2;
  		}
  		else if(52 <= l2[0] && l2[0] <= 56)
  		{
  			cout << "Belok kanan biasa" << endl;
  			lebar = 1;
  			flag2 = 1;
  		}
  		else if(43 <= l2[0] && l2[0] <= 51)
  		{
  			cout << "Belok kanan tajam" << endl;
  			lebar = 2;
  			flag2 = 2;
  		}
  		else if(22 >= l2[0] && l2[0] >=  56)
  		{
  			cout << "Lurus" << endl;
  			lebar = 0;
  			flag2 = 0;
  		}
  	}
  	flag += flag2;

  	cout << "Lebar1 : " << lebar1 << endl;
  	cout << "Lebar2 : " << lebar2 << endl;

  	// Every 2 curve it will be 1 curve
  	if(flag >= 10)
  	{
  		lebar1 -= lebar;
  		lebar2 -= lebar;
  		flag = 0;
  	}
  	else
  	{
  		lebar1 = lebar1;
  		lebar2 = lebar2;
  	}

  	// Check flag
  	cout << "Out flag" << endl;
  	panjang1 += 5;
  	panjang2 += 5;

  	tl1 += 5;
  	tl2 += 5;

  	cout << "Lebar1 after : " << lebar1 << endl;
  	cout << "Lebar2 after : " << lebar2 << endl;

  	// Create the road
  	for(x = panjang1; x <= panjang2; x++){
  		for(y = lebar1; y <= lebar2; y++){
  			for(z = -1; z <= 0; z++){
  				point3d endpoint((float)x*0.025f, (float)y*0.025f, (float)z*0.025f);
  				ColorOcTreeNode*node = treeCol.updateNode(endpoint, true);
  				node -> setColor(77, 77, 62);
  			}
  		}
  	}

  	//for(x = tl1; x <= tl2; x++){
  	//	for(y = 110; y <= 160; y++){
  	//		for(z = -1; z < 0; z++){
  	//			point3d endpoint((float)x*0.025f, (float)y*0.025f, (float)z*0.025f);
  	//			ColorOcTreeNode*node = treeCol.updateNode(endpoint, true);
  	//			node -> setColor(255, 0, 0);
  	//		}
  	//	}
  	//}

  	imshow("Video", frame);
  	if(waitKey(30) >= 0)
  		break;
  }

  //START ADD MY PROGRAM
  int x, y;
  int i = 0;
  int koorx[200], koory[200];

  
//HERE
  //for (x=-10; x<=8460; x++) {
  //  for (y=180; y<=210; y++) {
 //     for (z= -1; z<0; z++) {
  //      point3d endpoint ((float) x*0.025f, (float) y*0.025f, (float) z*0.025f);
  //      ColorOcTreeNode*node = treeCol.updateNode(endpoint, true);
  //      node->setColor(77,77,62);
  //    }
   // }
  //}


  std::fstream file("/home/ferdyan/octomap_project/object/0.txt", std::ios_base::in);

  //while (file >> x >> y)
  //{
    //koorx[i] = 540 - y*0.54 ;
    //koory[i] = 90 - x*0.857;

   // printf("Koorx = %d", koorx[i]);
   //printf("Koory = %d", koory[i]);

   // for (x = koorx[i]; x<= koorx[i] + 43; x++) {
    //  for (y = koory[i] - 9 ; y<= koory[i] + 9; y++) {
  //      for (z= 1; z <= 13; z++) {
    //      point3d endpoint ((float) x*0.025f, (float) y*0.025f, (float) z*0.025f);
          //tree.updateNode(endpoint, true); // integrate 'occupied' measurement

//          ColorOcTreeNode*node = treeCol.updateNode(endpoint, true);
      //    node->setColor(255,0,0);
     //   }
   //   }
  //  }
 //   i++;
 // }
//






  cout << endl;
  cout << "performing some queries:" << endl;
  
  point3d query (0., 0., 0.);
  ColorOcTreeNode* result = treeCol.search (query);
  print_query_info(query, result);

  query = point3d(-1.,-1.,-1.);
  result = treeCol.search (query);
  print_query_info(query, result);

  query = point3d(1.,1.,1.);
  result = treeCol.search (query);
  print_query_info(query, result);


  cout << endl;
  //tree.writeBinary("simple_tree_new.bt");
  treeCol.write("/home/ferdyan/octomap/build/simple_tree_new.ot");
  cout << "wrote example file simple_tree.bt" << endl << endl;
  cout << "now you can use octovis to visualize: octovis simple_tree.bt"  << endl;
  cout << "Hint: hit 'F'-key in viewer to see the freespace" << endl  << endl;  
  //imshow("Gray window", img);
  cap.release();
  destroyAllWindows();
  waitKey();
  return 0;
}
