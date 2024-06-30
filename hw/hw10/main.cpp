#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include<limits.h>

#include "image.h"
#include "priority_queue.h"

// ===================================================================================================

// distance field method functions
double NaiveDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image);
double ImprovedDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image);
double FastMarchingMethod(Image<Color> &input, Image<DistancePixel> &distance_image);

// visualization style helper functions
Color Rainbow(double distance, double max_distance);
Color GreyBands(double distance, double max_distance, int num_bands);

// ===================================================================================================

int main(int argc, char* argv[]) {
  if (argc != 5) {
    std::cerr << "Usage: " << argv[0] << " input.ppm output.ppm distance_field_method visualization_style" << std::endl;
    exit(1);
  }

  // open the input image
  Image<Color> input;
  if (!input.Load(argv[1])) {
    std::cerr << "ERROR: Cannot open input file: " << argv[1] << std::endl;
    exit(1);
  }

  // a place to write the distance values
  Image<DistancePixel> distance_image;
  distance_image.Allocate(input.Width(),input.Height());

  // calculate the distance field (each function returns the maximum distance value)
  double max_distance = 0;
  if (std::string(argv[3]) == std::string("naive_method")) {
    max_distance = NaiveDistanceFieldMethod(input,distance_image);
  } else if (std::string(argv[3]) == std::string("improved_method")) {
    max_distance = ImprovedDistanceFieldMethod(input,distance_image);
  } else if (std::string(argv[3]) == std::string("pq_with_map")) {
    max_distance = FastMarchingMethod(input,distance_image);
  } else if (std::string(argv[3]) == std::string("pq_with_hash_table")) {
    // EXTRA CREDIT: implement FastMarchingMethod with a hash table
  } else {
    std::cerr << "ERROR: Unknown distance field method: " << argv[3] << std::endl;
    exit(1);
  }

  // convert distance values to a visualization
  Image<Color> output;
  output.Allocate(input.Width(),input.Height());
  for (int i = 0; i < input.Width(); i++) {
    for (int j = 0; j < input.Height(); j++) {
      double v = distance_image.GetPixel(i,j).getValue();
      if (std::string(argv[4]) == std::string("greyscale")) {
	output.SetPixel(i,j,GreyBands(v,max_distance*1.01,1));
      } else if (std::string(argv[4]) == std::string("grey_bands")) {
	output.SetPixel(i,j,GreyBands(v,max_distance,4));
      } else if (std::string(argv[4]) == std::string("rainbow")) {
	output.SetPixel(i,j,Rainbow(v,max_distance));
      } else {
	// EXTRA CREDIT: create other visualizations 
	std::cerr << "ERROR: Unknown visualization style" << std::endl;
	exit(0);
      }
    }
  }
  // save output
  if (!output.Save(argv[2])) {
    std::cerr << "ERROR: Cannot save to output file: " << argv[2] << std::endl;
    exit(1);
  }

  return 0;
}

// ===================================================================================================

double NaiveDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image) {
  int w = input.Width();
  int h = input.Height();
  // return the maximum distance value
  double answer = 0;
  // loop over the pixels in the input image
  for (int i = 0; i < w; i++)  {
    for (int j = 0; j < h; j++) {
      double closest = -1;      
      // loop over all other pixels in the input image
      for (int i2 = 0; i2 < w; i2++)  {
	for (int j2 = 0; j2 < h; j2++) {
	  const Color& c = input.GetPixel(i2,j2);      
	  // skip all pixels that are not black
	  if (!c.isBlack()) continue;
	  // calculate the distance between the two pixels
	  double distance = sqrt((i-i2)*(i-i2) + (j-j2)*(j-j2));
	  // store the closest distance to a black pixel
	  if (closest < 0 || distance < closest) {
	    closest = distance;
	  }
	}
      }
      assert (closest >= 0);
      answer = std::max(answer,closest);
      // save the data to the distance image
      DistancePixel& p = distance_image.GetPixel(i,j);
      p.setValue(closest);
    }
  }
  return answer;
}


double ImprovedDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image) {
  int w = input.Width();
  int h = input.Height();

  //make a vector with every black pixel
  std::vector<std::pair<int, int> > bp;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      const Color& c = input.GetPixel(i,j);
      if (c.isBlack()) {
        bp.push_back(std::make_pair(i,j));
      }
    }
  }
  // return the maximum distance value
  double answer = 0;
  // loop over the pixels in the input image
  for (int i = 0; i < w; i++)  {
    for (int j = 0; j < h; j++) {
      double closest = -1;      
      // loop over all black pixels in the input image
      for (unsigned int k = 0; k < bp.size(); k++)  {    
        // calculate the distance between the two pixels
        double distance = sqrt((i-bp[k].first)*(i-bp[k].first) + (j-bp[k].second)*(j-bp[k].second));
        // store the closest distance to a black pixel
        if (closest < 0 || distance < closest) {
          closest = distance;
        }
      }
      assert (closest >= 0);
      answer = std::max(answer,closest);
      // save the data to the distance image
      DistancePixel& p = distance_image.GetPixel(i,j);
      p.setValue(closest);
    }
  }
  return answer;
}

double FastMarchingMethod(Image<Color> &input, Image<DistancePixel> &distance_image) {
  /*set the value of each black pixel to 0, and all others to INT_MAX. At the same time create a
  vector containing the black pixels.*/
  std::vector<std::pair<int, int> > b_pixels;
  for (int i = 0; i < input.Width(); i++) {
    for (int j = 0; j < input.Height(); j++) {
      const Color& c = input.GetPixel(i,j);
      DistancePixel& p = distance_image.GetPixel(i,j);
      p.setX(i);
      p.setY(j);
      if (c.isBlack()) {
        p.setValue(0);
        b_pixels.push_back(std::make_pair(i,j));
      } else {
        p.setValue(INT_MAX);
      }
    }
  }

  //for each black pixel, propogate the distance to each of its neighbors and add the neighbors to a vector
  std::vector<DistancePixel*> pixels;
  for (unsigned int i = 0; i < b_pixels.size(); i++) {
    DistancePixel& b = distance_image.GetPixel(b_pixels[i].first,b_pixels[i].second);
    for (int r = b.getX()-1; r < b.getX()+2; r++) {
      for (int c = b.getY()-1; c < b.getY()+2; c++) {
        //don't include the current pixel or anything out of bounds
        if (r >= input.Width() || c >= input.Height() || r < 0 || c < 0) {
          continue;
        }
        if (r == b.getX() && c == b.getY()) {
          continue;
        }
        DistancePixel& n = distance_image.GetPixel(r, c);
        //add the neighbor to the pixels vector if it isn't already there
        if (n.getValue() == INT_MAX) {
          pixels.push_back(&n);
        }
        //update the distance of the neighbor
        double distance = sqrt((r-b.getX())*(r-b.getX()) + (c-b.getY())*(c-b.getY()));
        if (n.getValue() > b.getValue() + distance) {
          n.setValue(b.getValue() + distance);
        }
      }
    }
  }

  double answer = 0;
  //create the heap and advance the front across the image
  DistancePixel_PriorityQueue pq(pixels);
  while (pq.size() > 0) {
    //propogate the top pixel's distance to its neighbors
    const DistancePixel* curr = pq.top();
    for (int r = curr->getX()-1; r < curr->getX()+2; r++) {
      for (int c = curr->getY()-1; c < curr->getY()+2; c++) {
        //don't include the current pixel or anything out of bounds
        if (r >= input.Width() || c >= input.Height() || r < 0 || c < 0) {
          continue;
        }
        if (r == curr->getX() && c == curr->getY()) {
          continue;
        }
        //update the distance of the neighbor
        DistancePixel& n = distance_image.GetPixel(r, c);
        double distance = sqrt((r-curr->getX())*(r-curr->getX()) + (c-curr->getY())*(c-curr->getY()));
        if (n.getValue() > curr->getValue() + distance) {
          n.setValue(curr->getValue() + distance);
          //add the pixel to the queue or update its position
          if (pq.in_heap(&n)) {
            pq.update_position(&n);
          } else {
            pq.push(&n);
          }
        }
      }
    }
    //remove the current pixel from the queue
    answer = std::max(answer, curr->getValue());
    pq.pop();
  }

  return answer;
}

// ===================================================================================================

Color Rainbow(double distance, double max_distance) {
  Color answer;
  if (distance < 0.001) {
    // black
    answer.r = 0; answer.g = 0; answer.b = 0;
  } else if (distance < 0.2*max_distance) {
    // blue -> cyan
    double tmp = distance * 5.0 / max_distance;
    answer.r = 0;
    answer.g = tmp*255;
    answer.b = 255;
  } else if (distance < 0.4*max_distance) {
    // cyan -> green
    double tmp = (distance-0.2*max_distance) * 5.0 / max_distance;
    answer.r = 0;
    answer.g = 255;
    answer.b = (1-tmp*tmp)*255;
  } else if (distance < 0.6*max_distance) {
    // green -> yellow
    double tmp = (distance-0.4*max_distance) * 5.0 / max_distance;
    answer.r = sqrt(tmp)*255;
    answer.g = 255;
    answer.b = 0;
  } else if (distance < 0.8*max_distance) {
    // yellow -> red
    double tmp = (distance-0.6*max_distance) * 5.0 / max_distance;
    answer.r = 255;
    answer.g = (1-tmp*tmp)*255;
    answer.b = 0;
  } else if (distance < max_distance) {
    // red -> white
    double tmp = (distance-0.8*max_distance) * 5.0 / max_distance;
    answer.r = 255;
    answer.g = tmp*255;
    answer.b = tmp*255;
  } else {
    // white
    answer.r = answer.g = answer.b = 255;
  }  
  return answer;
}

Color GreyBands(double distance, double max_value, int num_bands) {
  Color answer;
  if (distance < 0.001) {
    // red
    answer.r = 255; answer.g = 0; answer.b = 0;
  } else {
    // shades of grey
    answer.r = answer.g = answer.b = int(num_bands*256*distance/double(max_value)) % 256;
  }  
  return answer;
}

// ===================================================================================================
