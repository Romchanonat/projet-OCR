#ifndef HOUGHTRANSFORM_H
#define HOUGHTRANSFORM_H

//tuple Rho/Theta for the value of the new line to draw
typedef struct{
	double rho;
	double theta;
}tuple_RhoTheta;

//the main function how do the HoughTransform of the image
void HoughTransform(SDL_Surface* surface);

#endif
