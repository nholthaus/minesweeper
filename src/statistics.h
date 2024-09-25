//--------------------------------------------------------------------------------------------------
//
//	STATISTICS
//
//--------------------------------------------------------------------------------------------------
//
// The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//--------------------------------------------------------------------------------------------------
//
// Copyright (c) 2018 Nic Holthaus
//
//--------------------------------------------------------------------------------------------------
//
// ATTRIBUTION:
//
//
//--------------------------------------------------------------------------------------------------
//
/// @file	statistics.h
/// @brief	A class to measure various statistics of a population. All operations (except iterator
///			insert), are O(1), and the Statistics class uses a minimal, fixed amount of memory (i.e.
///			it does NOT keep a list of all members of its population);
//
//--------------------------------------------------------------------------------------------------

#pragma once
#ifndef statistics_h__
#define statistics_h__

//------------------------------
//	INCLUDES
//------------------------------

#include <algorithm>
#include <initializer_list>
#include <limits>
#include <utility>

//--------------------------------------------------------------------------------------------------
//	CLASS STATISTICS
//--------------------------------------------------------------------------------------------------
///	@brief		Keeps a simple average of a series of measurements
///	@details
//--------------------------------------------------------------------------------------------------
template<class T>
class Statistics
{
public:

	using T2 = decltype(T{} * T{});

public:

	/**
	 * @brief		Default Constructor
	 */
	inline Statistics()
			: m_count(T{ 0 })
			, m_max(std::numeric_limits<T>::lowest())
			, m_min(std::numeric_limits<T>::max())
			, m_sum (T{ 0 })
			, m_sumOfSquares(T2{ 0 })
	{

	}

	/**
	 * @brief		Constructor
	 * @details		O(1) complexity
	 * @param[in]	measurement	Initial measurement
	 */
	inline explicit Statistics(T measurement)
			: m_count(1)
			, m_max(measurement)
			, m_min(measurement)
			, m_sum(measurement)
			, m_sumOfSquares(measurement * measurement)
	{

	}

	/**
	 * @brief		Construct from initializer list
	 * @details		O(N) complexity.
	 * @param[in]	init	initializer list containing the population
	 */
	inline Statistics(std::initializer_list<T> init)
			: Statistics()
	{
		for (const auto& measurement : init)
			*this += measurement;
	}

	/**
	 * @brief		Constructor from input iterators
	 * @details		O(1) complexity
	 * @param[in]	seed	Starting value for the average. Defaults to 0.
	 */
	template<class InputIt>
	inline explicit Statistics(InputIt first, InputIt last)
			: Statistics()
	{
		for (InputIt itr = first; itr != last; ++itr)
			*this += *itr;
	}

	/*
	 *@brief		Clears statistics
	 * @details		O(1) complexity.
	 */
	inline void clear() noexcept
	{
		*this = std::move(Statistics());
	}

	/**
	 * @brief		Measurement count.
	 * @details		O(1) complexity.
	 * @returns		The number of measurements taken
	 */
	inline size_t count() const noexcept
	{
		return m_count;
	}

	/**
	 * @brief		Insert a measurement into the population
	 * @details		O(1) complexity. Equivalent to `+=`.
	 * @param[in]	measurement	Measurement to add to the population
	 * @returns		Reference to `this`
	 */
	inline Statistics& insert(const T& measurement) noexcept
	{
		*this += measurement;
		return *this;
	}

	/**
	 * @brief		Insert measurements into the population
	 * @details		O(N) complexity.
	 * @param[in]	first	begin iterator
	 * @param[in]	last	end iterator
	 * @returns		Reference to `this`
	 */
	template<class InputIt>
	inline Statistics& insert(const InputIt& first, const InputIt& last) noexcept
	{
		for(InputIt itr = first; itr != last; ++itr)
			*this += *itr;

		return *this;
	}

	/**
	 * @brief		Value of the mean.
	 * @details		O(1) complexity. Returns 0 if no measurements have been taken.
	 * @returns		Current value of the average
	 */
	inline T mean() const noexcept
	{
		return m_sum / std::max<size_t>(m_count, 1);
	}

	/**
	 * @brief		Returns minimum sampled value.
	 * @details		O(1) complexity.
	 */
	inline T min() const noexcept
	{
		return m_min;
	}

	/**
	 * @brief		Returns maximum sampled value.
	 * @details		O(1) complexity.
	 */
	inline T max() const noexcept
	{
		return m_max;
	}

	/**
	 * @brief		Sum of the samples
	 * @details		O(1) complexity.
	 * @returns		The sum of all sampled values
	 */
	inline T sum() const noexcept
	{
		return m_sum;
	}

	/**
	 * @brief		Returns the sum of squares of all measurements
	 * @details		O(1) complexity.
	 * @returns		T2
	 */
	inline T2 sumOfSquares() const noexcept
	{
		return m_sumOfSquares;
	}

	/**
	 * @brief		Calculates the variance of the population
	 * @details		O(1) complexity.
	 * @returns		variance
	 */
	inline auto variance() const noexcept
	{
		auto denom = std::max<size_t>(m_count, 1);
		return ((m_sumOfSquares / denom) - (m_sum / denom) * (m_sum / denom));
	}

	/**
	 * @brief		Calculates the standard deviation of the population
	 * @details		O(1) complexity.
	 * @returns		standard deviation
	 */
	inline T standardDeviation() const noexcept
	{
		return sqrt(variance());
	}

	/**
	 * @brief		Returns the average updated with the given measurement.
	 * @details		O(1) complexity.
	 * @param[in]	measurement	value to add to the average.
	 * @returns		Reference to this.
	 */
	inline Statistics& operator+=(const T& measurement) noexcept
	{
		if (measurement < m_min)
			m_min = measurement;
		else if (measurement > m_max)
			m_max = measurement;

		m_sum += measurement;
		m_sumOfSquares += measurement * measurement;

		++m_count;

		return *this;
	}

	/**
	 * @brief		Returns the average updated with the given measurement.
	 * @details		O(1) complexity.
	 * @param[in]	measurement	value to add to the average.
	 * @returns		Reference to this.
	 */
	inline Statistics& operator+=(const Statistics& rhs) noexcept
	{
		m_count += rhs.m_count;
		m_max = std::max(m_max, rhs.m_max);
		m_min = std::min(m_min, rhs.m_min);
		m_sum += rhs.m_sum;
		m_sumOfSquares += rhs.m_sumOfSquares;

		return *this;
	}

	//------------------------------
	//	FRIEND OPERATORS
	//------------------------------

	template<class U> friend Statistics<U> operator+(const Statistics<U>& lhs, const T& rhs) noexcept;
	template<class U> friend Statistics<U> operator+(const Statistics<U>& lhs, const Statistics<U>& rhs) noexcept;
	template<class U> friend bool operator==(const Statistics<U>& lhs, const Statistics<U>& rhs) noexcept;
	template<class U> friend bool operator!=(const Statistics<U>& lhs, const Statistics<U>& rhs) noexcept;
	template<class U> friend bool operator<(const Statistics<U>& lhs, const Statistics<U>& rhs) noexcept;
	template<class U> friend bool operator<=(const Statistics<U>& lhs, const Statistics<U>& rhs) noexcept;
	template<class U> friend bool operator>(const Statistics<U>& lhs, const Statistics<U>& rhs) noexcept;
	template<class U> friend bool operator>=(const Statistics<U>& lhs, const Statistics<U>& rhs) noexcept;

private:

	size_t		m_count;
	T			m_max;
	T			m_min;
	T			m_sum;
	T2			m_sumOfSquares;
};

/**
 * @brief		Adds a measurement to the population
 * @details		O(1) complexity.
 * @param[in]	population	population to add the measurement to
 * @param[in]	measurement measurement to add
 * @returns		The resulting population
 */
template<class T>
inline Statistics<T> operator+(const Statistics<T>& lhs, const T& rhs) noexcept
{
	Statistics<T> s;
	s += lhs;
	s += rhs;
	return s;
}

/**
 * @brief		Combine two populations
 * @details		O(1) complexity.
 * @param[in]	lhs	first population
 * @param[in]	rhs second population
 * @returns		The resulting population
 */
template<class T>
inline Statistics<T>& operator+(const Statistics<T>& lhs, const Statistics<T>& rhs) noexcept
{
	Statistics<T> s;
	s += lhs;
	s += rhs;
	return s;
}

/**
 * @brief		Equality operator
 * @details		O(1) complexity.
 * @param[in]	lhs left hand population
 * @param[in]	rhs	right hand population
 */
template<class T>
inline bool operator==(const Statistics<T>& lhs, const Statistics<T>& rhs) noexcept
{
	if (lhs.m_count != rhs.m_count)
		return false;
	else if (lhs.m_min != rhs.m_min)
		return false;
	else if (lhs.m_max != rhs.m_max)
		return false;
	else if (lhs.m_sum != rhs.m_sum)
		return false;
	else
		return true;
}

/**
 * @brief		Inequality operator
 * @details		O(1) complexity.
 * @param[in]	lhs left hand population
 * @param[in]	rhs	right hand population
 */
template<class T>
inline bool operator!=(const Statistics<T>& lhs, const Statistics<T>& rhs) noexcept
{
	return !(lhs == rhs);
}

/**
 * @brief		Less than operator
 * @details		O(1) complexity.
 * @param[in]	lhs left hand population
 * @param[in]	rhs	right hand population
 */
template<class T>
inline bool operator<(const Statistics<T>& lhs, const Statistics<T>& rhs) noexcept
{
	if (lhs.m_count < rhs.m_count)
		return true;
	else if (lhs.m_min < rhs.m_min)
		return true;
	else if (lhs.m_max < rhs.m_max)
		return true;
	else if (lhs.m_sum < rhs.m_sum)
		return true;
	else
		return false;
}

/**
 * @brief		Less than or equal to operator
 * @details		O(1) complexity.
 * @param[in]	lhs left hand population
 * @param[in]	rhs	right hand population
 */
template<class T>
inline bool operator<=(const Statistics<T>& lhs, const Statistics<T>& rhs) noexcept
{
	return !(rhs < lhs);
}

/**
 * @brief		Greater than operator
 * @details		O(1) complexity.
 * @param[in]	lhs left hand population
 * @param[in]	rhs	right hand population
 */
template<class T>
inline bool operator>(const Statistics<T>& lhs, const Statistics<T>& rhs) noexcept
{
	return rhs < lhs;
}

/**
 * @brief		Greater than or equal to operator
 * @details		O(1) complexity.
 * @param[in]	lhs left hand population
 * @param[in]	rhs	right hand population
 */
template<class T>
inline bool operator>=(const Statistics<T>& lhs, const Statistics<T>& rhs) noexcept
{
	return !(lhs < rhs);
}

#endif // average_h__