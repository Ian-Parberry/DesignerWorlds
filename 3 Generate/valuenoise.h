/// \file valuenoise.h
/// \brief Interface for the designer world class CDesignerWorld.

//Copyright Ian Parberry, November 2013.
//
//This file is made available under the GNU All-Permissive License.
//
//Copying and distribution of this file, with or without
//modification, are permitted in any medium without royalty
//provided the copyright notice and this notice are preserved.
//This file is offered as-is, without any warranty.

#pragma once

#define s_curve(t)  ((t)*(t)*(3.0f - 2.0f*(t)))
#define lerp(t, a, b) (a + (t)*(b - a))

/// \brief Designer World generator.
///
/// The Designer World generator makes 2D noise suitable for terrain height maps.
/// It combines Value Noise with a height distribution.

class CDesignerWorld{
  private:
    static const int SIZE = 256; ///< Size of height table;
    static const int MASK = 0xFF; ///< Mask for height table indices.

    float m_fPosition[SIZE]; ///< Value table.
    int m_nPermute[SIZE]; ///< Permutation table. Holds a random permutation.
    void InitSampleTable(float scale); /// Fill sample table.
    float noise(float x, float z); ///< Noise generator.  

  public:
    void Initialize(); ///< Initialize.
    float GetHeight(float x, float z, float a, float b, int n); ///< Get height.
    void SetValueTable(int table[], const int n); //< Set value table.
}; //CDesignerWorld