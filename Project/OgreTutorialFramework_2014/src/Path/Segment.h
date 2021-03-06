#pragma once

#ifndef _PATH_SEGMENT_
#define _PATH_SEGMENT_


// Engine headers.
#include <Path/Path.h>


/// <summary>
/// A complete cubic bezier curve which is used as a segment to create large paths.
/// </summary>
class Path::Segment final
{
    public:

        #pragma region Constructors and destructor

        /// <summary> The default constructor for the segment, initialises the size of the points vector to the correct size. </summary>
	    Segment();

        /// <summary> Constructs a segment with the four points given. </summary>
        /// <param name="p0"> The starting point of the curve. </param>
        /// <param name="p1"> The first intermediate point. </param>
        /// <param name="p2"> The second intermediate point. </param>
        /// <param name="p3"> The end point of the curve. </param>
        Segment (const Ogre::Vector3& p0, const Ogre::Vector3& p1, const Ogre::Vector3& p2, const Ogre::Vector3& p3);

        Segment (Segment&& move);
        Segment& operator= (Segment&& move);

        Segment (const Segment& copy)               = default;
        Segment& operator= (const Segment& copy)    = default;

	    ~Segment()                                  = default;

        #pragma endregion

        #pragma region Getters and setters

        /// <summary> Gets the desired point. Out of range will return the final point of the curve. </summary>
        const Ogre::Vector3& getPoint (const unsigned int index) const;

        /// <summary> Gets the most recently calculated arc length for the curve. </summary>
        /// <returns> The arc length of the curve, -1.f if the path has not been initialised. </returns>
        float getLength() const { return m_length; }

        /// <summary> Sets the desired point with the information given. </summary>
        /// <param name="index"> The desired point. Invalid values are ignored. </param>
        /// <param name="point"> The vector to set the point to. </param>
        void setPoint (const unsigned int index, const Ogre::Vector3& point);

        #pragma endregion

        #pragma region Core functionality

        /// <summary> Samples the segment to calculate the approximate arc length of the curve. This value is accessible via getLength(). </summary>
        /// <param name="samples"> The number of samples to use, the higher the more accurate. 100+ will often provide visually accurate results. </param>
        /// <returns> The calculated length, also accessible from getLength(). </returns>
        float calculateLength (const unsigned int samples);

        /// <summary> Calculates a point of the bezier curve according to the delta given. </summary>
        /// <param name="delta"> The delta value between 0.f and 1.f for the curve point. </param>
        /// <param name="derivative"> Which derivative to calculate, none will be the curve point itself, first is a tangent vector and second is a curviture vector. </param>
        /// <returns> The calculated curve point according to the given derivative. Unsupported derivatives return the curve point. </returns>
        Ogre::Vector3 curvePoint (const float delta, const Derivative derivative = Derivative::None) const;

        /// <summary> Translates every point in the segment by the vector specified. </summary>
        /// <param name="translation"> How much to translate the segment by. </param>
        void translate (const Ogre::Vector3& translation);

        /// <summary> Translates a single point by the vector specified. </summary>
        /// <param name="translation"> How much to translate the point by. </param>
        void translatePoint (const unsigned int point, const Ogre::Vector3& translation);

        /// <summary> Rotates the entire curve. </summary>
        /// <param name="rotation"> The rotation matrix to use to rotate the curve. </param>
        void rotate (const Ogre::Matrix3& rotation);

        #pragma endregion

    private:

        #pragma region Curve calculation

        /// <summary> Calculates the curve point based on a delta value. </summary>
        /// <param name="delta"> The 0.f to 1.f value representing a point on the curve. </param>
        /// <returns> The calculated vector. </returns>
        Ogre::Vector3 curvePosition (const float delta) const;

        /// <summary> Calculates the tangent vector of a given point (first derivative). </summary>
        /// <param name="delta"> The 0.f to 1.f value representing a point on the curve. </param>
        /// <returns> The calculated tangent vector. </returns>
        Ogre::Vector3 curveTangent (const float delta) const;

        /// <summary> Calculates the curvature vector of a given point (second derivative). </summary>
        /// <param name="delta"> The 0.f to 1.f value representing a point on the curve. </param>
        /// <returns> The calculated curvature vector. </returns>
        Ogre::Vector3 curveCurvature (const float delta) const;

        #pragma endregion

        #pragma region Implementation data

        std::vector<Ogre::Vector3>  m_points    {  };       //!< The list of points which make up the bezier curve.
        
        float                       m_length    { -1.f };   //!< The arc length of the bezier curve.

        #pragma endregion
};

#endif // _PATH_SEGMENT_