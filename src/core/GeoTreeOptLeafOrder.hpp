//=======================================================================
// Author: Donovan Parks
//
// Copyright 2009 Donovan Parks
//
// This file is part of GenGIS.
//
// GenGIS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GenGIS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GenGIS.  If not, see <http://www.gnu.org/licenses/>.
//=======================================================================

#ifndef _GENGIS_GEO_TREE_OPT_LEAF_ORDER_
#define _GENGIS_GEO_TREE_OPT_LEAF_ORDER_

#include "../core/Precompiled.hpp"

#include "../core/NodeGeoTree.hpp"
#include "../utils/Tree.hpp"

namespace GenGIS 
{
	typedef struct sLINEAR_RESULTS
	{
		sLINEAR_RESULTS(float _angle = 0, uint _numCrossings = 0): angle(_angle), numCrossings(_numCrossings) {}

		float angle;       // angle of geographic layout line in degrees
		uint numCrossings; // number of crossings which occurs for this layout lines

		/** Serialization for struct. */
		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			// Needs to be here
		}
	} LinearResults;

	class GeoTreeOptLeafOrder
	{
	public:
		/**
		* @brief Find optimal leaf ordering of a tree using branch and bound in order to reduce required computation.
		*				Note: This algorithm is designed to work with multifurcating trees.
		* @param tree Tree to optimize leaf node ordering of.
		* @param node Tree (subtree) to find optimal leaf ordering for.
		* @param numCrossings Number of crossings that occurs after optimizing leaf node order.
		* @param bOptimize Flag indicating if optimal ordering should be found (true) or if a heuristic ordering should be used (false).
		*/
		static void OptimizeLeafNodeOrdering(Tree<NodeGeoTree>::Ptr tree, NodeGeoTree* node, uint& numCrossings, bool bOptimize);

		/**
		* @brief Perform Monte Carlo significance test between tree topology and position of points on layout primative.
		* @param tree Tree significance test will be performed on.
		* @param node Subtree to perform significance test on.
		* @param iterations Number of iterations to perform when creating probability density function.
		* @param pdf Probability density function generated by significance test.
		* @param originalNumberCrossings Number of crossings for actual data.
		* @param bSuccess Flag indicating if test was run to completion or terminated early.
		* @return P-value from significance test.
		*/
		static double SignificanceTest(Tree<NodeGeoTree>::Ptr tree, NodeGeoTree* node, uint iterations, std::map<uint, uint>& pdf, uint& originalNumberCrossings, bool& bSuccess);

		/**
		* @brief Calculate goodness-of-fit for all linear axes.
		* @param tree Tree to perform linear axes analysis on.
		* @param node Subtree to perform analysis on.
		* @return Vector indicating number of crossings for every possible permutation of geographic locations.
		*/
		static std::vector<LinearResults> PerformLinearAxesAnalysis(Tree<NodeGeoTree>::Ptr tree, NodeGeoTree* node);

			/**
		* @brief Calculate length of all location lines.
		* @param tree Tree specifying ordering of geographic locations.
		* @param geographyLine Geography line used to layout geographic points.
		* @return Total length of location lines.
		*/
		static double CalculateLengthOfLocationLines(Tree<NodeGeoTree>::Ptr tree, const VisualLine& geographyLine);

	protected:
		/**
		* @brief Find optimal leaf ordering of a tree using branch and bound in order to reduce required computation.
		*				Note: This algorithm is designed to work with multifurcating trees.
		* @param node Tree (subtree) to find optimal leaf ordering for.
		* @param numCrossings Number of crossings that occurs after optimizing leaf node order.
		* @param bOptimize Flag indicating if optimal ordering should be found (true) or if a heuristic ordering should be used (false).
		*/
		static void BranchAndBoundTreeOpt(NodeGeoTree* node, uint& numCrossings, bool bOptimize);

		/**
		* @brief Count number of crossings between the subtrees below a given node (crossings within a subtree are not counted).
		* @param node Crossings between subtrees below this node will be counted.
		* @param leafOrder Desired order of leaf nodes (from left to right).
		* @param maxCrossings Function will return immediately after counting more than this many crossings.
		* @return Number of crossings.
		*/
		static uint CountCrossingsByInversionNumber(NodeGeoTree* node, uint maxCrossings = INT_MAX);

		/**
		* @brief Create a count matrix which indicates the number of crossings between any two subtrees below the given node.
		*				Note: crossings within a subtree are not counted.
		* @param node Crossings between subtrees below this node will be tabulated into a counting matrix.
		* @param countMatrix Count matrix to be populated.
		*/
		static void CreateCountMatrix(NodeGeoTree* node, std::vector< std::vector<uint> >& countMatrix);

		/**
		* @brief Calculate the canonical lower bound on the number of crossings.
		* @param countMatrix Count matrix indicating number of crossings between any pair of subtrees (see CreateCountMatrix).
		*/
		static uint CanonicalLowerBound(const std::vector< std::vector<uint> >& countMatrix);

		/**
		* @brief Calculate the ordering of subtrees below the given nodes as determined by the barycenter heuristic.
		* @param baryCenterOrdering The barycenter ordering.
		*/
		static void BaryCenterOrdering(NodeGeoTree* node, std::vector<uint>& baryCenterOrdering);

		/**
		* @brief Calculate the ordering of subtrees below the given nodes as determined by the median heuristic.
		* @param medianOrdering The median ordering.
		*/
		static void MedianOrdering(NodeGeoTree* node, std::vector<uint>& medianOrdering);

		/** Randomly permute a vector. */
		static void RandomPermutation(std::vector<uint>& leafOrder);

		/** Helpful routine for debugging. */
		static void PrintLayoutOrderOfLocations(const std::vector<NodeGeoTree*>& leaves); 

	protected:

	};

}

namespace boost
{
	namespace serialization
	{
		using namespace GenGIS;

		template<class Archive>
		inline void save_construct_data(Archive & ar, const LinearResults * t, const unsigned int file_version)
		{
			// Save data required to construct instance
			ar << t->angle;        // float
			ar << t->numCrossings; // uint
		}

		template<class Archive>
		inline void load_construct_data(Archive & ar, LinearResults * t, const unsigned int file_version)
		{
			// Retrieve data from archive required to construct new instance
			float angle;
			ar >> angle;

			uint numCrossings;
			ar >> numCrossings;

			::new(t)LinearResults(angle, numCrossings);
		}
	}
}

#endif
