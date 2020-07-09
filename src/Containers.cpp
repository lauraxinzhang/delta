/**
 * \file    Containers.cc
 * \author  Xin Zhang, Princeton Plasma Physics Laboratory
 * \date    June, 2020
 *
 * \brief   Implements the VecDoub and MatDoub classes
 * 
 */
#include "Containers.hpp"

MatDoub::MatDoub(int nw, int nh, double a)
		:nw_(nw), nh_(nh)
{
	data_.resize(nw_);
	std::vector<double> cols(nh_, a);
	for (int i = 0; i < nw_; i++) {
		data_[i].resize(nh_);
		data_[i] = cols;
	}
	return;
}

double& MatDoub::at(int w, int h)
{
	return data_.at(w).at(h);
}

std::vector<double>& MatDoub::getCol(int w)
{
	return data_.at(w);
}

int MatDoub::getW()
{
	return nw_;
}

int MatDoub::getH()
{
	return nh_;
}

