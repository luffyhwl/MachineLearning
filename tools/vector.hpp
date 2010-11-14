/** 
 @cond
 #########################################################################
 # GPL License                                                           #
 #                                                                       #
 # This file is part of the Machine Learning Framework.                  #
 # Copyright (c) 2010, Philipp Kraus, <philipp.kraus@flashpixx.de>       #
 # This program is free software: you can redistribute it and/or modify  #
 # it under the terms of the GNU General Public License as published by  #
 # the Free Software Foundation, either version 3 of the License, or     #
 # (at your option) any later version.                                   #
 #                                                                       #
 # This program is distributed in the hope that it will be useful,       #
 # but WITHOUT ANY WARRANTY; without even the implied warranty of        #
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
 # GNU General Public License for more details.                          #
 #                                                                       #
 # You should have received a copy of the GNU General Public License     #
 # along with this program.  If not, see <http://www.gnu.org/licenses/>. #
 #########################################################################
 @endcond
 **/



/** template header file with implementation for some vector functions **/


#ifndef MACHINELEARNING_TOOLS_VECTOR_HPP
#define MACHINELEARNING_TOOLS_VECTOR_HPP

#include <limits>
#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/permutation_iterator.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>

#include "../exception/exception.h"
#include "language/language.h"



namespace boost { namespace lambda {
    
    namespace ublas         = boost::numeric::ublas;
    
    
    template<class T, class Allocator, class B>
    
    struct plain_return_type_2<other_action<subscript_action>, ublas::vector<T, Allocator>, B> {
        typedef typename ublas::vector<T, Allocator>::reference type;
    };
    
};};



namespace machinelearning { namespace tools {
    
    namespace ublas = boost::numeric::ublas;
    namespace bac   = boost::accumulators;
    namespace lam   = boost::lambda;
    
    
    /** class for vector operations **/
    class vector {
        
        public :
        
            template<typename T> static ublas::vector<T> random( const std::size_t&, const tools::random::distribution& = tools::random::uniform, const T& = std::numeric_limits<T>::epsilon(), const T& = std::numeric_limits<T>::epsilon(), const T& = std::numeric_limits<T>::epsilon() );
            template<typename T> static ublas::vector<T> pow( const ublas::vector<T>&, const T& );
            template<typename T> static std::vector<T> copy( const ublas::vector<T>& );
            template<typename T> static ublas::vector<T> copy( const std::vector<T>& );
            template<typename T> static T min( const ublas::vector<T>& );
            template<typename T> static T max( const ublas::vector<T>& );
            template<typename T> static T mean( const ublas::vector<T>& );
            template<typename T> static std::vector<T> unique( const std::vector<T>& p_vec );
            template<typename T> static ublas::vector<std::size_t> rank( ublas::vector<T>& );
            template<typename T> static ublas::vector<std::size_t> rankIndexVector( ublas::vector<T>& );
            template<typename T> static ublas::indirect_array< std::vector<std::size_t> > rankIndex( ublas::vector<T>& );
            template<typename T> static ublas::vector<T> setNumericalZero( const ublas::vector<T>&, const T& = 0 );
    };
    
    
    
    /** creates a random vector of type T
     * @param p_length vector size
     * @param p_distribution name of the distribution
     * @param p_a first value for distribution
     * @param p_b first value for distribution
     * @param p_c first value for distribution
     * @return vector of type T
     **/
    template<typename T> inline ublas::vector<T> vector::random( const std::size_t& p_length, const tools::random::distribution& p_distribution, const T& p_a, const T& p_b, const T& p_c )
    {
        if (p_length == 0)
            throw exception::runtime(_("length must be greater than zero"));
        
        // initialisation of prototypes
        tools::random l_rand;
        ublas::vector<T> l_vec(p_length);
        BOOST_FOREACH( T& i, l_vec)
            i = l_rand.get<T>( p_distribution, p_a, p_b, p_c );
        
        return l_vec;
    }
    
    
    /** run for every item within the vector the math pow function
     * @param p_vec input vector
     * @param p_exponent exponent
     * @return "exponential" vector
     **/    
    template<typename T> inline ublas::vector<T> vector::pow( const ublas::vector<T>& p_vec, const T& p_exponent )
    {
        ublas::vector<T> l_vec(p_vec);
        BOOST_FOREACH( T& i, l_vec)
            i = std::pow(i, p_exponent);
        
        return l_vec;
    }    
    
    
    /** copy a blas vector to a standard vector
     * @param p_vec blas vector
     * @return std::vector
     **/
    template<typename T> inline std::vector<T> vector::copy( const ublas::vector<T>& p_vec )
    {
        std::vector<T> l_vec;
        std::copy(p_vec.begin(), p_vec.end(), std::back_inserter(l_vec));        
        return l_vec;
    }
    
    
    /** copy a std::vector to a blas vector
     * @param p_vec std::vector
     * @return ublas::vector
     **/
    template<typename T> inline ublas::vector<T> vector::copy( const std::vector<T>& p_vec )
    {
        ublas::vector<T> l_vec(p_vec.size());
        
        for(std::size_t i=0; i < p_vec.size(); ++i)
            l_vec(i) = p_vec[i];
        
        return l_vec;
    }    
    
      
    /** returns the minimum of the vector
     * @param p_vec input vector
     * @return minimum of the vector
     **/    
    template<typename T> inline T vector::min( const ublas::vector<T>& p_vec )
    {
        bac::accumulator_set<T, bac::stats< bac::tag::min > > l_acc;
        std::for_each( p_vec.begin(), p_vec.end(), boost::bind<void>( boost::ref(l_acc), _1 ));
        return bac::min(l_acc);
    }

    
    /** returns the maximum of the vector
     * @param p_vec input vector
     * @return maximum of the vector
     **/    
    template<typename T> inline T vector::max( const ublas::vector<T>& p_vec )
    {
        bac::accumulator_set<T, bac::stats< bac::tag::max > > l_acc;
        std::for_each( p_vec.begin(), p_vec.end(), boost::bind<void>( boost::ref(l_acc), _1 ));
        return bac::max(l_acc);
    }
    
    
    /** returns the mean / average of the vector elements
     * @param p_vec input vector
     * @return mean of the vector
     **/    
    template<typename T> inline T vector::mean( const ublas::vector<T>& p_vec )
    {
        bac::accumulator_set<T, bac::stats< bac::tag::mean > > l_acc;
        std::for_each( p_vec.begin(), p_vec.end(), boost::bind<void>( boost::ref(l_acc), _1 ));
        return bac::mean(l_acc);
    }
    
    
    /** returns a std::vector that elements are unique
     * @param p_vec input std::vector
     * @return std::vector with unique elements
    **/
    template<typename T> inline std::vector<T> vector::unique( const std::vector<T>& p_vec )
    {
        std::vector<T> l_unique( p_vec );
        std::sort( l_unique.begin(), l_unique.end() );
        l_unique.erase(  std::unique( l_unique.begin(), l_unique.end()), l_unique.end()  );
        
        return l_unique;
    }
    
    /** ranks the elements of the vector, each element
     * holds the rank value [0..n-1]. Every value
     * ist the ranking position 0, the smales, n-1 the biggest
     * @param p_vec vector with elements
     * @return rank vector
     **/
    template<typename T> inline ublas::vector<std::size_t> vector::rank( ublas::vector<T>& p_vec )
    {
        
        std::vector<std::size_t> temp(boost::counting_iterator<std::size_t>(0), boost::counting_iterator<std::size_t>(p_vec.size()));
        std::sort(temp.begin(), temp.end(), lam::var(p_vec)[lam::_1] < lam::var(p_vec)[lam::_2]);
        
        ublas::vector<std::size_t> l_ranking(temp.size());
        std::copy( boost::counting_iterator<std::size_t>(0), boost::counting_iterator<std::size_t>(p_vec.size()),
                  boost::make_permutation_iterator(l_ranking.begin(), temp.begin())
                  );
        
        return l_ranking;
    }
    
    
    /** ranks the element of the vector and
     * returns a vector with index of the elements
     * first element is the smallest, second the next grater element and so on
     * @param p_vec vector with elements
     * @return index vector
     **/
    template<typename T> inline ublas::vector<std::size_t> vector::rankIndexVector( ublas::vector<T>& p_vec )
    {
        std::vector<std::size_t> temp(boost::counting_iterator<std::size_t>(0), boost::counting_iterator<std::size_t>(p_vec.size()));
        std::sort(temp.begin(), temp.end(), lam::var(p_vec)[lam::_1] < lam::var(p_vec)[lam::_2]);
        
        ublas::vector<std::size_t> l_rank( temp.size() );
        std::copy( temp.begin(), temp.end(), l_rank.begin() );
        
        return l_rank;
        
    }
    
    
    /** ranks the element of the vector and
     * returns a index array for using ublas::project
     * first element is the smallest, second the next grater element and so on
     * @param p_vec vector with elements
     * @return index array
     **/
    template<typename T> inline ublas::indirect_array< std::vector<std::size_t> > vector::rankIndex( ublas::vector<T>& p_vec )
    {
        std::vector<std::size_t> l_temp(boost::counting_iterator<std::size_t>(0), boost::counting_iterator<std::size_t>(p_vec.size()));
        std::sort( l_temp.begin(), l_temp.end(), lam::var(p_vec)[lam::_1] < lam::var(p_vec)[lam::_2]);
        
        return ublas::indirect_array< std::vector<std::size_t> >(p_vec.size(), l_temp);
    }
    
    
    /** changes numerical zero / datatype limit to a fixed value
     * @param p_vec input vector
     * @param p_val fixed vakue
     * @return changed vector
     **/
    template<typename T> inline ublas::vector<T> vector::setNumericalZero( const ublas::vector<T>& p_vec, const T& p_val)
    {
        ublas::vector<T> l_vec(p_vec);
        
        BOOST_FOREACH( T& i, l_vec)
            if (tools::function::isNumericalZero(i))
                i = p_val;
        
        return l_vec;
    }    
    
    
    
};};

#endif