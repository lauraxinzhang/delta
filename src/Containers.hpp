/**
 * \file    Containers.h
 * \author  Xin Zhang, Princeton Plasma Physics Laboratory
 * \date    June, 2020
 *
 * \brief   Declares the VecDoub and MatDoub classes
 * 
 */

#ifndef CONTAINER_H_INCLUDED
#define CONTAINER_H_INCLUDED 1

#include <vector>

typedef std::vector<double> VecDoub;

/**
 * \brief Data container class for 2D arrays
 */
class MatDoub
{
	public:
		/**
		 * \brief Constructs a nw x nh array, filled with value a
		 * \param nw Number of horizontal elements
		 * \param nh Number of vertical elements
		 * \param a  Default to 0. Fill array with constant values.
		 */
		MatDoub(int nw, int nh, double a = 0);

		/**
		 * \brief Element access
		 * \param w Row index
		 * \param h Col index
		 * \return An editable reference to matrix element
		 */
		double& at(int w, int h);

		/**
		 * \brief Get an editable reference to a certain column
		 */
		std::vector<double>& getCol(int w);

		/**
		 * \brief get number of elements in a row
		 */
		int getW();

		/**
		 * \brief get number of elements in a col
		 */
		int getH();

		

	private:
		// Default constructor disabled.
		MatDoub();

		std::vector<std::vector<double>> data_;
		int nw_;
		int nh_;
};

#endif // CONTAINER_H_INCLUDED
