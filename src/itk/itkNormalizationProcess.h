#ifndef __mincNormalizationProcess_h__
#define __mincNormalizationProcess_h__

#include <vector>
#include <algorithm>
#include <itkOffset.h>
#include <itkLightObject.h>

namespace itk
{
  //! Pixel distance metric
  template <class PixelType> struct NormSampleDistance
  {
    PixelType      value;
    size_t         sample;
    double         distance;

    NormSampleDistance(PixelType _value,
                  double         _distance,
                  size_t         _sample
                 ):
      value(_value),distance(_distance),sample(_sample)
      {
      }
  };

  template<class PixelType,class OutputPixel>
    class MedianNormalization: public itk::LightObject
  {
  public:
    typedef NormSampleDistance<PixelType> NormSampleDistanceType;
    typedef std::vector<NormSampleDistanceType> SampleDistanceVector;

    typedef MedianNormalization             Self;
    itkTypeMacro(MedianNormalization, itk::LightObject);
    
    typedef itk::SmartPointer<Self>        Pointer;
    typedef itk::SmartPointer<const Self>  ConstPointer;
    
    itkNewMacro(Self);
    
  protected:
    struct compare_labels
    {
      bool operator()(const NormSampleDistanceType &a,const NormSampleDistanceType &b) const
      {
        return a.distance<b.distance;
      }
    };

  public:

    OutputPixel default_value(void) const
    {
      return 0;
    }

    void process(SampleDistanceVector &samples,
                    OutputPixel &output_value
                ) const
    {
      output_value=default_value();

      if(samples.empty())
        return ; 

      compare_labels cmp;
      std::sort(samples.begin(),samples.end(),cmp);

      output_value=samples[samples.size()/2.0].value;
      //output_value=samples[samples.size()/2].distance;
      //output_value=2.0;
      //output_value=samples.size();
    }

    bool operator!=(const MedianNormalization &a)
    {
      return true;
    }
  };

} //minc

#endif //__mincNormalizationProcess_h__