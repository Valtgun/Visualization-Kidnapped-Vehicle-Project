/* Funcitons for reading from files from Udacity Github repo:
 * https://github.com/udacity/CarND-Kidnapped-Vehicle-Project
 *
 *
 *
 */

#include <iostream>
#include <ctime>
#include <iomanip>
#include <random>

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"

#include "helper_functions.h"

using namespace std;
using namespace cv;

int main() {

	// Read map data
	Map map;
	if (!read_map_data("data/map_data.txt", map)) {
		cout << "Error: Could not open map file" << endl;
		return -1;
	}

	// Read position data
	vector<control_s> position_meas;
	if (!read_control_data("data/control_data.txt", position_meas)) {
		cout << "Error: Could not open position/control measurement file" << endl;
		return -1;
	}

	// Read ground truth data
	vector<ground_truth> gt;
	if (!read_gt_data("data/gt_data.txt", gt)) {
		cout << "Error: Could not open ground truth data file" << endl;
		return -1;
	}


  namedWindow("Visualizing Kidnapped Vehicle", WINDOW_AUTOSIZE );

/*
  for (int i=0; i<gt.size();i++)
  {
    Point p;
    p.x = int(gt[i].x)*2+160;
    p.y = int(gt[i].y)*2+280;
    circle(image, p, 4, Scalar(0,0,255));
  }
*/
  int last_frame = 1;
  bool show_l = false;
  int zoom = 64;
  for (;;)
  {
    Mat image(400,800, CV_8UC3, Scalar(255,255,255));
    Mat image_l(400,800, CV_8UC3, Scalar(255,255,255));
    //rectangle(image, Point(20,60), Point(80, 160), Scalar(255,0,0));
    arrowedLine(image, Point(160,0), Point(160,400),Scalar(0,255,0));
    arrowedLine(image, Point(0,280), Point(800,280),Scalar(0,255,0));
    for (int i=0; i<map.landmark_list.size();i++)
    {
      Point p;
      p.x = int(map.landmark_list[i].x_f)*2+160;
      p.y = int(map.landmark_list[i].y_f)*2+280;
      circle(image, p, 4, Scalar(0,0,255));
    }

    for (int frame = 1; frame<last_frame;frame++)
    {
      Point p_this;
      p_this.x = int(gt[frame].x)*2+160;
      p_this.y = int(gt[frame].y)*2+280;
      Point p_last;
      p_last.x = int(gt[frame-1].x)*2+160;
      p_last.y = int(gt[frame-1].y)*2+280;
      line(image, p_last, p_this, Scalar(0,0,255));
    }
    Point p;
    p.x = int(gt[last_frame].x)*2+160;
    p.y = int(gt[last_frame].y)*2+280;
    double dir = gt[last_frame].theta;
    circle(image, p, 4, Scalar(0,0,255),FILLED);
    Point p_targ;
    p_targ.x = 16*cos(dir)+p.x;
    p_targ.y = 16*sin(dir)+p.y;
    arrowedLine(image, p, p_targ,Scalar(0,0,255));

    // draw particles at last frame
    ostringstream file;
    file << "out/out_particles_" << setfill('0') << setw(6) << last_frame+1 << "_pred.txt";
    vector<Particle> particles;
    if (!read_particle_data(file.str(), particles)) {
			cout << "Error: Could not open particle file " << last_frame+1 << endl;
			return -1;
		}

    for (int i = 0; i < particles.size(); i++)
    {
      Point p;
      p.x = int(particles[i].x)*2+160;
      p.y = int(particles[i].y)*2+280;
      double dir = particles[i].theta;
      circle(image, p, 2, Scalar(255,0,0),FILLED);
      Point p_targ;
      p_targ.x = 4*cos(dir)+p.x;
      p_targ.y = 4*sin(dir)+p.y;
      arrowedLine(image, p, p_targ,Scalar(255,0,0));
    }

    // Local map generation
    for (int i=0; i<map.landmark_list.size();i++)
    {
      Point p;
      p.x = int((map.landmark_list[i].x_f-gt[last_frame].x)*zoom)+400;
      p.y = int((map.landmark_list[i].y_f-gt[last_frame].y)*zoom)+200;
      circle(image_l, p, 4, Scalar(0,0,255));
    }

    Point pc;
    pc.x = 400;
    pc.y = 200;
    double dirc = gt[last_frame].theta;
    circle(image_l, pc, 16, Scalar(0,0,255));
    Point pc_targ;
    pc_targ.x = 32*cos(dirc)+pc.x;
    pc_targ.y = 32*sin(dirc)+pc.y;
    arrowedLine(image_l, pc, pc_targ,Scalar(0,0,255));

    for (int i = 0; i < particles.size(); i++)
    {
      Point p;
      p.x = int((particles[i].x-gt[last_frame].x)*zoom)+400;
      p.y = int((particles[i].y-gt[last_frame].y)*zoom)+200;
      double dir = particles[i].theta;
      circle(image_l, p, 2, Scalar(255,0,0),FILLED);
      Point p_targ;
      p_targ.x = 8*cos(dir)+p.x;
      p_targ.y = 8*sin(dir)+p.y;
      arrowedLine(image_l, p, p_targ,Scalar(255,0,0));
    }

    // print help text
    putText(image,"Press Q to Exit!", Point(300,40),  FONT_HERSHEY_TRIPLEX, .5, Scalar(0,0,0), 1, LINE_AA);
    putText(image,"F,G,H - back 100,10,1", Point(300,60),  FONT_HERSHEY_TRIPLEX, .5, Scalar(0,0,0), 1, LINE_AA);
    putText(image,"J,K,L - forward 1,10,100", Point(300,80),  FONT_HERSHEY_TRIPLEX, .5, Scalar(0,0,0), 1, LINE_AA);
    putText(image,"Red - landmarks and car", Point(550,40),  FONT_HERSHEY_TRIPLEX, .5, Scalar(0,0,0), 1, LINE_AA);
    putText(image,"Green - map axis", Point(550,60),  FONT_HERSHEY_TRIPLEX, .5, Scalar(0,0,0), 1, LINE_AA);
    putText(image,"Blue - particles", Point(550,80),  FONT_HERSHEY_TRIPLEX, .5, Scalar(0,0,0), 1, LINE_AA);
    putText(image,"V - switch to local/map view", Point(400,100),  FONT_HERSHEY_TRIPLEX, .5, Scalar(0,0,0), 1, LINE_AA);
    putText(image_l,"Press Q to Exit!", Point(300,40),  FONT_HERSHEY_TRIPLEX, .5, Scalar(0,0,0), 1, LINE_AA);
    putText(image_l,"F,G,H - back 100,10,1", Point(300,60),  FONT_HERSHEY_TRIPLEX, .5, Scalar(0,0,0), 1, LINE_AA);
    putText(image_l,"J,K,L - forward 1,10,100", Point(300,80),  FONT_HERSHEY_TRIPLEX, .5, Scalar(0,0,0), 1, LINE_AA);
    putText(image_l,"Red - landmarks and car", Point(550,40),  FONT_HERSHEY_TRIPLEX, .5, Scalar(0,0,0), 1, LINE_AA);
    putText(image_l,"Green - map axis", Point(550,60),  FONT_HERSHEY_TRIPLEX, .5, Scalar(0,0,0), 1, LINE_AA);
    putText(image_l,"Blue - particles", Point(550,80),  FONT_HERSHEY_TRIPLEX, .5, Scalar(0,0,0), 1, LINE_AA);
    putText(image_l,"V - switch to local/map view", Point(400,100),  FONT_HERSHEY_TRIPLEX, .5, Scalar(0,0,0), 1, LINE_AA);
    putText(image_l,"Z-zoom in, X-zoom out", Point(400,120),  FONT_HERSHEY_TRIPLEX, .5, Scalar(0,0,0), 1, LINE_AA);

    if (show_l)
    {
      imshow("Visualizing Kidnapped Vehicle", image_l);
    }
    else {
      imshow("Visualizing Kidnapped Vehicle", image);
    }


    char key = (char)waitKey();
    if( key == 27 || key == 'q' || key == 'Q' ) // 'ESC'
      break;
    if (key == 'l' || key == 'L')
    {
      last_frame += 100;
      if (last_frame>=gt.size()) last_frame=gt.size()-1;
    }
    if (key == 'k' || key == 'K')
    {
      last_frame += 10;
      if (last_frame>=gt.size()) last_frame=gt.size()-1;
    }
    if (key == 'j' || key == 'J')
    {
      last_frame += 1;
      if (last_frame>=gt.size()) last_frame=gt.size()-1;
    }
    if (key == 'h' || key == 'H')
    {
      last_frame -= 1;
      if (last_frame<0) last_frame=0;
    }
    if (key == 'g' || key == 'G')
    {
      last_frame -= 10;
      if (last_frame<0) last_frame=0;
    }
    if (key == 'f' || key == 'F')
    {
      last_frame -= 100;
      if (last_frame<0) last_frame=0;
    }
    if (key == 'v' || key == 'V')
    {
      show_l = !show_l;
    }
    if (key == 'z' || key == 'Z')
    {
      zoom = zoom*2;
      if (zoom>256) zoom=256;
    }
    if (key == 'x' || key == 'X')
    {
      zoom = int(zoom/2);
      if (zoom<8) zoom=8;
    }
  }
	return 0;
}
